#include "Vakz.h"
#include "Scene.h"
#include "Quad.h"
#include "VGL.h"
#include "VInput.h"
#include "VMath.h"
#include "Log.h"
#include "Timer.h"
#include "Matrix.h"
#include "BoxCollider.h"
#include "OrientedBoxCollider.h"
#include "ResourceLibrary.h"
#include "DiffuseMaterial.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Text.h"
#include "RimlitMaterial.h"

#include <stdio.h>
#include <math.h>

#define ROT_SPEED 70.0f
#define MOVE_SPEED 15.0f
#define THRESH 0.4f

#define CONT_SPHERE_MOVE_RATIO 0.2f
#define SPHERE_COUNT 0
#define MOVING_SPHERE_COUNT 500
#define MOVING_SPHERE_SPEED 4.0f
#define SCENE_SIZE 100.0f
#define DISTRIB_SIZE 90.0f
#define SPHERE_MIN_SCALE 1.0f
#define SPHERE_MAX_SCALE 1.0f

int main()
{
    SetWindowSize(1024, 768);
    SetResolution(1024, 768);
    Initialize();

    Scene* pTestScene = new Scene();
    SetScene(pTestScene);

    // Create camera
    Camera* pCamera = new Camera();
    pCamera->SetProjectionType(Camera::CAMERA_PERSPECTIVE);
    pTestScene->SetCamera(pCamera);

    // Create diffuse material
    DiffuseMaterial* pWhiteMat = new DiffuseMaterial();
    pWhiteMat->SetColor(0.4f, 0.4f, 1.0f, 1.0f);
    DiffuseMaterial* pMovingSphereMat = new DiffuseMaterial();
    pMovingSphereMat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);


    // Sphere assets
    StaticMesh* pSphereMesh = new StaticMesh();
    pSphereMesh->Load("sphere.obj");

    BoxCollider* pSphereCollider = new BoxCollider();
    pSphereCollider->SetHalfExtents(1.0f, 1.0f, 1.0f);
    pSphereCollider->SetColor(0.2f, 0.1f, 8.0f, 0.3f);

    float arRand[3];
    float arCenter[3]  = {0.0f, 0.0f, 0.0f};
    float arExtents[3] = {SCENE_SIZE/2.0f,
                          SCENE_SIZE/2.0f,
                          SCENE_SIZE/2.0f};
      
    pTestScene->SetSceneBounds(arCenter, arExtents);

#if (SPHERE_COUNT > 0)  
    Matter* arSpheres[SPHERE_COUNT];

    for (int i = 0; i < SPHERE_COUNT; i++)
    {
        arSpheres[i] = new Matter();
        arSpheres[i]->SetMesh(pSphereMesh);
        arSpheres[i]->SetMaterial(pWhiteMat);
        arSpheres[i]->AddCollider(pSphereCollider);

        arSpheres[i]->SetPhysical(1);
        arSpheres[i]->SetSorted(1);
        arSpheres[i]->SetMobile(0);
        arSpheres[i]->EnableColliderRendering();

        arRand[0] = ((float) rand())/((float) RAND_MAX);
        arRand[1] = ((float) rand())/((float) RAND_MAX);
        arRand[2] = ((float) rand())/((float) RAND_MAX);

        arSpheres[i]->SetPosition(arRand[0] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f,
                                  arRand[1] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f,
                                  arRand[2] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f);

        arRand[0] = ((float) rand())/((float) RAND_MAX);

        arSpheres[i]->SetScale(arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE,
                               arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE,
                               arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE);

        pTestScene->AddMatter(arSpheres[i]);
    }

#endif

