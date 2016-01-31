#ifndef SOUND_H
#define SOUND_H

class Sound
{

public:

    //## **********************************************************************
    //## Constructor
    //## **********************************************************************
    Sound();

    //## **********************************************************************
    //## Destructor
    //## **********************************************************************
    ~Sound();

    //## **********************************************************************
    //## Initialize
    //## 
    //## Initializes sound features of vakz.
    //## **********************************************************************
    static void Initialize();

    //## **********************************************************************
    //## Shutdown
    //##
    //## Cleans up sound libraries used.
    //## **********************************************************************
    static void Shutdown();

    //## **********************************************************************
    //## Load
    //## 
    //## Loads a sound file from filename.
    //## The filename should represent a WAV song in 22.5kHZ 8bit format.
    //##
    //## Input:
    //##   pFileName - null terminated file path string.
    //## **********************************************************************
    void Load(const char* pFileName);

    //## **********************************************************************
    //## Play
    //##
    //## Plays the sound file that is currently loaded via Load().
    //## **********************************************************************
    void Play();

    //## **********************************************************************
    //## Stop
    //## 
    //## Stops playing this sound.
    //## **********************************************************************
    void Stop();

    //## **********************************************************************
    //## PlayLooped
    //##
    //## Plays the sound continuously.
    //## **********************************************************************
    void PlayLooped();

    enum SoundEnum
    {
        FILE_NAME_BUFFER_SIZE = 128,
        SOUND_PLAYER_COUNT = 8
    };

private:

    // Saved filename path
    char m_arFile[FILE_NAME_BUFFER_SIZE];

    // Pointer to sound buffer (for ANDROID)
    char* m_pSoundBuffer;

    // Size of soundbuffer
    int   m_nSoundBufferSize;

};

#endif
