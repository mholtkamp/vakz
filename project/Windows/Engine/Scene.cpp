#include "Scene.h"
#include "VGL.h"
#include "Log.h"
#include "VInput.h"
#include "Settings.h"
#include "Toast.h"
#include "Quad.h"

#define DEFAULT_AMBIENT_RED   0.12f
#define DEFAULT_AMBIENT_GREEN 0.12f
#define DEFAULT_AMBIENT_BLUE  0.12f
#define DEFAULT_AMBIENT_ALPHA 0.12f

#define DEFAULT_GRAVITY 9.8f

int Scene::s_nFBOInitialized = 0;
unsigned int Scene::s_hFBO   = 0;
Texture Scene::s_texColorAttach;
Texture Scene::s_texDepthAttach;
Texture Scene::s_texEffectColorAttach;

//*****************************************************************************
// Constructor
//*****************************************************************************
Scene::Scene(int nMaxMatters,
             int nMaxLights,
             int nMaxGlyphs,
             int nMaxEffects,
             int nMaxParticles)
{
    m_nMaxMatters   = nMaxMatters;
    m_nMaxLights    = nMaxLights;
    m_nMaxGlyphs    = nMaxGlyphs;
    m_nMaxEffects   = nMaxEffects;
    m_nMaxParticles = nMaxParticles;

    // When first initialized, there are no objects in the scene
    m_nNumMatters   = 0;
    m_nNumLights    = 0;
    m_nNumGlyphs    = 0;
    m_nNumEffects   = 0;
    m_nNumParticles = 0;

    // Allocate the arrays to hold scene components
    m_pMatters = new Matter*[m_nMaxMatters];
    m_pLights  = new Light*[m_nMaxLights];
    m_pGlyphs  = new Glyph*[m_nMaxGlyphs];
    m_pEffects = new Effect*[m_nMaxEffects];
    m_pParticleSystems = new ParticleSystem*[m_nMaxParticles];

    if (m_pMatters == 0 ||
        m_pLights  == 0 ||
        m_pGlyphs  == 0 ||
        m_pEffects == 0 ||
        m_pParticleSystems == 0)
    {
        LogError("Memory allocation error in Scene Constructor");
    }

    m_pCamera           = 0;
    m_pDirectionalLight = 0;

    m_arAmbientColor[0] = DEFAULT_AMBIENT_RED;
    m_arAmbientColor[1] = DEFAULT_AMBIENT_GREEN;
    m_arAmbientColor[2] = DEFAULT_AMBIENT_BLUE;
    m_arAmbientColor[3] = DEFAULT_AMBIENT_ALPHA;

    m_fGravity = DEFAULT_GRAVITY;

    m_pPhysTimer = 0;

    InitializeFBO();
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Scene::~Scene()
{
    // It is assumed that all scene components will be freed by the user.
    // If the user would like the scene to free the memory components it 
    // contains, call Scene::Destroy().

    if (m_pMatters != 0)
    {
        delete [] m_pMatters;
        m_pMatters = 0;
    }

    if (m_pLights != 0)
    {
        delete [] m_pLights;
        m_pLights = 0;
    }

    if (m_pGlyphs != 0)
    {
        delete [] m_pGlyphs;
        m_pGlyphs = 0;
    }

    if (m_pEffects != 0)
    {
        delete [] m_pEffects;
        m_pEffects = 0;
    }

    if (m_pPhysTimer != 0)
    {
        delete m_pPhysTimer;
        m_pPhysTimer = 0;
    }
}

//*****************************************************************************
// Render()
//*****************************************************************************
void Scene::Render()
{
    int i = 0;
    int nEffectOn = 0;
    Matrix matVP;
    Matrix* pProj = 0;
    Matrix* pView = 0;
    Matrix matMVP;
    Quad quadScreen;

    quadScreen.SetPosition(-1.0f, -1.0f);
    quadScreen.SetDimensions(2.0f, 2.0f);

    // Bind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, s_hFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D,
                           s_texColorAttach.GetHandle(),
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Adjust the viewport to match the FBO size
    glViewport(0,0, g_nResolutionX, g_nResolutionY);

    // Render 3D Objects
    if (m_pCamera != 0)
    {
        m_pCamera->GenerateViewMatrix();
        m_pCamera->GenerateProjectionMatrix();

        // Render matter
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        for (i = 0; i < m_nNumMatters; i++)
        {
            m_pMatters[i]->Render(this);
        }

        glDisable(GL_CULL_FACE);

        // Render colliders for debugging if enabled
        for (i = 0; i < m_nNumMatters; i++)
        {
            // Setup state for rendering translucent collision meshes
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            if (m_pMatters[i]->GetCollider()                       != 0 && 
                m_pMatters[i]->GetCollider()->IsRenderingEnabled() != 0)
            {
                GenerateMVPMatrix(m_pMatters[i]->GetModelMatrix(),
                                  m_pCamera->GetViewMatrix(),
                                  m_pCamera->GetProjectionMatrix(),
                                  &matMVP);
                m_pMatters[i]->GetCollider()->Render(&matMVP);
            }

            // Restore state
            glDepthMask(GL_TRUE);
            glDisable(GL_DEPTH_TEST);
        }

        // Render particle systems
        for (i = 0; i < m_nNumParticles; i++)
        {
            pProj = m_pCamera->GetProjectionMatrix();
            pView = m_pCamera->GetViewMatrix();

            // Multiply projection/view matrices.
            matVP = (*pProj) * (*pView);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            m_pParticleSystems[i]->Update();
            m_pParticleSystems[i]->Render(&matVP);

            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }
    }

    //@@ Possibly render code to a RenderGlyphs() private helper function.
    // Render screen glyphs last after 3D scene has rendered
    glDisable(GL_DEPTH_TEST);
    for (i = 0; i < m_nNumGlyphs; i++)
    {
        m_pGlyphs[i]->Render();
    }

    // Render effects if they are enabled
    if (m_nNumEffects        > 0)
    {
        // Render separate texture
        nEffectOn = 0;
        for (i = 0; i < m_nNumEffects; i++)
        {
            if (m_pEffects[i]->IsEnabled())
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                        GL_COLOR_ATTACHMENT0,
                                        GL_TEXTURE_2D,
                                        s_texEffectColorAttach.GetHandle(),
                                        0);
                nEffectOn = 1;
                break;
            }
        }

        for (i = 0; i < m_nNumEffects; i++)
        {
            if (m_pEffects[i]->IsEnabled() != 0)
            {
                m_pEffects[i]->Render(this,
                                      s_hFBO,
                                      s_texColorAttach.GetHandle(),
                                      s_texDepthAttach.GetHandle());
            }
        }
    }

    // Render toasts, if there are no active toasts, this function
    // will not render anything.
    RenderToast();

    // If the soft keyboard is enabled, render it
    if (IsSoftKeyboardEnabled() != 0)
    {
        RenderSoftKeyboard();
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    // Now render a full-screen quad to the screen.
    // Note: glBlitFramebuffer was previously used but the result
    // would be flipped on certain android devices.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Change viewport to draw on entire screen
    glViewport(0,0, g_nScreenWidth, g_nScreenHeight);

    if (nEffectOn != 0)
    {
        quadScreen.SetTexture(&s_texEffectColorAttach);
    }
    else
    {
        quadScreen.SetTexture(&s_texColorAttach);
    }
    quadScreen.Render();
    


    //@@ Old
    //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, s_hFBO);

    //glBlitFramebuffer(0,
    //                  0,
    //                  g_nResolutionX,
    //                  g_nResolutionY,
    //                  0,
    //                  0,
    //                  g_nScreenWidth,
    //                  g_nScreenHeight,
    //                  GL_COLOR_BUFFER_BIT,
    //                  GL_NEAREST);
}