#if (MOVING_SPHERE_COUNT > 0)
    // Create matters
    Matter* arMovingSpheres[MOVING_SPHERE_COUNT];
    float arVelocities[MOVING_SPHERE_COUNT * 3]; 

    for (int i = 0; i < MOVING_SPHERE_COUNT; i++)
    {
        arMovingSpheres[i] = new Matter();
        arMovingSpheres[i]->SetMesh(pSphereMesh);
        arMovingSpheres[i]->SetMaterial(pMovingSphereMat);
        arMovingSpheres[i]->AddCollider(pSphereCollider);

        arMovingSpheres[i]->SetPhysical(0);
        arMovingSpheres[i]->SetSorted(1);
        arMovingSpheres[i]->SetMobile(1);
        //arMovingSpheres[i]->EnableColliderRendering();

        arRand[0] = ((float) rand())/((float) RAND_MAX);
        arRand[1] = ((float) rand())/((float) RAND_MAX);
        arRand[2] = ((float) rand())/((float) RAND_MAX);

        arMovingSpheres[i]->SetPosition(arRand[0] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f,
                                  arRand[1] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f,
                                  arRand[2] * DISTRIB_SIZE - DISTRIB_SIZE/2.0f);

        arRand[0] = ((float) rand())/((float) RAND_MAX);
        arRand[1] = ((float) rand())/((float) RAND_MAX);
        arRand[2] = ((float) rand())/((float) RAND_MAX);

        arVelocities[i*3 + 0] = arRand[0] * 2.0f - 1.0f;
        arVelocities[i*3 + 1] = arRand[1] * 2.0f - 1.0f;
        arVelocities[i*3 + 2] = arRand[2] * 2.0f - 1.0f;

        NormalizeVector(&arVelocities[i*3]);

        arRand[0] = ((float) rand())/((float) RAND_MAX);

        arMovingSpheres[i]->SetScale(arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE,
                               arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE,
                               arRand[0]*(SPHERE_MAX_SCALE - SPHERE_MIN_SCALE) + SPHERE_MIN_SCALE);

        pTestScene->AddActor(arMovingSpheres[i]);
    }
    
