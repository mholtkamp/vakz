#include "Vakz.h"
#include <stdio.h>

int main()
{
    SetWindowSize(1024,768);
    Initialize();

    while (Render())
    {
        printf("Rendering.\n");
    }

    printf("Exiting.\n");
}