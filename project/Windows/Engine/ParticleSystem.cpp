#include "ParticleSystem.h"
#include "VGL.h"
#include "Log.h"
#include <stdlib.h>

// Shader uniform handles for update program
static int s_hMinLifetime   = -1;
static int s_hMaxLifetime   = -1;
static int s_hMinColor      = -1;
static int s_hMaxColor      = -1;
static int s_hMinVelocity   = -1;
static int s_hMaxVelocity   = -1;
static int s_hMinSize       = -1;
static int s_hMaxSize       = -1;
static int s_hGravity       = -1;
static int s_hSeed          = -1;
static int s_hDeltaTime     = -1;
static int s_hGenerate      = -1;
static int s_hOrigin        = -1;
static int s_hSpawnVariance = -1;

// Shader attribute handles for update program
static int s_hPosition    = -1;
static int s_hVelocity    = -1;
static int s_hColor       = -1;
static int s_hLife        = -1;
static int s_hSize        = -1;

// Shader uniform handles for render program
static int s_hRendMatrixVP  = -1;
static int s_hTexType       = -1;
static int s_hRendTexture   = -1;

// Shader attribute handles for render program
static int s_hRendPosition = -1;
static int s_hRendColor    = -1;
static int s_hRendLife     = -1;
static int s_hRendSize     = -1;

// Class-static variables
int ParticleSystem::s_nStaticInit = 0;

//*****************************************************************************
// Constructor
//*****************************************************************************
ParticleSystem::ParticleSystem()
{
    m_nClass = ACTOR_PARTICLE;

    m_nGenerate = 1;
    m_nParticleCount = 1000;

    m_nEmissionRate = 20;

    m_fMinLifetime = 2.0f;
    m_fMaxLifetime = 4.0f;

    m_arMinColor[0] = 1.0f;
    m_arMinColor[1] = 1.0f;
    m_arMinColor[2] = 1.0f;
    m_arMinColor[3] = 1.0f;

    m_arMaxColor[0] = 1.0f;
    m_arMaxColor[1] = 1.0f;
    m_arMaxColor[2] = 1.0f;
    m_arMaxColor[3] = 1.0f;

    m_arMinVelocity[0] = -0.5f;
    m_arMinVelocity[1] =  1.0f;
    m_arMinVelocity[2] = -0.5f;

    m_arMaxVelocity[0] = 0.5f;
    m_arMaxVelocity[1] = 2.0f;
    m_arMaxVelocity[2] = 0.5f;

    m_fMinSize = 0.1f;
    m_fMaxSize = 0.2f;

    m_arGravity[0] =  0.0f;
    m_arGravity[1] = -1.0f;
    m_arGravity[2] =  0.0f;

    m_arOrigin[0] = 0.0f;
    m_arOrigin[1] = 0.0f;
    m_arOrigin[2] = 0.0f;

    m_arSpawnVariance[0] = 0.1f;
    m_arSpawnVariance[1] = 0.1f;
    m_arSpawnVariance[2] = 0.1f;

    m_pTexture = 0;

    m_arVBOs[0] = 0;
    m_arVBOs[1] = 0;

    m_nUpdateBuffer = 0;

    m_pSync = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
ParticleSystem::~ParticleSystem()
{
    // Free video memory
    // Allow for a glError if the VBOs are 0.
    glDeleteBuffers(2, m_arVBOs);
}

//*****************************************************************************
// Initialize
//*****************************************************************************
void ParticleSystem::Initialize()
{   
    int hProg         = 0;
    char* pZeroBuffer = 0;

    // Perform static initialization
    if (s_nStaticInit == 0)
    {
        hProg = GetShaderProgram(PARTICLE_UPDATE_PROGRAM);

        s_hMinLifetime   = glGetUniformLocation(hProg, "uMinLifetime");
        s_hMaxLifetime   = glGetUniformLocation(hProg, "uMaxLifetime");
        s_hMinColor      = glGetUniformLocation(hProg, "uMinColor");
        s_hMaxColor      = glGetUniformLocation(hProg, "uMaxColor");
        s_hMinVelocity   = glGetUniformLocation(hProg, "uMinVelocity");
        s_hMaxVelocity   = glGetUniformLocation(hProg, "uMaxVelocity");
        s_hMinSize       = glGetUniformLocation(hProg, "uMinSize");
        s_hMaxSize       = glGetUniformLocation(hProg, "uMaxSize");
        s_hGravity       = glGetUniformLocation(hProg, "uGravity");
        s_hSeed          = glGetUniformLocation(hProg, "uSeed");
        s_hDeltaTime     = glGetUniformLocation(hProg, "uDeltaTime");
        s_hGenerate      = glGetUniformLocation(hProg, "uGenerate");
        s_hOrigin        = glGetUniformLocation(hProg, "uOrigin");
        s_hSpawnVariance = glGetUniformLocation(hProg, "uSpawnVariance");

        s_hPosition = glGetAttribLocation(hProg, "aPosition");
        s_hVelocity = glGetAttribLocation(hProg, "aVelocity");
        s_hColor    = glGetAttribLocation(hProg, "aColor");
        s_hLife     = glGetAttribLocation(hProg, "aLife");
        s_hSize     = glGetAttribLocation(hProg, "aSize");

        hProg = GetShaderProgram(PARTICLE_RENDER_PROGRAM);

        s_hRendMatrixVP  = glGetUniformLocation(hProg, "uMatrixVP");
        s_hTexType       = glGetUniformLocation(hProg, "uTexType");
        s_hRendTexture   = glGetUniformLocation(hProg, "uTexture");

        s_hRendPosition = glGetAttribLocation(hProg, "aPosition");
        s_hRendColor    = glGetAttribLocation(hProg, "aColor");
        s_hRendLife     = glGetAttribLocation(hProg, "aLife");
        s_hRendSize     = glGetAttribLocation(hProg, "aSize");

#if defined (WINDOWS)
        // Lastly, we need to enable point size control in the vertex shader.
        // On windows, Vakz runs in a compatibility context, so it is necessary
        // to enable this setting so that the fixed function glPointSize() is not 
        // used instead. On android (running a pure ES 3.0 environment) this call
        // is not needed and would probably produce an error if it was used.
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_POINT_SPRITE);
#endif

        s_nStaticInit = 1;
    }

    // Kick off timer
    m_timer.Start();

    // Create and allocate the two buffers that will be swapped
    // when performing transform feedback
    glGenBuffers(2, m_arVBOs);

    pZeroBuffer = new char[m_nParticleCount * PARTICLE_DATA_SIZE];
    memset(pZeroBuffer, 0, m_nParticleCount * PARTICLE_DATA_SIZE);

    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 PARTICLE_DATA_SIZE * m_nParticleCount,
                 pZeroBuffer,
                 GL_DYNAMIC_COPY);

    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 PARTICLE_DATA_SIZE * m_nParticleCount,
                 pZeroBuffer,
                 GL_DYNAMIC_COPY);

    delete [] pZeroBuffer;
}

