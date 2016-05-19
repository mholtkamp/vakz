#include "Actor.h"
#include <string.h>

Actor::Actor()
{
    m_nClass = ACTOR_NONE;
    m_pScene = 0;
    memset(m_arName, 0, ACTOR_NAME_BUFFER_SIZE);
}

Actor::~Actor()
{

}

int Actor::GetClass()
{
    return m_nClass;
}

void* Actor::GetScene()
{
    return m_pScene;
}

void Actor::SetScene(void* pScene)
{
    m_pScene = pScene;
}

char* Actor::GetName()
{
    return m_arName;
}

void Actor::SetName(char* pNewName)
{
    strncpy(m_arName, pNewName, ACTOR_MAX_NAME_LENGTH);
}
