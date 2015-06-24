#include "Log.h"
#include <Windows.h>
#include <stdio.h>

//*****************************************************************************
// LogError
//*****************************************************************************
void LogError(char* pError)
{
#if defined(WIN32)
    // Grab handle to console to change color.
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);

    // Print error message.
    printf(pError);
    printf("\n");

    // Return color back to normal
    SetConsoleTextAttribute(hConsole, 15);
#else
    // Print error message.
    printf(pError);
    printf("\n");
#endif
}

//*****************************************************************************
// LogWarning
//*****************************************************************************
void LogWarning(char* pWarning)
{
#if defined(WIN32)
    // Grab handle to console to change color.
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 14);

    // Print warning message.
    printf(pWarning);
    printf("\n");

    // Return color back to normal
    SetConsoleTextAttribute(hConsole, 15);
#else
    // Print warning message.
    printf(pWarning);
    printf("\n");
#endif
}

//*****************************************************************************
// LogDebug
//*****************************************************************************
void LogDebug(char* pDebug)
{
#if defined (WINDOWS)
    // Grab handle to console to change color.
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15);

    // Print debug message
    printf(pDebug);
    printf("\n");
#else
    // Print debug message
    printf(pDebug);
    printf("\n");
#endif
}