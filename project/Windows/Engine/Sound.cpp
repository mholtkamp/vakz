#include "Sound.h"
#include "Log.h"
#include "VFile.h"
#include <string.h>

#if defined (WINDOWS)
#include <windows.h>
#elif defined (ANDROID)
#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

static int s_nInitialized           = 0;
static android_app* s_pApp          = 0;
static SLObjectItf s_slEngineObj    = 0;
static SLEngineItf s_slEngine       = 0;
static SLObjectItf s_slOutputMixObj = 0;

static int s_nQueuePlayIndex = 0;
static SLObjectItf      s_arPlayerObjs[Sound::SOUND_PLAYER_COUNT] = {0};
static SLPlayItf        s_arPlayers[Sound::SOUND_PLAYER_COUNT]    = {0};
static SLBufferQueueItf s_arPlayerQueues[Sound::SOUND_PLAYER_COUNT];
#endif

//*****************************************************************************
// Constructor
//*****************************************************************************
Sound::Sound()
{
   memset(m_arFile, 0, FILE_NAME_BUFFER_SIZE);
   m_pSoundBuffer     = 0;
   m_nSoundBufferSize = 0;
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Sound::~Sound()
{
    if (m_pSoundBuffer != 0)
    {
        delete [] m_pSoundBuffer;
        m_pSoundBuffer = 0;
    }
}

//*****************************************************************************
// Initialize
//*****************************************************************************
void Sound::Initialize()
{
#if defined (ANDROID)
    if (s_nInitialized == 0)
    {
        // Set initialization flag immediately.
        // If there is an error initializing, then initializing multiple
        // times won't result in a memory leak.
        s_nInitialized = 1;

        LogDebug("Initializing Sound class.");

        SLresult result;
        const SLuint32      engineMixIIDCount = 1;
        const SLInterfaceID engineMixIIDs[] = {SL_IID_ENGINE};
        const SLboolean     engineMixReqs[] = {SL_BOOLEAN_TRUE};
        const SLuint32      outputMixIIDCount = 0;
        const SLInterfaceID outputMixIIDs[] = {};
        const SLboolean     outputMixReqs[] = {};

        // Create the engine object
        result = slCreateEngine(&s_slEngineObj,
                                0,
                                0,
                                engineMixIIDCount,
                                engineMixIIDs,
                                engineMixReqs);


        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error creating SL ES engine.");
            return;
        }

        // Realize the engine object to allocate memory
        result = (*s_slEngineObj)->Realize(s_slEngineObj, SL_BOOLEAN_FALSE);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error realizing the SL ES engine object.");
            return;
        }

        // Get engine object interface
        result = (*s_slEngineObj)->GetInterface(s_slEngineObj, SL_IID_ENGINE, &s_slEngine);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error retrieving SL engine interface.");
            return;
        }

        // Create audio output object
        result = (*s_slEngine)->CreateOutputMix(s_slEngine,
                                                &s_slOutputMixObj,
                                                outputMixIIDCount,
                                                outputMixIIDs,
                                                outputMixReqs);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error creating SL output mix object.");
            return;
        }

        // Realize the output mix object
        result = (*s_slOutputMixObj)->Realize(s_slOutputMixObj, SL_BOOLEAN_FALSE);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error realizing SL output mix object.");
            return;
        }


        // Now to create buffer queues for loading sound to be played.
        SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
        dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
        dataLocatorIn.numBuffers = 1;

        SLDataFormat_PCM dataFormat;
        dataFormat.formatType    = SL_DATAFORMAT_PCM;
        dataFormat.numChannels   = 1;
        dataFormat.samplesPerSec = SL_SAMPLINGRATE_22_05;
        dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_8;
        dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_8;
        dataFormat.channelMask   = SL_SPEAKER_FRONT_CENTER;
        dataFormat.endianness    = SL_BYTEORDER_LITTLEENDIAN;

        SLDataSource dataSource;
        dataSource.pLocator = &dataLocatorIn;
        dataSource.pFormat  = &dataFormat;

        SLDataLocator_OutputMix dataLocatorOut;
        dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        dataLocatorOut.outputMix = s_slOutputMixObj;

        SLDataSink dataSink;
        dataSink.pLocator = &dataLocatorOut;
        dataSink.pFormat  = 0;

        const SLuint32 soundPlayerIIDCount = 2;
        const SLInterfaceID soundPlayerIIDs[] = {SL_IID_PLAY, SL_IID_BUFFERQUEUE};
        const SLboolean soundPlayerReqs[] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

        for (int i = 0; i < SOUND_PLAYER_COUNT; i++)
        {
            // Create
            result = (*s_slEngine)->CreateAudioPlayer(s_slEngine,
                                                      &s_arPlayerObjs[i],
                                                      &dataSource,
                                                      &dataSink,
                                                      soundPlayerIIDCount,
                                                      soundPlayerIIDs,
                                                      soundPlayerReqs);

            if (result != SL_RESULT_SUCCESS)
            {
                LogError("Error creating audio player.");
                return;
            }

            // Realize
            result = (*(s_arPlayerObjs[i]))->Realize(s_arPlayerObjs[i], SL_BOOLEAN_FALSE);

            if (result != SL_RESULT_SUCCESS)
            {
                LogError("Error realizing audio player.");
                return;
            }

            // Player Interface
            result = (*(s_arPlayerObjs[i]))->GetInterface(s_arPlayerObjs[i],
                                                          SL_IID_PLAY,
                                                          &(s_arPlayers[i]));

            if (result != SL_RESULT_SUCCESS)
            {
                LogError("Error creating player interface.");
                return;
            }

            // Buffer Queue Interface
            result = (*(s_arPlayerObjs[i]))->GetInterface(s_arPlayerObjs[i],
                                                          SL_IID_BUFFERQUEUE,
                                                          &(s_arPlayerQueues[i]));

            if (result != SL_RESULT_SUCCESS)
            {
                LogError("Error creating buffer queue interface.");
                return;
            }

            result = (*(s_arPlayers[i]))->SetPlayState(s_arPlayers[i], SL_PLAYSTATE_PLAYING);

            if (result != SL_RESULT_SUCCESS)
            {
                LogError("Error setting audio player to play state.");
                return;
            }
        }
    }

