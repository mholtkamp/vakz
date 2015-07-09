#if defined(ANDROID)

#include "Vakz.h"
#include <stdio.h>
#include "VGL.h"
#include "Settings.h"
#include "Log.h"
#include "Scene.h"
#include <android_native_app_glue.h>

//## VakzData struct to hold information needed when rendering.
typedef struct VakzData
               {
                    ANativeWindow* window;
                    int animating;
                    EGLDisplay display;
                    EGLSurface surface;
                    EGLContext context;
                    int32_t width;
                    int32_t height;
               } VakzData;

// Create static struct to hold data needed by engine.
static VakzData vakzData;

static volatile int nInitialized = 0;
static int nAnimating = 1;

// Pointer to the scene object used during rendering.
static Scene* s_pScene = 0;

static const EGLint arContextAttribs[] =
{
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_NONE
};

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

int InitializeGraphics(ANativeWindow* pWindow)
{
    // initialize OpenGL ES and EGL
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */

    vakzData.window = pWindow;

    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(vakzData.window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, vakzData.window, NULL);

    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LogWarning("Error making context current");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    vakzData.display = display;
    vakzData.context = context;
    vakzData.surface = surface;
    vakzData.width = w;
    vakzData.height = h;

    char arBuff[64] = {0};
    sprintf(arBuff, "Width: %d Height: %d", vakzData.width, vakzData.height);
    LogDebug((const char*) arBuff);

    SetWindowSize(vakzData.width, vakzData.height);

    // Initialize GL state.
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);                // Black Background
    glClearDepthf(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                             // Enables Depth Testing
    glDepthFunc(GL_LESS);                              // The Type Of Depth Testing To Do

    // Load all shaders used by Vakz
    if (LoadShaders() == 0)
    {
        LogError("Failed to compile/link shaders.");
    }

    return 0;
}

static void HandleCommand(struct android_app* app,
                          int cmd)
{
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            LogWarning("**** APP_CMD_INIT_WINDOW ****");
            // The window is being shown, get it ready.
            if (app->window != NULL)
            {
                LogWarning("**** app->window != null ****");
                InitializeGraphics(app->window);
                nInitialized = 1;
                Render();
            }
            break;
        }
    }
}

// Initializes the Vakz Engine
int Initialize(void* pData)
{
    // Make sure glue isn't stripped.
    app_dummy();

    struct android_app* pState = reinterpret_cast<struct android_app*>(pData);

    // Assign that app state to the engine data struct.
    vakzData.window = pState->window;

    // Set the command handler
    pState->onAppCmd = HandleCommand;


    int ident;
    int events;
    struct android_poll_source* source;

    LogWarning("****ISLAND APP STARTING****");

    // Keep processing events until the OpenGL context is created.
    while(nInitialized == 0)
    {
        while ((ident=ALooper_pollAll(nAnimating ? 1 : -1, NULL, &events,
                (void**)&source)) >= 0)
        {
            LogWarning("****Processing Event****");
            // Process this event.
            if (source != NULL)
            {
                source->process(pState, source);
            }
        }
    }

    return 0;
}

// Render the current scene 
int Render()
{
    if (vakzData.display != 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (s_pScene != 0)
        {
            s_pScene->Render();
        }
        else
        {
            LogWarning("No scene is set.");
        }
        eglSwapBuffers(vakzData.display, vakzData.surface);
        return 1;
    }
    return 0;
}

// Set the scene
int SetScene(void* pScene)
{
    s_pScene = reinterpret_cast<Scene*>(pScene);
    return 0;
}

// Sets the GL clear color
void SetClearColor(float fRed,
                   float fGreen,
                   float fBlue,
                   float fAlpha)
{
    glClearColor(fRed, fGreen, fBlue, fAlpha);
}

#endif

