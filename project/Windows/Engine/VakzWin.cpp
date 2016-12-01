#if defined(WINDOWS)

/*
*        This Code Was Created By Jeff Molofee 2000
*        A HUGE Thanks To Fredric Echols For Cleaning Up
*        And Optimizing This Code, Making It More Flexible!
*        If You've Found This Code Useful, Please Let Me Know.
*        Visit My Site At nehe.gamedev.net
*/

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#include "Vakz.h"
#include "Scene.h"
#include "stdio.h"
#include <windows.h>        // Header File For Windows
#include "VGL.h"
#include "Settings.h"
#include "Log.h"
#include "VInput.h"
#include "VSocket.h"

// Pointer to the scene object used during rendering.
static           Scene* s_pScene    = 0;

// Status bit field
static    unsigned char s_ucStatus  = 0;

HDC            hDC = NULL;  // Private GDI Device Context
HGLRC          hRC = NULL;    // Permanent Rendering Context
HWND           hWnd = NULL;    // Holds Our Window Handle
HINSTANCE      hInstance;     // Holds The Instance Of The Application

char    szWindowTitle[256] = "Vakz";

int     keys[256];            // Array Used For The Keyboard Routine
int     active = TRUE;        // Window Active Flag Set To TRUE By Default

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);    // Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)      // Resize And Initialize The GL Window
{
    if (height == 0)                                     // Prevent A Divide By Zero By
    {
        height = 1;                                      // Making Height Equal One
    }

    glViewport(0, 0, width, height);                     // Reset The Current Viewport
    SetWindowSize(width, height);
}

int InitGL(GLvoid)                                       // All Setup For OpenGL Goes Here
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);                // Black Background
    glClearDepthf(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                             // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                              // The Type Of Depth Testing To Do
    return TRUE;                                         // Initialization Went OK
}

int DrawGLScene(GLvoid)                                  // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen And Depth Buffer

    if (s_pScene != 0)
    {
        s_pScene->Render();
    }
    else
    {
        LogWarning("No scene is set.");
    }
    return TRUE;                                         // Everything Went OK
}

GLvoid KillGLWindow(GLvoid)                              // Properly Kill The Window
{
    if (hRC)                                             // Do We Have A Rendering Context?
    {
        if (!wglMakeCurrent(NULL, NULL))                 // Are We Able To Release The DC And RC Contexts?
        {
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC))                      // Are We Able To Delete The RC?
        {
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }
        hRC = NULL;                                      // Set RC To NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC))                    // Are We Able To Release The DC
    {
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                      // Set DC To NULL
    }

    if (hWnd && !DestroyWindow(hWnd))                    // Are We Able To Destroy The Window?
    {
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                     // Set hWnd To NULL
    }

    if (!UnregisterClass("OpenGL", hInstance))           // Are We Able To Unregister Class
    {
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                                    // Set hInstance To NULL
    }
}

LRESULT CALLBACK WndProc(HWND    hWnd,            // Handle For This Window
                         UINT    uMsg,            // Message For This Window
                         WPARAM  wParam,          // Additional Message Information
                         LPARAM  lParam)          // Additional Message Information
{
    switch (uMsg)                                 // Check For Windows Messages
    {
    case WM_ACTIVATE:                             // Watch For Window Activate Message
    {
        if (!HIWORD(wParam))                      // Check Minimization State
        {
            s_ucStatus |= VAKZ_ACTIVE;                        // Program Is Active
        }
        else
        {
            s_ucStatus &= (~VAKZ_ACTIVE);                       // Program Is No Longer Active
        }

        return 0;                                 // Return To The Message Loop
    }

    case WM_SYSCOMMAND:                           // Intercept System Commands
    {
        switch (wParam)                           // Check System Calls
        {
        case SC_SCREENSAVE:                       // Screensaver Trying To Start?
        case SC_MONITORPOWER:                     // Monitor Trying To Enter Powersave?
            return 0;                             // Prevent From Happening
        }
        break;                                    // Exit
    }

    case WM_CLOSE:                                // Did We Receive A Close Message?
    {
        PostQuitMessage(0);                       // Send A Quit Message
        return 0;                                 // Jump Back
    }

    case WM_KEYDOWN:                              // Is A Key Being Held Down?
    {
        SetKey(wParam);                         // If So, Mark It As TRUE
        return 0;                                 // Jump Back
    }

    case WM_KEYUP:                                // Has A Key Been Released?
    {
        ClearKey(wParam);                        // If So, Mark It As FALSE
        return 0;                                 // Jump Back
    }

    case WM_LBUTTONDOWN:
    {
        SetButton(VBUTTON_LEFT);
        return 0;
    }

    case WM_RBUTTONDOWN:
    {
        SetButton(VBUTTON_RIGHT);
        return 0;
    }

    case WM_MBUTTONDOWN:
    {
        SetButton(VBUTTON_MIDDLE);
        return 0;
    }

    case WM_XBUTTONDOWN:
    {
        int nButton = HIWORD(wParam);
        
        if (nButton == 1)
        {
            SetButton(VBUTTON_X1);
        }
        else if (nButton == 2)
        {
            SetButton(VBUTTON_X2);
        }

        return 0;
    }

    case WM_LBUTTONUP:
    {
        ClearButton(VBUTTON_LEFT);
        return 0;
    }

    case WM_RBUTTONUP:
    {
        ClearButton(VBUTTON_RIGHT);
        return 0;
    }

    case WM_MBUTTONUP:
    {
        ClearButton(VBUTTON_MIDDLE);
        return 0;
    }

    case WM_XBUTTONUP:
    {
        int nButton = HIWORD(wParam);
        
        if (nButton == 1)
        {
            ClearButton(VBUTTON_X1);
        }
        else if (nButton == 2)
        {
            ClearButton(VBUTTON_X2);
        }

        return 0;
    }

    case WM_MOUSEWHEEL:
    {
        SetScrollWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA);

        return 0;
    }
    case WM_MOUSEMOVE:
    {
        int nX = LOWORD(lParam);
        int nY = HIWORD(lParam);

        // Invert the y axis to match the bottom-left origin
        // convention used thoughout Vakz.
        SetMousePosition(nX, (g_nScreenHeight - 1) - nY);

        return 0;
    }

    case WM_SIZE:                                 // Resize The OpenGL Window
    {
        ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
        return 0;                                 // Jump Back
    }
    }

    // Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Sets the window size
