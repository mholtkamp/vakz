#include "Log.h"
#include <stdio.h>

#if defined(WINDOWS)
#include <Windows.h>
#endif

//*****************************************************************************
// LogError
//*****************************************************************************
void LogError(const char* pError)
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
    puts(pError);
    puts("\n");
#endif
}

//*****************************************************************************
// LogWarning
//*****************************************************************************
void LogWarning(const char* pWarning)
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
    puts(pWarning);
    puts("\n");
#endif
}

//*****************************************************************************
// LogDebug
//*****************************************************************************
void LogDebug(const char* pDebug)
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
    puts(pDebug);
    puts("\n");
#endif
}
