#include "Vakz.h"
#include "VGL.h"
#include "LevelEditor.h"

int main()
{
    //SetFullScreen(1);
    SetWindowSize(1366, 768);
    Initialize();

    LevelEditor* pEditor = new LevelEditor();
    pEditor->Initialize();
    SetScene(&pEditor->m_scene);

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        Update();

        pEditor->HandleInput();
        Render();
    }

    delete pEditor;
    pEditor = 0;

    exit(0);
}
