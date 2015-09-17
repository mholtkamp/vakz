#ifndef SOUND_H
#define SOUND_H

class Sound
{

public:

    Sound();
    ~Sound();

    void Load(const char* pFileName);

    void Play();

    void Stop();

    void PlayLooped();

    enum SoundEnum
    {
        FILE_NAME_BUFFER_SIZE = 128,
    };

private:

    char m_arFile[FILE_NAME_BUFFER_SIZE];

};

#endif
