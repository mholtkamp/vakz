#include "Vakz.h"
#include <stdio.h>

int main()
{
    Initialize();

    while (Render())
    {
        printf("Rendering.\n");
    }

    printf("Exiting.\n");
}