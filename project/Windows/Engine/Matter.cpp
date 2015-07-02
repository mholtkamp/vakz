#include "Matter.h"
#include "Log.h"
#include "VGL.h"
#include "Camera.h"
#include "Scene.h"

#define MATTER_MAX_POINT_LIGHTS 4
#define MATTER_MAX_DIRECTIONAL_LIGHTS 1

Matter::Matter()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;

    m_fRotX = 0.0f;
    m_fRotY = 0.0f;
    m_fRotZ = 0.0f;

    m_fScaleX = 1.0f;
    m_fScaleY = 1.0f;
    m_fScaleZ = 1.0f;

    m_pStaticMesh   = 0;
    m_pAnimatedMesh = 0;
    m_pSkeletalMesh = 0;
    
    m_pMaterial     = 0;
}

Matter::~Matter()
{
    
}

void Matter::SetStaticMesh(StaticMesh* pMesh)
{
    if (pMesh != 0)
    {
        // Set the new static mesh.
        m_pStaticMesh   = pMesh;

        // Clear any other types of meshes.
        m_pAnimatedMesh = 0;
        m_pSkeletalMesh = 0;
    }
    else
    {
        LogWarning("Denied attempt to set static mesh to null pointer in"
                   " Matter::SetStaticMesh().");
    }
}

void Matter::SetMaterial(Material* pMaterial)
{
    if (pMaterial != 0)
    {
        m_pMaterial = pMaterial;
    }
    else
    {
        LogWarning("Denied attempt to set null material in Matter::SetMaterial().");
    }
}

void Matter::Render(void* pScene)
{
    int          i           =  0;
    unsigned int hProg       =  0;
    int          hMatrixMVP  = -1;
    int          hMatrixM    = -1;
    Matrix*      pView       =  0;
    Matrix*      pProjection =  0;
    Matrix       matMVP;
    Camera*      pCamera     = reinterpret_cast<Scene*>(pScene)->GetCamera();
    Light**      pLights     = reinterpret_cast<Scene*>(pScene)->GetLightArray();
    int          nNumLights  = reinterpret_cast<Scene*>(pScene)->GetNumLights();

    // Generate model matrix
    // Note: May want to move this generation to the SetPosition()/
    // SetRotation()/SetScale() functions to reduce calculations.
    GenerateModelMatrix();
    
    if (m_pStaticMesh != 0 &&
        m_pMaterial   != 0 &&
        pCamera       != 0)
    {
        // Get the proper shader based on mesh type and material type
        // GetShader(MESH_STATIC, m_pMaterial->GetType());
        hProg = GetShaderProgram(STATIC_DIFFUSE_PROGRAM);
        
        // Start Rendering
        glUseProgram(hProg);
    
        // Set up uniforms/attributes
        m_pStaticMesh->SetRenderState(pScene, hProg);
        m_pMaterial->SetRenderState(pScene, hProg);

        // Set up lights
        for (i = 0; i < nNumLights; i++)
        {
            pLights[i]->SetRenderState(pScene,
                                       hProg,
                                       0);
        }

        // Get View and Projection matrices from camera
        pView       = reinterpret_cast<Camera*>(pCamera)->GetViewMatrix();
        pProjection = reinterpret_cast<Camera*>(pCamera)->GetProjectionMatrix();

        // Construct MVP matrix
        if (pView       != 0 &&
            pProjection != 0)
        {
            // MVP matrix = Projection * View * Model
            matMVP = (*pProjection) * (*pView) * m_matModel;
            hMatrixMVP = glGetUniformLocation(hProg, "uMatrixMVP");
            glUniformMatrix4fv(hMatrixMVP, 1, GL_FALSE, matMVP.GetArray());

            // Upload model matrix to shader for normal transformations
            hMatrixM = glGetUniformLocation(hProg, "uMatrixM");
            glUniformMatrix4fv(hMatrixM, 1, GL_FALSE, m_matModel.GetArray());
        }
        else
        {
            LogWarning("Could not retrieve view/projection matrix from camera in Matter::Render().");
            return;
        }
        
        glDrawArrays(GL_TRIANGLES,
                     0,
                     m_pStaticMesh->GetVertexCount());
    }


}

void Matter::GenerateModelMatrix()
{
    // Reset the matrix
    m_matModel.LoadIdentity();

    // Translate
    m_matModel.Translate(m_fX, m_fY, m_fZ);

    // Rotate
    m_matModel.Rotate(m_fRotX, 1.0f, 0.0f, 0.0f);
    m_matModel.Rotate(m_fRotY, 0.0f, 1.0f, 0.0f);
    m_matModel.Rotate(m_fRotZ, 0.0f, 0.0f, 1.0f);

    // Scale
    m_matModel.Scale(m_fScaleX, m_fScaleY, m_fScaleZ);
}

void Matter::SetPosition(float fX,
                         float fY,
                         float fZ)
{
    m_fX = fX;
    m_fY = fY;
    m_fZ = fZ;
}

void Matter::SetRotation(float fRotX,
                         float fRotY,
                         float fRotZ)
{
    m_fRotX = fRotX;
    m_fRotY = fRotY;
    m_fRotZ = fRotZ;
}

void Matter::SetScale(float fScaleX,
                      float fScaleY,
                      float fScaleZ)
{
    m_fScaleX = fScaleX;
    m_fScaleY = fScaleY;
    m_fScaleZ = fScaleZ;
}
