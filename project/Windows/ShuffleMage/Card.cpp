#include "Card.h"
#include "Game.h"

int Card::SpawnActivation(void* pGame,
                          int   nCaster,
                          Activation* pActivation)
{
    int i      = 0;
    int nIndex = 0;
#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(pGame);
#endif

    // Iterate through activations and find one that is null
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (pTheGame->m_arActivations[i] == 0)
        {
            nIndex = i;
            pTheGame->m_arActivations[i] = pActivation;
            pActivation->OnCreate(pGame,
                                  nIndex,
                                  nCaster);
            return nIndex;
        }
    }

    return -1;
}