#endif

    pTestScene->EnableMatterOctreeRendering();

    // Create controllable sphere
    DiffuseMaterial* pContSphereMat = new DiffuseMaterial();
    pContSphereMat->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    Matter* pContSphere = new Matter();
    pContSphere->SetMesh(pSphereMesh);
    pContSphere->SetMaterial(pContSphereMat);
    pContSphere->AddCollider(pSphereCollider);
    pContSphere->SetPhysical(1);
    pContSphere->SetSorted(0);
    pContSphere->SetMobile(1);
    //pTestScene->AddActor(pContSphere);

    

    // Create sun
    DirectionalLight* pSun = new DirectionalLight();
    pSun->SetColor(1.0f, 1.0f, 1.0f);
    pSun->SetDirectionVector(3.0f, -1.0f, -2.0f);
    //pTestScene->AddActor(pSun);
    
    // Point light 1
    PointLight* pPointLight = new PointLight();
    pPointLight->SetColor(1.0f, 0.0f, 0.0f);
    pPointLight->SetPosition(0.0f, -50.0f, 0.0f);
    pPointLight->SetIntensity(100.0f);
    Box plBox;
    plBox.m_arCenter[0] = -50.0f;
    plBox.m_arCenter[1] = -50.0f;
    plBox.m_arCenter[2] = -50.0f;
    plBox.m_arExtent[0] = 100.0f;
    plBox.m_arExtent[1] = 100.0f;
    plBox.m_arExtent[2] = 100.0f;
    pPointLight->SetBox(plBox);
    pTestScene->AddActor(pPointLight);

    // Point light 2
    PointLight* pPointLight2 = new PointLight();
    pPointLight2->SetColor(0.0f, 0.0f, 1.0f);
    pPointLight2->SetPosition(0.0f, 50.0f, 0.0f);
    pPointLight2->SetIntensity(100.0f);
    Box pl2Box;
    pl2Box.m_arCenter[0] = 0.0f;
    pl2Box.m_arCenter[1] = 50.0f;
    pl2Box.m_arCenter[2] = 0.0f;
    pl2Box.m_arExtent[0] = 100.0f;
    pl2Box.m_arExtent[1] = 100.0f;
    pl2Box.m_arExtent[2] = 100.0f;
    pPointLight2->SetBox(pl2Box);
    pTestScene->AddActor(pPointLight2);

    // Point light 3
    PointLight* pPointLight3 = new PointLight();
    pPointLight3->SetColor(0.0f, 1.0f, 1.0f);
    pPointLight3->SetPosition(0.0f, 20.0f, 0.0f);
    pPointLight3->SetIntensity(10.0f);
    Box pl3Box;
    pl3Box.m_arCenter[0] = 0.0f;
    pl3Box.m_arCenter[1] = 20.0f;
    pl3Box.m_arCenter[2] = 0.0f;
    pl3Box.m_arExtent[0] = 10.0f;
    pl3Box.m_arExtent[1] = 10.0f;
    pl3Box.m_arExtent[2] = 10.0f;
    pPointLight3->SetBox(pl3Box);
    pTestScene->AddActor(pPointLight3);

    // Point light 4
    PointLight* pPointLight4 = new PointLight();
    pPointLight4->SetColor(0.0f, 1.0f, 1.0f);
    pPointLight4->SetPosition(-5.0f, 20.0f, 0.0f);
    pPointLight4->SetIntensity(10.0f);
    Box pl4Box;
    pl4Box.m_arCenter[0] = -5.0f;
    pl4Box.m_arCenter[1] = 20.0f;
    pl4Box.m_arCenter[2] = 0.0f;
    pl4Box.m_arExtent[0] = 10.0f;
    pl4Box.m_arExtent[1] = 10.0f;
    pl4Box.m_arExtent[2] = 10.0f;
    pPointLight4->SetBox(pl4Box);
    pTestScene->AddActor(pPointLight4);
    
    // Point light 5
    PointLight* pPointLight5 = new PointLight();
    pPointLight5->SetColor(0.0f, 1.0f, 1.0f);
    pPointLight5->SetPosition(5.0f, 20.0f, 0.0f);
    pPointLight5->SetIntensity(10.0f);
    Box pl5Box;
    pl5Box.m_arCenter[0] = 5.0f;
    pl5Box.m_arCenter[1] = 20.0f;
    pl5Box.m_arCenter[2] = 0.0f;
    pl5Box.m_arExtent[0] = 10.0f;
    pl5Box.m_arExtent[1] = 10.0f;
    pl5Box.m_arExtent[2] = 10.0f;
    pPointLight5->SetBox(pl5Box);
    pTestScene->AddActor(pPointLight5);


    float fX = 0.0f;
    float fY = 0.0f;
    float fZ = 0.0f;

    float fCamZ = 10.0f;
    float fCamY = 0.0f;
    float fCamX = 0.0f;

    float fSphereX = 0.0f;
    float fSphereY = 0.0f;
    float fSphereZ = 0.0f;

    float fRotX = 0.0f;
    float fRotY = 0.0f;
    float fRotZ = 0.0f;

    int nLock = 0;
    int nControlSphere = 0;

    float fSeconds = 0.0f;
    int nRenderCount = 0;
    Timer timer;
    Timer timerRender;
    timer.Start();

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        glFinish();

        timer.Stop();

        fSeconds = timer.Time();

        // Update
        Update();

        fX = 0.0f;
        fY = 0.0f;
        fZ = 0.0f;

        if (IsControllerConnected(0))
        {
            // Camera
            if (GetControllerAxisValue(VCONT_AXIS_RZ, 0) < -THRESH)
            {
                fRotX += fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_RZ, 0) > THRESH)
            {
                fRotX -= fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_Z, 0) > THRESH)
            {
                fRotY -= fSeconds * ROT_SPEED;
            }
            if (GetControllerAxisValue(VCONT_AXIS_Z, 0) < -THRESH)
            {
                fRotY += fSeconds * ROT_SPEED;
            }

            // Movement
            if (GetControllerAxisValue(VCONT_AXIS_Y, 0) < -THRESH)
            {
                fZ -=  fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
                fX -=  fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_Y, 0) > THRESH)
            {
                fZ += fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
                fX += fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_X, 0) < -THRESH)
            {
                fZ -= fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
                fX -= fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
            }
            if (GetControllerAxisValue(VCONT_AXIS_X, 0) > THRESH)
            {
                fZ += fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
                fX += fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
            }
            if (IsControllerButtonDown(VCONT_R1, 0))
            {
                fY += fSeconds * MOVE_SPEED;
            }
            if (IsControllerButtonDown(VCONT_L1, 0))
            {
                fY -= fSeconds * MOVE_SPEED;
            }
        }
        else
        {
            //LogDebug("Controller is not connected");
        }

        // Rotate camera
        if (IsKeyDown(VKEY_UP))
        {
            fRotX += fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_DOWN))
        {
            fRotX -= fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_RIGHT))
        {
            fRotY -= fSeconds * ROT_SPEED;
        }
        if (IsKeyDown(VKEY_LEFT))
        {
            fRotY += fSeconds * ROT_SPEED;
        }

        // Move
        if (IsKeyDown(VKEY_W))
        {
            fZ -= fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
            fX -= fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_S))
        {
            fZ += fSeconds * MOVE_SPEED * cos(fRotY * DEGREES_TO_RADIANS);
            fX += fSeconds * MOVE_SPEED * sin(fRotY * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_A))
        {
            fZ -= fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
            fX -= fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_D))
        {
            fZ += fSeconds * MOVE_SPEED * cos((90.0f + fRotY) * DEGREES_TO_RADIANS);
            fX += fSeconds * MOVE_SPEED * sin((90.0f + fRotY) * DEGREES_TO_RADIANS);
        }
        if (IsKeyDown(VKEY_SPACE))
        {
            fY += fSeconds * MOVE_SPEED;
        }
        if (IsKeyDown(VKEY_CONTROL))
        {
            fY -= fSeconds * MOVE_SPEED;
        }

        if (IsKeyDown(VKEY_M))
        {
            pTestScene->DisableMatterOctreeRendering();
        }

        if (IsKeyDown(VKEY_B))
        {
            nControlSphere = !nControlSphere;
        }

        if (nControlSphere == 0)
        {
            fCamX += fX;
            fCamY += fY;
            fCamZ += fZ;
        }
        else
        {
            pContSphere->Translate(fX * CONT_SPHERE_MOVE_RATIO,
                                   fY * CONT_SPHERE_MOVE_RATIO,
                                   fZ * CONT_SPHERE_MOVE_RATIO);
        }
        
        pCamera->SetPosition(fCamX, fCamY, fCamZ);
        pCamera->SetRotation(fRotX, fRotY, fRotZ);

