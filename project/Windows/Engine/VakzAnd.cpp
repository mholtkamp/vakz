#if defined(ANDROID)

#include "Vakz.h"
#include <stdio.h>
#include "VGL.h"
#include "VInput.h"
#include "VFile.h"
#include "Settings.h"
#include "Log.h"
#include "Scene.h"
#include "MeshLoader.h"
#include <android_native_app_glue.h>
#include <android/input.h>
#include <android/window.h>

//## VakzData struct to hold information needed when rendering.
typedef struct VakzData
               {
                    struct android_app* state;
                    ANativeWindow* window;
                    ANativeActivity* activity;
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
static           Scene* s_pScene    = 0;

// Status bitfield
static    unsigned char s_ucStatus  = 0;

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

void Shutdown()
{
    if (vakzData.display != EGL_NO_DISPLAY) {
        eglMakeCurrent(vakzData.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (vakzData.context != EGL_NO_CONTEXT) {
            eglDestroyContext(vakzData.display, vakzData.context);
        }
        if (vakzData.surface != EGL_NO_SURFACE) {
            eglDestroySurface(vakzData.display, vakzData.surface);
        }
        eglTerminate(vakzData.display);
    }

    // Stop rendering
    s_ucStatus &= (~VAKZ_ACTIVE);
    s_ucStatus |= VAKZ_QUIT;

    vakzData.display = EGL_NO_DISPLAY;
    vakzData.context = EGL_NO_CONTEXT;
    vakzData.surface = EGL_NO_SURFACE;
}

static void HandleCommand(struct android_app* app,
                          int cmd)
{
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            LogDebug("APP_CMD_INIT_WINDOW");
            // The window is being shown, get it ready.
            if (app->window != NULL)
            {
                InitializeGraphics(app->window);
                nInitialized = 1;
                Render();
            }
            break;
        }
        case APP_CMD_TERM_WINDOW:
        {
            LogDebug("APP_CMD_TERM_WINDOW");
            // The window is being hidden or closed, clean it up.
            //Shutdown();
            break;
        }
        case APP_CMD_GAINED_FOCUS:
        {
            LogDebug("APP_CMD_GAINED_FOCUS");
            // Enable rendering again.
            s_ucStatus |= VAKZ_ACTIVE;
            break;
        }
        case APP_CMD_LOST_FOCUS:
        {
            LogDebug("APP_CMD_LOST_FOCUS");
            // Disable rendering.
            s_ucStatus &= (~VAKZ_ACTIVE);
            break;
        }
        case APP_CMD_START:
        {
            LogDebug("APP_CMD_START");
            break;
        }
        case APP_CMD_RESUME:
        {
            LogDebug("APP_CMD_RESUME");
            break;
        }
        case APP_CMD_PAUSE:
        {
            LogDebug("APP_CMD_PAUSE");
            break;
        }
        case APP_CMD_STOP:
        {
            LogDebug("APP_CMD_STOP");
            s_ucStatus |= VAKZ_QUIT;
            break;
        }
        case APP_CMD_DESTROY:
        {
            LogDebug("APP_CMD_DESTROY");
            s_ucStatus |= VAKZ_QUIT;
            break;
        }

    }
}

