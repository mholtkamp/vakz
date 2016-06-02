#include "Vakz.h"
#include "VGL.h"
#include "LevelEditor.h"

int main()
{
    SetFullScreen(1);
    Initialize();

    LevelEditor* pEditor = new LevelEditor();
    SetScene(&pEditor->m_scene);

    while ((GetStatus() & VAKZ_QUIT) == 0)
    {
        Update();
        Render();
    }

    exit(0);
}
