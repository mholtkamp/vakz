#include "CoreAnd.h"
#include <stdio.h>
#include "VGL.h"
#include "Settings.h"
#include "Log.h"

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

// Initializes the Vakz Engine
int Initialize(void* pData)
{
    // Assign that app state to the engine data struct.
    vakzData.window = (ANativeWindow*) pData;

    // initialize OpenGL ES and EGL
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
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
        printf("Error making context current");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    vakzData.display = display;
    vakzData.context = context;
    vakzData.surface = surface;
    vakzData.width = w;
    vakzData.height = h;

    // Initialize GL state.
    glDisable(GL_DEPTH_TEST);

    return 0;
}

// Render the current scene 
int Render()
{

	return 1;
}

// Set the scene
int SetScene(Scene* pScene)
{
    s_pScene = pScene;
    return 0;
}