//*****************************************************************************
// Update
//*****************************************************************************
void ParticleSystem::Update()
{
    float fDeltaTime = 0.0f;
    int hProg = GetShaderProgram(PARTICLE_UPDATE_PROGRAM);

    m_timer.Stop();
    fDeltaTime = m_timer.Time();

    glUseProgram(hProg);
    
    // Toggle the update buffer index
    m_nUpdateBuffer = !m_nUpdateBuffer;
    
    // Bind the correct buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[m_nUpdateBuffer]);
    glBindBuffer ( GL_TRANSFORM_FEEDBACK_BUFFER, m_arVBOs[!m_nUpdateBuffer]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_arVBOs[!m_nUpdateBuffer]);

    // Assign vertex attributes
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          0);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 12);
    glVertexAttribPointer(2,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 24);
    glVertexAttribPointer(3,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 40);
    glVertexAttribPointer(4,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 44);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Set Uniforms
    glUniform1f(s_hMinLifetime, m_fMinLifetime);
    glUniform1f(s_hMaxLifetime, m_fMaxLifetime);
    glUniform4fv(s_hMinColor, 1, m_arMinColor);
    glUniform4fv(s_hMaxColor, 1, m_arMaxColor);
    glUniform3fv(s_hMinVelocity, 1, m_arMinVelocity);
    glUniform3fv(s_hMaxVelocity, 1, m_arMaxVelocity);
    glUniform1f(s_hMinSize, m_fMinSize);
    glUniform1f(s_hMaxSize, m_fMaxSize);
    glUniform3fv(s_hGravity, 1, m_arGravity);
    glUniform1i(s_hSeed, rand());
    glUniform1f(s_hDeltaTime, fDeltaTime);
    glUniform1i(s_hGenerate, m_nGenerate);
    glUniform3fv(s_hOrigin, 1, m_arOrigin);
    glUniform3fv(s_hSpawnVariance, 1, m_arSpawnVariance);
    
    // Since this shader is only being used for transform feedback,
    // we don't want anything to be drawn to the framebuffer.
    // To disable drawing, enable GL_RASTERIZE_DISCARD
    glEnable(GL_RASTERIZER_DISCARD);

    // Update particles with transform feedback
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, m_nParticleCount);
    glEndTransformFeedback();

    // Since the rendering relies on the particle update being finished,
    // and the gpu does its processing asynchronously, we must wait until
    // the gpu processing is done by using a sync object. glFinish should
    // work too, but might not be as effectient.
    m_pSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    glDisable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_timer.Start();
}