//*****************************************************************************
// AddMatter
//*****************************************************************************
void Scene::AddMatter(Matter* pMatter)
{
    if (pMatter != 0)
    {
        if (m_nNumMatters < m_nMaxMatters)
        {
            m_pMatters[m_nNumMatters] = pMatter;
            m_nNumMatters++;
        }
        else
        {
            LogError("Failed to add matter to scene because maximum number of "
                     "matters has been reached.");
        }
    }
    else
    {
        LogWarning("Denied attempt to add null matter to scene.");
    }
}

//*****************************************************************************
// AddLight
//*****************************************************************************
void Scene::AddLight(Light* pLight)
{
    if (pLight != 0)
    {
        if (pLight->GetType() == LIGHT_DIRECTIONAL)
        {
            m_pDirectionalLight = reinterpret_cast<DirectionalLight*>(pLight);
        }
        else if (m_nNumLights < m_nMaxLights)
        {
            m_pLights[m_nNumLights] = pLight;
            m_nNumLights++;
        }
        else
        {
            LogError("Failed to add light to scene because maximum number of "
                     "lights has been reached.");
        }
    }
    else
    {
        LogWarning("Denied attempt to add null light to scene.");
    }
}

//*****************************************************************************
// AddGlyph
//*****************************************************************************
void Scene::AddGlyph(Glyph* pGlyph)
{
    if (pGlyph != 0)
    {
        if (m_nNumGlyphs < m_nMaxGlyphs)
        {
            m_pGlyphs[m_nNumGlyphs] = pGlyph;
            m_nNumGlyphs++;
        }
        else
        {
            LogError("Failed to add glyph to scene because maximum number of "
                     "glyphs has been reached.");
        }
    }
    else
    {
        LogWarning("Denied attempt to add null glyph to scene.");
    }
}