static int HandleInput(struct android_app* app, AInputEvent* event)
{
    int nAction    = 0;
    int nPointer   = 0;
    int nDevice    = 0;
    int nKey       = 0;
    int nSource    = 0;
    int nContIndex = 0;

    nDevice = AInputEvent_getDeviceId(event);
    nSource = AInputEvent_getSource(event) & 0xffffff00;


    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        nAction  = AMotionEvent_getAction(event);
        nPointer = (0xff00 & nAction) >> 8;
        nAction  = 0x00ff & nAction;

        if (nAction == AMOTION_EVENT_ACTION_DOWN)
        {
            SetTouch(nPointer);
            SetTouchPosition(static_cast<int>(AMotionEvent_getX(event, nPointer)),
                            (g_nScreenHeight - 1) - static_cast<int>(AMotionEvent_getY(event, nPointer)),
                            nPointer);
            return 1;
        }
        else if (nAction == AMOTION_EVENT_ACTION_UP)
        {
            ClearTouch(nPointer);
            SetTouchPosition(static_cast<int>(AMotionEvent_getX(event, nPointer)),
                            (g_nScreenHeight - 1) - static_cast<int>(AMotionEvent_getY(event, nPointer)),
                            nPointer);
            return 1;
        }
        else if (nAction == AMOTION_EVENT_ACTION_POINTER_DOWN)
        {
            SetTouch(nPointer);
            SetTouchPosition(static_cast<int>(AMotionEvent_getX(event, nPointer)),
                            (g_nScreenHeight - 1) - static_cast<int>(AMotionEvent_getY(event, nPointer)),
                            nPointer);
            return 1;
        }
        else if (nAction == AMOTION_EVENT_ACTION_POINTER_UP)
        {
            ClearTouch(nPointer);
            SetTouchPosition(static_cast<int>(AMotionEvent_getX(event, nPointer)),
                            (g_nScreenHeight - 1) - static_cast<int>(AMotionEvent_getY(event, nPointer)),
                            nPointer);
            return 1;
        }
        else if (nAction == AMOTION_EVENT_ACTION_MOVE)
        {
            for (int nPtr = 0; nPtr < VINPUT_MAX_TOUCHES; nPtr++)
            {
                SetTouchPosition(static_cast<int>(AMotionEvent_getX(event, nPtr)),
                                (g_nScreenHeight - 1) - static_cast<int>(AMotionEvent_getY(event, nPtr)),
                                nPtr);
            }

            if (nSource & AINPUT_SOURCE_JOYSTICK)
            {
                nContIndex = GetControllerIndex(nDevice);

                float fAxisX = AMotionEvent_getAxisValue(event,
                                                         AMOTION_EVENT_AXIS_X,
                                                         0);
                float fAxisY = AMotionEvent_getAxisValue(event,
                                                         AMOTION_EVENT_AXIS_Y,
                                                         0);
                float fAxisZ = AMotionEvent_getAxisValue(event,
                                                         AMOTION_EVENT_AXIS_Z,
                                                         0);
                float fAxisRZ = AMotionEvent_getAxisValue(event,
                                                          AMOTION_EVENT_AXIS_RZ,
                                                          0);
                float fAxisHatX = AMotionEvent_getAxisValue(event,
                                                            AMOTION_EVENT_AXIS_HAT_X,
                                                            0);
                float fAxisHatY = AMotionEvent_getAxisValue(event,
                                                            AMOTION_EVENT_AXIS_HAT_Y,
                                                            0);
                float fAxisTriggerL = AMotionEvent_getAxisValue(event,
                                                            AMOTION_EVENT_AXIS_LTRIGGER,
                                                            0);
                float fAxisTriggerR = AMotionEvent_getAxisValue(event,
                                                            AMOTION_EVENT_AXIS_RTRIGGER,
                                                            0);

                SetControllerAxisValue(VCONT_AXIS_X,  fAxisX,  nContIndex);
                SetControllerAxisValue(VCONT_AXIS_Y,  fAxisY,  nContIndex);
                SetControllerAxisValue(VCONT_AXIS_Z,  fAxisZ,  nContIndex);
                SetControllerAxisValue(VCONT_AXIS_RZ, fAxisRZ, nContIndex);
                SetControllerAxisValue(VCONT_AXIS_HAT_X, fAxisHatX, nContIndex);
                SetControllerAxisValue(VCONT_AXIS_HAT_Y, fAxisHatY, nContIndex);
                SetControllerAxisValue(VCONT_AXIS_LTRIGGER, fAxisTriggerL, nContIndex);
                SetControllerAxisValue(VCONT_AXIS_RTRIGGER, fAxisTriggerR, nContIndex);
            }
            return 1;
        }
    }
    else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
    {
        nAction = AKeyEvent_getAction(event);

        if (nAction == AKEY_EVENT_ACTION_DOWN)
        {
            nKey = AKeyEvent_getKeyCode(event);

            if (nSource & AINPUT_SOURCE_GAMEPAD)
            {
                nContIndex = GetControllerIndex(nDevice);
                SetControllerButton(nKey, nContIndex);
            }
            else
            {
                SetKey(nKey);
            }

            return 1;
        }
        else if (nAction == AKEY_EVENT_ACTION_UP)
        {
            nKey = AKeyEvent_getKeyCode(event);

            if (nSource & AINPUT_SOURCE_GAMEPAD)
            {
                nContIndex = GetControllerIndex(nDevice);
                ClearControllerButton(nKey, nContIndex);
            }
            else
            {
                ClearKey(nKey);
            }

            return 1;
        }
    }

    return 0;
}

// Initializes the Vakz Engine
int Initialize(void* pData)
{
    // Make sure glue isn't stripped.
    app_dummy();

    struct android_app* pState = reinterpret_cast<struct android_app*>(pData);

    // Assign the app state
    vakzData.state = pState;
    vakzData.window = pState->window;
    vakzData.activity = pState->activity;

    // Set the command handler
    pState->onAppCmd = HandleCommand;

    // Set the input handler
    pState->onInputEvent = HandleInput;

    int ident;
    int events;
    struct android_poll_source* source;

    // Keep processing events until the OpenGL context is created.
    while(nInitialized == 0)
    {
        while ((ident=ALooper_pollAll(nAnimating ? 1 : -1, NULL, &events,
                (void**)&source)) >= 0)
        {
            // Process this event.
            if (source != NULL)
            {
                source->process(pState, source);
            }
        }
    }

    // Set window flag to keep screen on
    //ANativeActivity_setWindowFlags(vakzData.activity, AWINDOW_FLAG_KEEP_SCREEN_ON, 0);

    // Set the asset manager where it is needed
    SetAssetManager(pState->activity->assetManager);

    InitializeSoftKeyboard();

    return 0;
}

// Render the current scene 
int Render()
{
    if (vakzData.display != 0 &&
        (s_ucStatus & VAKZ_ACTIVE))
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

void Update()
{
    // Read all pending events.
    int ident;
    int events;
    struct android_poll_source* source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((ident=ALooper_pollAll(0, NULL, &events,
            (void**)&source)) >= 0) {

        // Process this event.
        if (source != NULL) {
            source->process(vakzData.state, source);
        }

        // Check if we are exiting.
        if (vakzData.state->destroyRequested != 0) {
            Shutdown();
            return;
        }
    }

    // Update scene
    if (s_pScene != 0)
    {
        s_pScene->Update();
    }

    UpdateSoftKeyboard();
}

unsigned char GetStatus()
{
    return s_ucStatus;
}

#endif