//*****************************************************************************
// Render
//*****************************************************************************
void ParticleSystem::Render(Matrix* pMatrixVP)
{
    int hProg = GetShaderProgram(PARTICLE_RENDER_PROGRAM);

    glWaitSync((GLsync)m_pSync, 0, GL_TIMEOUT_IGNORED);
    glDeleteSync((GLsync)m_pSync);

    glUseProgram(hProg);
    glBindBuffer(GL_ARRAY_BUFFER, m_arVBOs[!m_nUpdateBuffer]);

    // Assign vertex attributes
    glVertexAttribPointer(s_hRendPosition,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          0);
    glVertexAttribPointer(s_hRendColor,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 24);
    glVertexAttribPointer(s_hRendLife,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 40);
    glVertexAttribPointer(s_hRendSize,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          PARTICLE_DATA_SIZE,
                          (void*) 44);

    // Enable the attrib arrays
    glEnableVertexAttribArray(s_hRendPosition);
    glEnableVertexAttribArray(s_hRendColor);
    glEnableVertexAttribArray(s_hRendLife);
    glEnableVertexAttribArray(s_hRendSize);

    // Setup the uniforms
    glUniformMatrix4fv(s_hRendMatrixVP, 1, GL_FALSE, pMatrixVP->GetArray());
    if (m_pTexture == 0)
    {
        glUniform1i(s_hTexType, 0);
    }
    else
    {
        glUniform1i(s_hTexType, 1);
    }
    glUniform1i(s_hRendTexture, 0);

    // Bind texture
    if (m_pTexture != 0)
    {
        m_pTexture->Bind();
    }

    glDrawArrays(GL_POINTS, 0, m_nParticleCount);
}

//*****************************************************************************
// SetParticleCount
//*****************************************************************************
void ParticleSystem::SetParticleCount(int nParticleCount)
{
    if (nParticleCount > 0 &&
        nParticleCount <= MAX_PARTICLE_COUNT)
    {
        m_nParticleCount = nParticleCount;
    }
    else
    {
        LogError("Particle count cannot be set above engine maximum.");
    }
}

//*****************************************************************************
// SetEmissionRate
//*****************************************************************************
void ParticleSystem::SetEmissionRate(int nEmissionRate)
{
    if (nEmissionRate > 0)
    {
        m_nEmissionRate = nEmissionRate;
        m_fEmissionPeriod = 1.0f/m_nEmissionRate;
    } 
}

//*****************************************************************************
// SetLifetime
//*****************************************************************************
void ParticleSystem::SetLifetime(float fMinLifetime,
                                 float fMaxLifetime)
{
    m_fMinLifetime = fMinLifetime;
    m_fMaxLifetime = fMaxLifetime;
}

//*****************************************************************************
// SetColor
//*****************************************************************************
void ParticleSystem::SetColor(float arMinColor[4],
                              float arMaxColor[4])
{
    m_arMinColor[0] = arMinColor[0];
    m_arMinColor[1] = arMinColor[1];
    m_arMinColor[2] = arMinColor[2];
    m_arMinColor[3] = arMinColor[3];

    m_arMaxColor[0] = arMaxColor[0];
    m_arMaxColor[1] = arMaxColor[1];
    m_arMaxColor[2] = arMaxColor[2];
    m_arMaxColor[3] = arMaxColor[3];
}

//*****************************************************************************
// SetVelocity
//*****************************************************************************
void ParticleSystem::SetVelocity(float arMinVelocity[3],
                                 float arMaxVelocity[3])
{
    m_arMinVelocity[0] = arMinVelocity[0];
    m_arMinVelocity[1] = arMinVelocity[1];
    m_arMinVelocity[2] = arMinVelocity[2];

    m_arMaxVelocity[0] = arMaxVelocity[0];
    m_arMaxVelocity[1] = arMaxVelocity[1];
    m_arMaxVelocity[2] = arMaxVelocity[2];
}

//*****************************************************************************
// SetSize
//*****************************************************************************
void ParticleSystem::SetSize(float fMinSize,
                             float fMaxSize)
{
    m_fMinSize = fMinSize;
    m_fMaxSize = fMaxSize;
}

//*****************************************************************************
// SetGravity
//*****************************************************************************
void ParticleSystem::SetGravity(float arGravity[3])
{
    m_arGravity[0] = arGravity[0];
    m_arGravity[1] = arGravity[1];
    m_arGravity[2] = arGravity[2];
}

//*****************************************************************************
// SetOrigin
//*****************************************************************************
void ParticleSystem::SetOrigin(float fX,
               float fY,
               float fZ)
{
    m_arOrigin[0] = fX;
    m_arOrigin[1] = fY;
    m_arOrigin[2] = fZ;
}

//*****************************************************************************
// SetSpawnVariance
//*****************************************************************************
void ParticleSystem::SetSpawnVariance(float fX,
                                      float fY,
                                      float fZ)
{
    m_arSpawnVariance[0] = fX;
    m_arSpawnVariance[1] = fY;
    m_arSpawnVariance[2] = fZ;
}

//*****************************************************************************
// SetGenerate
//*****************************************************************************
void ParticleSystem::SetGenerate(int nGenerate)
{
    m_nGenerate = nGenerate;
}

//*****************************************************************************
// SetTexture
//*****************************************************************************
void ParticleSystem::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}