//*****************************************************************************
// AddEffect
//*****************************************************************************
void Scene::AddEffect(Effect* pEffect)
{
    if (pEffect != 0)
    {
        if (m_nNumEffects < m_nMaxEffects)
        {
            m_pEffects[m_nNumEffects] = pEffect;
            m_nNumEffects++;
        }
        else
        {
            LogError("Failed to add effect to scene because maximum number of "
                     "effects has been reached.");
        }
    }
    else
    {
        LogWarning("Denied attempt to add null effect to scene.");
    }
}

//*****************************************************************************
// AddParticleSystem
//*****************************************************************************
void Scene::AddParticleSystem(ParticleSystem* pParticleSystem)
{
    if (pParticleSystem != 0)
    {
        if (m_nNumParticles < m_nMaxParticles)
        {
            m_pParticleSystems[m_nNumParticles] = pParticleSystem;
            m_nNumParticles++;
        }
        else
        {
            LogError("Failed to add particle system to scene because maximum number of "
                     "particle systems has been reached.");
        }
    }
    else
    {
        LogWarning("Denied attempt to add null particle system to scene.");
    }
}

//*****************************************************************************
// SetCamera
//*****************************************************************************
void Scene::SetCamera(Camera* pCamera)
{
    if (pCamera != 0)
    {
        m_pCamera = pCamera;
    }
    else
    {
        LogWarning("Denied attempt to set scene camera to null pointer.");
    }
}

//*****************************************************************************
// GetCamera
//*****************************************************************************
Camera* Scene::GetCamera()
{
    return m_pCamera;
}

//*****************************************************************************
// GetLightArray
//*****************************************************************************
Light** Scene::GetLightArray()
{
    return m_pLights;
}

//*****************************************************************************
// GetDirectionalLight
//*****************************************************************************
DirectionalLight* Scene::GetDirectionalLight()
{
    return m_pDirectionalLight;
}

//*****************************************************************************
// GetNumLights
//*****************************************************************************
int Scene::GetNumLights()
{
    return m_nNumLights;
}

//*****************************************************************************
// SetAmbientLight
//*****************************************************************************
void Scene::SetAmbientLight(float fRed,
                            float fGreen,
                            float fBlue,
                            float fAlpha)
{
    m_arAmbientColor[0] = fRed;
    m_arAmbientColor[1] = fGreen;
    m_arAmbientColor[2] = fBlue;
    m_arAmbientColor[3] = fAlpha;
}

//*****************************************************************************
// GetAmbientLight
//*****************************************************************************
float* Scene::GetAmbientLight()
{
    return m_arAmbientColor;
}

//*****************************************************************************
// GenerateMVPMatrix
//*****************************************************************************
void Scene::GenerateMVPMatrix(Matrix* pModel,
                              Matrix* pView,
                              Matrix* pProjection,
                              Matrix* pMVP)
{
    *pMVP = (*pProjection) * (*pView) * (*pModel);
}

//*****************************************************************************
// SetGravity
//*****************************************************************************
void Scene::SetGravity(float fGravity)
{
    m_fGravity = fGravity;
}

//*****************************************************************************
// GetGravity
//*****************************************************************************
float Scene::GetGravity()
{
    return m_fGravity;
}

//*****************************************************************************
// Update
//*****************************************************************************
void Scene::Update()
{
    int   i     = 0;
    float fTime = 0.0f;

    // If this is the first time hitting update, create the timer.
    if (m_pPhysTimer == 0)
    {
        m_pPhysTimer = new Timer();
        m_pPhysTimer->Start();
    }
    
    // Get the frame time
    m_pPhysTimer->Stop();
    fTime = m_pPhysTimer->Time();

    // Update physics
    for (i = 0; i < m_nNumMatters; i++)
    {
        m_pMatters[i]->UpdatePhysics(this, fTime);
    }

    // Reset timer for next frame calculation
    m_pPhysTimer->Start();
}

