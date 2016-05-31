#include "Scene.h"
#include "VGL.h"
#include "Log.h"
#include "VInput.h"
#include "Settings.h"
#include "Toast.h"
#include "Quad.h"
#include "PointLight.h"

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

static float s_arOctreeColor[4] = {0.9f, 0.2f, 0.4f, 0.5f};

//*****************************************************************************
// Constructor
//*****************************************************************************
Scene::Scene()
{

    m_pCamera           = 0;
    m_pDirectionalLight = 0;

    m_arAmbientColor[0] = DEFAULT_AMBIENT_RED;
    m_arAmbientColor[1] = DEFAULT_AMBIENT_GREEN;
    m_arAmbientColor[2] = DEFAULT_AMBIENT_BLUE;
    m_arAmbientColor[3] = DEFAULT_AMBIENT_ALPHA;

    InitializeFBO();

    // Create and initialize default octrees
    m_pMatterOctree = new Octree();
    m_pPointLightOctree = new Octree();

    Box boxDefault;
    boxDefault.m_arCenter[0] = 0.0f;
    boxDefault.m_arCenter[1] = 0.0f;
    boxDefault.m_arCenter[2] = 0.0f;
    boxDefault.m_arExtent[0] = 50.0f;
    boxDefault.m_arExtent[1] = 50.0f;
    boxDefault.m_arExtent[2] = 50.0f;

    m_pMatterOctree->Initialize(boxDefault);
    m_pPointLightOctree->Initialize(boxDefault);

    m_nRenderMatterOctree = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Scene::~Scene()
{
    if (m_pMatterOctree != 0)
    {
        delete m_pMatterOctree;
        m_pMatterOctree = 0;
    }

    if (m_pPointLightOctree != 0)
    {
        delete m_pPointLightOctree;
        m_pPointLightOctree = 0;
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
    ListNode* pNode = 0;

    Matter* pMatter           = 0;
    ParticleSystem* pParticle = 0;
    Effect* pEffect           = 0;

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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        pNode = m_lMatters.GetHead();
        while (pNode != 0)
        {
            reinterpret_cast<Matter*>(pNode->m_pData)->Render(this);
            pNode = pNode->m_pNext;
        }

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        // Render particle systems
        pNode = m_lParticleSystems.GetHead();
        while(pNode != 0)
        {
            pParticle = reinterpret_cast<ParticleSystem*>(pNode->m_pData);
            pNode = pNode->m_pNext;

            pProj = m_pCamera->GetProjectionMatrix();
            pView = m_pCamera->GetViewMatrix();

            // Multiply projection/view matrices.
            matVP = (*pProj) * (*pView);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);

            pParticle->Update();
            pParticle->Render(&matVP);

            glDisable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }

        // Render the matter octree for debugging if enabled
        if (m_nRenderMatterOctree != 0)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glEnable(GL_DEPTH_TEST);

            m_pMatterOctree->Render(m_pCamera, s_arOctreeColor);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }
    }

    //@@ Possibly render code to a RenderGlyphs() private helper function.
    // Render screen glyphs last after 3D scene has rendered
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pNode = m_lGlyphs.GetHead();
    while (pNode != 0)
    {   
        reinterpret_cast<Glyph*>(pNode->m_pData)->Render();
        pNode = pNode->m_pNext;
    }

    // Render effects if they are enabled
    if (m_lEffects.Count() > 0)
    {
        // Render separate texture
        nEffectOn = 0;

        pNode = m_lEffects.GetHead();
        while(pNode != 0)
        {
            if (reinterpret_cast<Effect*>(pNode->m_pData)->IsEnabled())
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                        GL_COLOR_ATTACHMENT0,
                                        GL_TEXTURE_2D,
                                        s_texEffectColorAttach.GetHandle(),
                                        0);
                nEffectOn = 1;
                break;
            }

            pNode = pNode->m_pNext;
        }

        pNode = m_lEffects.GetHead();
        while(pNode != 0)
        {
            pEffect = reinterpret_cast<Effect*>(pNode->m_pData);

            if (pEffect->IsEnabled() != 0)
            {
                pEffect->Render(this,
                                s_hFBO,
                                s_texColorAttach.GetHandle(),
                                s_texDepthAttach.GetHandle());
            }

            pNode = pNode->m_pNext;
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

void Scene::AddActor(Actor* pActor)
{
    switch(pActor->GetClass())
    {
    case ACTOR_MATTER:
        AddMatter(reinterpret_cast<Matter*>(pActor));
        break;
    case ACTOR_PARTICLE:
        AddParticleSystem(reinterpret_cast<ParticleSystem*>(pActor));
        break;
    case ACTOR_LIGHT:
        AddLight(reinterpret_cast<Light*>(pActor));
        break;
    default:
        return;
    }
    
    pActor->SetScene(this);
}



//*****************************************************************************
// AddMatter
//*****************************************************************************
void Scene::AddMatter(Matter* pMatter)
{
    if (pMatter != 0)
    {
        m_lMatters.Add(pMatter);

        // Add object to octree if marked for sorting and currently has a collider
        if (pMatter->IsSorted() &&
            pMatter->GetColliderList()->Count() > 0)
        {
            m_pMatterOctree->Add(pMatter, *pMatter->GetBoundingBox());
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
        else if (pLight->GetType() == LIGHT_POINT)
        {
            PointLight* pPointLight = reinterpret_cast<PointLight*>(pLight);
            m_lLights.Add(pPointLight);
            m_pPointLightOctree->Add(pPointLight, *pPointLight->GetBox());
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
        m_lGlyphs.Add(pGlyph);
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
        m_lEffects.Add(pEffect);
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
        m_lParticleSystems.Add(pParticleSystem);
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
List* Scene::GetLightList()
{
    return &m_lLights;
}

//*****************************************************************************
// GetMatterList
//*****************************************************************************
 List* Scene::GetMatterList()
 {
    return &m_lMatters;
 }

//*****************************************************************************
// GetNumMatters
//*****************************************************************************
 int Scene::GetNumMatters()
 {
    return m_lMatters.Count();
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
    return m_lLights.Count();
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
// Update
//*****************************************************************************
void Scene::Update()
{

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
    m_lMatters.Remove(pMatter);
}

//*****************************************************************************
// RemoveLight
//*****************************************************************************
void Scene::RemoveLight(Light* pLight)
{
    m_lLights.Remove(pLight);
}

//*****************************************************************************
// RemoveGlyph
//*****************************************************************************
void Scene::RemoveGlyph(Glyph* pGlyph)
{
    m_lGlyphs.Remove(pGlyph);
}

//*****************************************************************************
// RemoveEffect
//*****************************************************************************
void Scene::RemoveEffect(Effect* pEffect)
{
    m_lEffects.Remove(pEffect);
}

//*****************************************************************************
// RemoveParticleSystem
//*****************************************************************************
void Scene::RemoveParticleSystem(ParticleSystem* pParticleSystem)
{
    m_lParticleSystems.Remove(pParticleSystem);
}

//*****************************************************************************
// SetSceneBounds
//*****************************************************************************
void Scene::SetSceneBounds(float* arCenter,
                           float* arExtent)
{
    Box boxScene;
    boxScene.m_arCenter[0] = arCenter[0];
    boxScene.m_arCenter[1] = arCenter[1];
    boxScene.m_arCenter[2] = arCenter[2];

    boxScene.m_arExtent[0] = arExtent[0];
    boxScene.m_arExtent[1] = arExtent[1];
    boxScene.m_arExtent[2] = arExtent[2];

    // Delete old octrees
    if (m_pMatterOctree != 0)
    {
        delete m_pMatterOctree;
        m_pMatterOctree = 0;
    }

    if (m_pPointLightOctree != 0)
    {
        delete m_pPointLightOctree;
        m_pPointLightOctree = 0;
    }

    // Remake new octrees
    m_pMatterOctree = new Octree();
    m_pPointLightOctree = new Octree();

    m_pMatterOctree->Initialize(boxScene);
    m_pPointLightOctree->Initialize(boxScene);

    // Fill the initialized octrees
    ListNode* pNode = m_lMatters.GetHead();
    Matter* pMatter = 0;

    while (pNode != 0)
    {
        pMatter = reinterpret_cast<Matter*>(pNode->m_pData);
        pNode = pNode->m_pNext;

        if (pMatter->IsSorted()                 != 0 &&
            pMatter->GetColliderList()->Count()  > 0)
        {
            m_pMatterOctree->Add(pMatter, *pMatter->GetBoundingBox());
        }
    }

    pNode = m_lLights.GetHead();
    PointLight* pLight = 0;

    while (pNode != 0)
    {
        pLight = reinterpret_cast<PointLight*>(pNode->m_pData);
        pNode = pNode->m_pNext;

        m_pPointLightOctree->Add(pLight, *pLight->GetBox());
    }
}

void Scene::EnableMatterOctreeRendering()
{
    m_nRenderMatterOctree = 1;
}

void Scene::DisableMatterOctreeRendering()
{
    m_nRenderMatterOctree = 0;
}

void Scene::GetNearbyMatter(Matter* pMatter, 
                            List& lMatter)
{
    if (pMatter != 0 &&
        pMatter->GetColliderList()->Count() > 0)
    {
        m_pMatterOctree->FindIntersectingObjects(*pMatter->GetBoundingBox(), lMatter);
    } 
}

void Scene::GetNearbyMatter(Box& bBounds, 
                            List& lMatter)
{
    m_pMatterOctree->FindIntersectingObjects(bBounds, lMatter);
}

void Scene::GetNearbyPointLights(Matter* pMatter,
                                 List& lPointLights)
{
    if (pMatter != 0 &&
        pMatter->GetColliderList()->Count() > 0)
    {
        m_pPointLightOctree->FindIntersectingObjects(*pMatter->GetBoundingBox(), lPointLights);
    }
}

void Scene::GetNearbyPointLights(Box& bBounds,
                                 List& lPointLights)
{
    m_pPointLightOctree->FindIntersectingObjects(bBounds, lPointLights);
}

Octree* Scene::GetMatterOctree()
{
    return m_pMatterOctree;
}
