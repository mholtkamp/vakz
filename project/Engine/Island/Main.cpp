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

    Quad* pTestQuad = new Quad();
    pTestScene->AddGlyph(pTestQuad);
    
    pTestQuad->SetBox(200.0f,
                      300.0f,
                      100.0f,
                      200.0f);
    pTestQuad->SetColor(0.0f, 0.0f, 0.6f, 1.0f);

    while (1)
    {
        Render();

        printf("Rendering.\n");
    }

    delete pTestQuad;
    delete pTestScene;

    printf("Exiting.\n");
}