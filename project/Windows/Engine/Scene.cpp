#include "Scene.h"
#include "VGL.h"
#include "Log.h"
#include "VInput.h"
#include "Settings.h"

#define DEFAULT_AMBIENT_RED   0.12f
#define DEFAULT_AMBIENT_GREEN 0.12f
#define DEFAULT_AMBIENT_BLUE  0.12f
#define DEFAULT_AMBIENT_ALPHA 0.12f

#define DEFAULT_GRAVITY 9.8f

int Scene::s_nEffectProcessing = 0;
unsigned int Scene::s_hFBO          = 0;
unsigned int Scene::s_hColorAttach  = 0;
unsigned int Scene::s_hDepthAttach  = 0;

//*****************************************************************************
// Constructor
//*****************************************************************************
Scene::Scene(int nMaxMatters,
             int nMaxLights,
             int nMaxGlyphs,
             int nMaxEffects)
{
    m_nMaxMatters = nMaxMatters;
    m_nMaxLights  = nMaxLights;
    m_nMaxGlyphs  = nMaxGlyphs;
    m_nMaxEffects = nMaxEffects;

    // When first initialized, there are no objects in the scene
    m_nNumMatters = 0;
    m_nNumLights  = 0;
    m_nNumGlyphs  = 0;
    m_nNumEffects = 0;

    // Allocate the arrays to hold scene components
    m_pMatters = new Matter*[m_nMaxMatters];
    m_pLights  = new Light*[m_nMaxLights];
    m_pGlyphs  = new Glyph*[m_nMaxGlyphs];
    m_pEffects = new Effect*[m_nMaxEffects];

    if (m_pMatters == 0 ||
        m_pLights  == 0 ||
        m_pGlyphs  == 0 ||
        m_pEffects == 0)
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
    Matrix matMVP;

    // Bind FBO if scene has effects
    if (m_nNumEffects        > 0 &&
        s_nEffectProcessing != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, s_hFBO);
    }
    
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
            glDisable(GL_BLEND);
        }
    }

    //@@ Possibly render code to a RenderGlyphs() private helper function.
    // Render screen glyphs last after 3D scene has rendered
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (i = 0; i < m_nNumGlyphs; i++)
    {
        m_pGlyphs[i]->Render();
    }

    // Render effects if they are enabled
    if (m_nNumEffects        > 0 &&
        s_nEffectProcessing != 0)
    {
        // Render to screen buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        for (i = 0; i < m_nNumEffects; i++)
        {
            if (m_pEffects[i]->IsEnabled() != 0)
            {
                m_pEffects[i]->Render(this,
                                      s_hFBO,
                                      s_hColorAttach,
                                      s_hDepthAttach);
            }
        }
    }

    // If the soft keyboard is enabled, render it
    if (IsSoftKeyboardEnabled() != 0)
    {
        RenderSoftKeyboard();
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);


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

            if (s_nEffectProcessing == 0)
            {
                InitializeEffectProcessing();
            }
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
void Scene::InitializeEffectProcessing()
{
    int nStatus = 0;

    // First create the framebuffer to work with
    glGenFramebuffers(1, &s_hFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, s_hFBO);

    // Next create the color attachment (Texture)
    glGenTextures(1, &s_hColorAttach);
    glBindTexture(GL_TEXTURE_2D, s_hColorAttach);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 g_nScreenWidth,
                 g_nScreenHeight,
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
                           s_hColorAttach,
                           0);

    // The depth attachment will be attached next.
    // A Renderbuffer will be used for the depth attachment.
    glGenRenderbuffers(1, &s_hDepthAttach);
    glBindRenderbuffer(GL_RENDERBUFFER, s_hDepthAttach);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH24_STENCIL8,
                          g_nScreenWidth,
                          g_nScreenHeight);

    // Attach it to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER,
                              s_hDepthAttach);

    // Lastly, check if the FBO is ready to be used.
    nStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (nStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        LogError("Framebuffer is not complete.");
    }

    s_nEffectProcessing = 1;
}