#endif
}

//*****************************************************************************
// Shutdown
//*****************************************************************************
void Sound::Shutdown()
{
#if defined(ANDROID)
    if (s_nInitialized != 0)
    {
        LogDebug("Shutting down Sound class.");

        // Destroy the audio players first
        for (int i = 0; i < SOUND_PLAYER_COUNT; i++)
        {
            if (s_arPlayerObjs[i] != 0)
            {
                (*(s_arPlayerObjs[i]))->Destroy(s_arPlayerObjs[i]);
                s_arPlayerObjs[i]   = 0;
                s_arPlayers[i]      = 0;
                s_arPlayerQueues[i] = 0;
            }
        }

        if (s_slOutputMixObj != 0)
        {
            (*s_slOutputMixObj)->Destroy(s_slOutputMixObj);
            s_slOutputMixObj = 0;
        }

        if (s_slEngineObj != 0)
        {
            (*s_slEngineObj)->Destroy(s_slEngineObj);
            s_slEngineObj = 0;
            s_slEngine    = 0;
        }

        s_nInitialized = 0;
    }
#endif
}

//*****************************************************************************
// Load
//*****************************************************************************
void Sound::Load(const char* pFileName)
{
    memset(m_arFile, 0, FILE_NAME_BUFFER_SIZE);
    strcpy(m_arFile, pFileName);

#if defined (ANDROID)
    // Load sound into memory
    int nFileSize = GetAssetSize(pFileName);

    m_pSoundBuffer     = new char[nFileSize];
    m_nSoundBufferSize = nFileSize;

    ReadAsset(pFileName, m_pSoundBuffer, nFileSize);
#endif
}

//*****************************************************************************
// Play
//*****************************************************************************
void Sound::Play()
{
#if defined (WINDOWS)
    PlaySound(m_arFile, 0, SND_FILENAME | SND_ASYNC);
#elif defined (ANDROID)
    SLresult result;
    SLuint32 playerState;

    (*(s_arPlayerObjs[s_nQueuePlayIndex]))->GetState(s_arPlayerObjs[s_nQueuePlayIndex], &playerState);

    if (playerState == SL_OBJECT_STATE_REALIZED)
    {
        // Clear any sound that was there before
        result = (*(s_arPlayerQueues[s_nQueuePlayIndex]))->Clear(s_arPlayerQueues[s_nQueuePlayIndex]);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Error clearing player queue.");
            return;
        }

        // Add the new sound buffer to queue
        result = (*(s_arPlayerQueues[s_nQueuePlayIndex]))->Enqueue(s_arPlayerQueues[s_nQueuePlayIndex],
                                                                   m_pSoundBuffer,
                                                                   m_nSoundBufferSize);

        if (result != SL_RESULT_SUCCESS)
        {
            LogError("Could not enqueue sound buffer to queue.");
            return;
        }


        s_nQueuePlayIndex++;
        if (s_nQueuePlayIndex >= SOUND_PLAYER_COUNT)
        {
            // Loop back around to first queue for next play.
            s_nQueuePlayIndex = 0;
        }
    }
#endif
}

//*****************************************************************************
// Stop
//*****************************************************************************
void Sound::Stop()
{
#if defined (WINDOWS)

#elif defined (ANDROID)

#endif
}

//*****************************************************************************
// PlayLooped
//*****************************************************************************
void Sound::PlayLooped()
{
#if defined (WINDOWS)
    PlaySound(m_arFile, 0, SND_FILENAME | SND_ASYNC);
#elif defined (ANDROID)
    //@@ TODO
#endif
}
