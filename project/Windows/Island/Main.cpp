#include "Vakz.h"
#include "Quad.h"
#include "VGL.h"

#include <stdio.h>

int main()
{
    SetWindowSize(1024,768);
    Initialize();
    Scene* pTestScene = new Scene();
    SetScene(pTestScene);

    // Force a warning to occur.
    Camera* pCamera = 0;
    pTestScene->SetCamera(pCamera);

    Quad* pTestQuad = new Quad();
    pTestScene->AddGlyph(pTestQuad);
    
    pTestQuad->SetBox(100.0f,
                      100.0f,
                      100.0f,
                      200.0f);
    pTestQuad->SetColor(0.0f, 0.0f, 0.6f, 1.0f);

    while (1)
    {
        Render();
    }

    delete pTestQuad;
    delete pTestScene;

    printf("Exiting.\n");
}