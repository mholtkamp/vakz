#ifndef ACTOR_H
#define ACTOR_H

enum ActorEnum
{
    ACTOR_NONE     = -1,
    ACTOR_MATTER   = 0,
    ACTOR_LIGHT    = 1,
    ACTOR_PARTICLE = 2,

    ACTOR_MAX_NAME_LENGTH = 32,
    ACTOR_NAME_BUFFER_SIZE = ACTOR_MAX_NAME_LENGTH + 1
};

class Actor
{
public:

    Actor();
    virtual ~Actor();

    int GetClass();
    void* GetScene();
    char* GetName();
    void SetName(char* pNewName);
    void SetScene(void* pScene);

protected:

    int m_nClass;
    void* m_pScene;
    char m_arName[ACTOR_MAX_NAME_LENGTH + 1];
};


#endif