#include "Sound.h"

#if defined (WINDOWS)
#include <windows.h>
#elif defined (ANDROID)

#endif


Sound::Sound()
{
   memset(m_arFile, 0, FILE_NAME_BUFFER_SIZE);
}

Sound::~Sound()
{

}

void Sound::Load(const char* pFileName)
{
    memset(m_arFile, 0, FILE_NAME_BUFFER_SIZE);
    strcpy(m_arFile, pFileName);
}

void Sound::Play()
{
#if defined (WINDOWS)
    PlaySound(m_arFile, 0, SND_FILENAME | SND_ASYNC);
#elif defined (ANDROID)

#endif
}

void Sound::Stop()
{
#if defined (WINDOWS)

#elif defined (ANDROID)

#endif
}

void Sound::PlayLooped()
{
#if defined (WINDOWS)
    PlaySound(m_arFile, 0, SND_FILENAME | SND_ASYNC);
#elif defined (ANDROID)

#endif
}