int SetWindowSize(int nWidth,
                  int nHeight)
{
    if (nWidth  > 0 &&
        nHeight > 0)
    {
        g_nScreenWidth  = nWidth;
        g_nScreenHeight = nHeight;
        return 1;
    }
    return 0;
}

// Sets resolution
int SetResolution(int nResX,
                  int nResY)
{
    if (nResX > 0 &&
        nResY > 0)
    {
        g_nResolutionX = nResX;
        g_nResolutionY = nResY;
        return 1;
    }

    return 0;
}


// Set the window title
int SetTitle(char* szTitle)
{
    if (szTitle != 0)
    {
        memcpy(szWindowTitle, szTitle, 255);
        szWindowTitle[255] = 0;
        return 1;
    }
    
    return 0;
}

// Initializes the Vakz Engine
int Initialize(void* pData)
{
    GLuint        PixelFormat;                      // Holds The Results After Searching For A Match
    WNDCLASS    wc;                                 // Windows Class Structure
    DWORD        dwExStyle;                         // Window Extended Style
    DWORD        dwStyle;                           // Window Style
    RECT        WindowRect;                         // Grabs Rectangle Upper Left / Lower Right Values

    if (g_nFullScreen != 0)
    {
        // If the window should be full screen,
        // query the native resolution of primary monitor
        g_nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        g_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    }

    WindowRect.left   = (long) 0;                      // Set Left Value To 0
    WindowRect.right  = (long) g_nScreenWidth;         // Set Right Value To Requested Width
    WindowRect.top    = (long) 0;                       // Set Top Value To 0
    WindowRect.bottom = (long) g_nScreenHeight;       // Set Bottom Value To Requested Height

    hInstance = GetModuleHandle(NULL);              // Grab An Instance For Our Window
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Size, And Own DC For Window.
    wc.lpfnWndProc = (WNDPROC)WndProc;              // WndProc Handles Messages
    wc.cbClsExtra = 0;                              // No Extra Window Data
    wc.cbWndExtra = 0;                              // No Extra Window Data
    wc.hInstance = hInstance;                       // Set The Instance
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);         // Load The Default Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Load The Arrow Pointer
    wc.hbrBackground = NULL;                        // No Background Required For GL
    wc.lpszMenuName = NULL;                         // We Don't Want A Menu
    wc.lpszClassName = "OpenGL";                    // Set The Class Name

    if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
    {
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Return FALSE
    }

    if (g_nFullScreen != 0)
    {
        dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
        dwStyle = WS_POPUP | WS_CLIPCHILDREN;                  // Windows Style
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
        dwStyle = WS_OVERLAPPEDWINDOW;                  // Windows Style
    }


    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);    // Adjust Window To True Requested Size

    // Create The Window
    if (!(hWnd = CreateWindowEx(dwExStyle,                         // Extended Style For The Window
        "OpenGL",                            // Class Name
        szWindowTitle,                       // Window Title
        dwStyle |                            // Defined Window Style
        WS_CLIPSIBLINGS |                    // Required Window Style
        WS_CLIPCHILDREN,                     // Required Window Style
        0, 0,                                // Window Position
        WindowRect.right - WindowRect.left,  // Calculate Window Width
        WindowRect.bottom - WindowRect.top,  // Calculate Window Height
        NULL,                                // No Parent Window
        NULL,                                // No Menu
        hInstance,                           // Instance
        NULL)))                              // Dont Pass Anything To WM_CREATE
    {
        KillGLWindow();                      // Reset The Display
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                        // Return FALSE
    }

    static    PIXELFORMATDESCRIPTOR pfd =          // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),             // Size Of This Pixel Format Descriptor
        1,                                         // Version Number
        PFD_DRAW_TO_WINDOW |                       // Format Must Support Window
        PFD_SUPPORT_OPENGL |                       // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                          // Must Support Double Buffering
        PFD_TYPE_RGBA,                             // Request An RGBA Format
        32,                                        // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                          // Color Bits Ignored
        0,                                         // No Alpha Buffer
        0,                                         // Shift Bit Ignored
        0,                                         // No Accumulation Buffer
        0, 0, 0, 0,                                // Accumulation Bits Ignored
        24,                                        // 24Bit Z-Buffer (Depth Buffer)  
        8,                                         // 8 bit Stencil Buffer
        0,                                         // No Auxiliary Buffer
        PFD_MAIN_PLANE,                            // Main Drawing Layer
        0,                                         // Reserved
        0, 0, 0                                    // Layer Masks Ignored
    };

    if (!(hDC = GetDC(hWnd)))                          // Did We Get A Device Context?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) // Did Windows Find A Matching Pixel Format?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))       // Are We Able To Set The Pixel Format?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    if (!(hRC = wglCreateContext(hDC)))                // Are We Able To Get A Rendering Context?
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    if (!wglMakeCurrent(hDC, hRC))                     // Try To Activate The Rendering Context
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    if (g_nFullScreen != 0)
    {
        DEVMODE dmScreenSettings;                   // Device Mode
        memset(&dmScreenSettings,0,sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
        dmScreenSettings.dmSize=sizeof(dmScreenSettings);       // Size Of The Devmode Structure
        dmScreenSettings.dmPelsWidth    = g_nScreenWidth;            // Selected Screen Width
        dmScreenSettings.dmPelsHeight   = g_nScreenHeight;           // Selected Screen Height
        dmScreenSettings.dmBitsPerPel   = 32;             // Selected Bits Per Pixel
        dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

        ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
    }

    if (LoadVGL() == 0)
    {
        KillGLWindow();                                // Reset the display
        MessageBox(NULL, "Failed to load ES30 extensions.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return 0;                                      // Return 0
    }

    if (LoadShaders() == 0)
    {
        KillGLWindow();
        MessageBox(NULL, "Failed to compile/link shaders.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    }

    ShowWindow(hWnd, SW_SHOW);                         // Show The Window
    SetForegroundWindow(hWnd);                         // Slightly Higher Priority
    SetFocus(hWnd);                                    // Sets Keyboard Focus To The Window
    ReSizeGLScene(g_nScreenWidth, g_nScreenHeight);    // Set Up Our Perspective GL Screen

    if (g_nResolutionX == 0 ||
        g_nResolutionY == 0)
    {
        g_nResolutionX = g_nScreenWidth;
        g_nResolutionY = g_nScreenHeight;
    }

    if (!InitGL())                                     // Initialize Our Newly Created GL Window
    {
        KillGLWindow();                                // Reset The Display
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                  // Return FALSE
    }

    InitializeSoftKeyboard();

    Socket::Initialize();

    return TRUE;                                       // Success
}

// Render the current scene 
int Render()
{
    // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
    if (s_ucStatus & VAKZ_ACTIVE)              // Program Active?
    {
        if (keys[VK_ESCAPE])                   // Was ESC Pressed?
        {
            s_ucStatus |= VAKZ_QUIT;           // ESC Signalled A Quit
        }
        else                                   // Not Time To Quit, Update Screen
       {
            DrawGLScene();                     // Draw The Scene
            SwapBuffers(hDC);                  // Swap Buffers (Double Buffering)
        }
    }

    return 1;
}

// Set the scene
int SetScene(void* pScene)
{
    s_pScene = reinterpret_cast<Scene*>(pScene);
    return 0;
}

void SetClearColor(float fRed,
                   float fGreen,
                   float fBlue,
                   float fAlpha)
{
    glClearColor(fRed, fGreen, fBlue, fAlpha);
}

void Shutdown()
{
    KillGLWindow();
}

void Update()
{
    MSG     msg;                                   // Windows Message Structure
    BOOL    done = FALSE;                          // Bool Variable To Exit Loop

    // Reset just down/up keys
    ResetJusts();

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // Is There A Message Waiting?
    {
        if (msg.message == WM_QUIT)                // Have We Received A Quit Message?
        {
            s_ucStatus |= VAKZ_QUIT;                   // If So done=TRUE
        }
        else                                       // If Not, Deal With Window Messages
        {
            TranslateMessage(&msg);                // Translate The Message
            DispatchMessage(&msg);                 // Dispatch The Message
        }
    }

    // Update the scene
    if (s_pScene != 0)
    {
        s_pScene->Update();
    }

    RefreshControllerStates();
    UpdateSoftKeyboard();
}

unsigned char GetStatus()
{
    return s_ucStatus;
}

void SetFullScreen(int nFullScreen)
{
    g_nFullScreen = nFullScreen;
}

#endif