#if (MOVING_SPHERE_COUNT > 0)
        for (int i = 0; i < MOVING_SPHERE_COUNT; i++)
        {
            arMovingSpheres[i]->Translate(arVelocities[i*3 + 0] * MOVING_SPHERE_SPEED * fSeconds,
                                          arVelocities[i*3 + 1] * MOVING_SPHERE_SPEED * fSeconds,
                                          arVelocities[i*3 + 2] * MOVING_SPHERE_SPEED * fSeconds);

            const float* pPosition = arMovingSpheres[i]->GetPosition();
            float arNewPos[3];
            arNewPos[0] = pPosition[0];
            arNewPos[1] = pPosition[1];
            arNewPos[2] = pPosition[2];

            // Test mins
            if (arNewPos[0] < -SCENE_SIZE/2.0f)
            {
                arNewPos[0] = -SCENE_SIZE/2.0f;
                arVelocities[i*3 + 0] *= -1.0f;
            }
            if (arNewPos[1] < -SCENE_SIZE/2.0f)
            {
                arNewPos[1] = -SCENE_SIZE/2.0f;
                arVelocities[i*3 + 1] *= -1.0f;
            }
            if (arNewPos[2] < -SCENE_SIZE/2.0f)
            {
                arNewPos[2] = -SCENE_SIZE/2.0f;
                arVelocities[i*3 + 2] *= -1.0f;
            }

            // Test maximums
            if (arNewPos[0] > SCENE_SIZE/2.0f)
            {
                arNewPos[0] = SCENE_SIZE/2.0f;
                arVelocities[i*3 + 0] *= -1.0f;
            }
            if (arNewPos[1] > SCENE_SIZE/2.0f)
            {
                arNewPos[1] = SCENE_SIZE/2.0f;
                arVelocities[i*3 + 1] *= -1.0f;
            }
            if (arNewPos[2] > SCENE_SIZE/2.0f)
            {
                arNewPos[2] = SCENE_SIZE/2.0f;
                arVelocities[i*3 + 2] *= -1.0f;
            }
                
            // Check collisions with other spheres
            List lTests;
            pTestScene->GetNearbyMatter(arMovingSpheres[i], lTests);

            ListNode* pNode = lTests.GetHead();

            while (pNode != 0)
            {
                Matter* pOther = reinterpret_cast<Matter*>(pNode->m_pData);
                if (pOther != arMovingSpheres[i] &&
                    arMovingSpheres[i]->Overlaps(pOther))
                {
                    // Have velocity point outwards from collision
                    float arDiff[3];
                    arDiff[0] = arMovingSpheres[i]->GetPosition()[0] - pOther->GetPosition()[0];
                    arDiff[1] = arMovingSpheres[i]->GetPosition()[1] - pOther->GetPosition()[1];
                    arDiff[2] = arMovingSpheres[i]->GetPosition()[2] - pOther->GetPosition()[2];

                    NormalizeVector(arDiff);

                    arVelocities[i*3 + 0] = arDiff[0];
                    arVelocities[i*3 + 1] = arDiff[1];
                    arVelocities[i*3 + 2] = arDiff[2];
                }

                pNode = pNode->m_pNext;
            }
        }
#endif

        timer.Start();
        if (nRenderCount == 0)
        {
            timerRender.Start();
        }

        Render();
        nRenderCount++;

        if(nRenderCount == 50)
        {
            char arMsg[32] = {0};
            timerRender.Stop();
            nRenderCount = 0;
            float fRenderTime = timerRender.Time()/50.0f;
            sprintf(arMsg, "Render Time: %f", fRenderTime);
            LogDebug(arMsg);
        }
    }

    exit(0);
}
