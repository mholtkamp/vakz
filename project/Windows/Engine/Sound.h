#ifndef SOUND_H
#define SOUND_H

class Sound
{

public:

    Sound();
    ~Sound();

    static void Initialize();

    static void Shutdown();

    void Load(const char* pFileName);

    void Play();

    void Stop();

    void PlayLooped();

    enum SoundEnum
    {
        FILE_NAME_BUFFER_SIZE = 128,
        SOUND_PLAYER_COUNT = 8
    };

private:

    char m_arFile[FILE_NAME_BUFFER_SIZE];

    char* m_pSoundBuffer;
    int   m_nSoundBufferSize;

};

#endif
