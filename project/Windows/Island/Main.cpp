#include "Vakz.h"
#include "Quad.h"
#include "VGL.h"
#include "Matrix.h"

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

    // Matrix test
    Matrix matTest1;
    Matrix matTest2;
    Matrix matTest3;

    matTest1.LoadIdentity();
    matTest2.LoadIdentity();

    matTest1.Translate(3.0f, -2.0f, 8.0f);
    matTest1.Scale(2.0f, 1.5f, 1.2f);

    matTest2.Rotate(30, 0.0f, 1.0f, 0.0f);

    matTest3 = matTest1*matTest2;

    while (1)
    {
        Render();
    }

    delete pTestQuad;
    delete pTestScene;

    printf("Exiting.\n");
}