//*****************************************************************************
// InitializeEffectProcessing
//*****************************************************************************
void Scene::InitializeFBO()
{
    int nStatus        = 0;
    unsigned int hTemp = 0;

    if (s_nFBOInitialized == 0)
    {
        // First create the framebuffer to work with
        glGenFramebuffers(1, &s_hFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, s_hFBO);

        // Next create the color attachment (Texture)
        glGenTextures(1, &hTemp);
        s_texColorAttach.SetHandle(hTemp);
        glBindTexture(GL_TEXTURE_2D, hTemp);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     g_nResolutionX,
                     g_nResolutionY,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Attach the color attachment texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               hTemp,
                               0);

        // The depth attachment will be attached next.
        // A Renderbuffer will be used for the depth attachment.
        glGenRenderbuffers(1, &hTemp);
        s_texDepthAttach.SetHandle(hTemp);
        glBindRenderbuffer(GL_RENDERBUFFER, hTemp);
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH24_STENCIL8,
                              g_nResolutionX,
                              g_nResolutionY);

        // Attach it to the FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  hTemp);

        // Lastly, check if the FBO is ready to be used.
        nStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (nStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            LogError("Framebuffer is not complete.");
        }

        // Set the new drawing viewport
        glViewport(0,0, g_nResolutionX, g_nResolutionY);

        // Create effect texture
        glGenTextures(1, &hTemp);
        s_texEffectColorAttach.SetHandle(hTemp);
        glBindTexture(GL_TEXTURE_2D, hTemp);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     g_nResolutionX,
                     g_nResolutionY,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        s_nFBOInitialized = 1;
    }
}

//*****************************************************************************
// RemoveMatter
//*****************************************************************************
void Scene::RemoveMatter(Matter* pMatter)
{
    int i      = 0;
    int nFound = 0;

    if (pMatter == 0)
    {
        return;
    }

    for (i = 0; i < m_nNumMatters; i++)
    {
        if (m_pMatters[i] == pMatter)
        {
            m_pMatters[i] = 0;
            nFound = 1;
            i++;
            break;
        }
    }

    if (nFound != 0)
    {
        for (; i < m_nNumMatters; i++)
        {
            m_pMatters[i-1] = m_pMatters[i];
        }

        m_pMatters[m_nNumMatters - 1] = 0;
        m_nNumMatters--;
    }
}

//*****************************************************************************
// RemoveLight
//*****************************************************************************
void Scene::RemoveLight(Light* pLight)
{
    int i      = 0;
    int nFound = 0;

    if (pLight == 0)
    {
        return;
    }

    for (i = 0; i < m_nNumLights; i++)
    {
        if (m_pLights[i] == pLight)
        {
            m_pLights[i] = 0;
            nFound = 1;
            i++;
            break;
        }
    }

    if (nFound != 0)
    {
        for (; i < m_nNumLights; i++)
        {
            m_pLights[i-1] = m_pLights[i];
        }

        m_pLights[m_nNumLights - 1] = 0;
        m_nNumLights--;
    }
}

//*****************************************************************************
// RemoveGlyph
//*****************************************************************************
void Scene::RemoveGlyph(Glyph* pGlyph)
{
    int i      = 0;
    int nFound = 0;

    if (pGlyph == 0)
    {
        return;
    }

    for (i = 0; i < m_nNumGlyphs; i++)
    {
        if (m_pGlyphs[i] == pGlyph)
        {
            m_pGlyphs[i] = 0;
            nFound = 1;
            i++;
            break;
        }
    }

    if (nFound != 0)
    {
        for (; i < m_nNumGlyphs; i++)
        {
            m_pGlyphs[i-1] = m_pGlyphs[i];
        }

        m_pGlyphs[m_nNumGlyphs - 1] = 0;
        m_nNumGlyphs--;
    }
}

//*****************************************************************************
// RemoveEffect
//*****************************************************************************
void Scene::RemoveEffect(Effect* pEffect)
{
    int i      = 0;
    int nFound = 0;

    if (pEffect == 0)
    {
        return;
    }

    for (i = 0; i < m_nNumEffects; i++)
    {
        if (m_pEffects[i] == pEffect)
        {
            m_pEffects[i] = 0;
            nFound = 1;
            i++;
            break;
        }
    }

    if (nFound != 0)
    {
        for (; i < m_nNumEffects; i++)
        {
            m_pEffects[i-1] = m_pEffects[i];
        }

        m_pEffects[m_nNumEffects - 1] = 0;
        m_nNumEffects--;
    }
}

//*****************************************************************************
// RemoveParticleSystem
//*****************************************************************************
void Scene::RemoveParticleSystem(ParticleSystem* pParticleSystem)
{
    int i      = 0;
    int nFound = 0;

    if (pParticleSystem == 0)
    {
        return;
    }

    for (i = 0; i < m_nNumParticles; i++)
    {
        if (m_pParticleSystems[i] == pParticleSystem)
        {
            m_pParticleSystems[i] = 0;
            nFound = 1;
            i++;
            break;
        }
    }

    if (nFound != 0)
    {
        for (; i < m_nNumParticles; i++)
        {
            m_pParticleSystems[i-1] = m_pParticleSystems[i];
        }

        m_pParticleSystems[m_nNumParticles - 1] = 0;
        m_nNumParticles--;
    }
}
