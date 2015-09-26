#include "Card.h"
#include "Game.h"

#if defined (SM_SERVER)
#include "../ShuffleMageServer/ServerGame.h"
#endif

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

void Card::DestroyDuplicateActivations(void* pGame,
                                       int   nCaster,
                                       Activation* pActivation)
{
    int i      = 0;

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(pGame);
#endif

    // Iterate through activations and find one that is null
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (pTheGame->m_arActivations[i] != 0 &&
            pTheGame->m_arActivations[i]->m_nType == pActivation->m_nType)
        {
            // Duplicate activation found, destroy the original.
            pTheGame->m_arActivations[i]->OnDestroy();
            delete pTheGame->m_arActivations[i];
            pTheGame->m_arActivations[i] = 0;
        }
    }
}

int Card::CheckForDuplicateActivation(void* pGame,
                                     int   nCaster,
                                     Activation* pActivation)
{
    int i      = 0;

#if defined (SM_CLIENT)
    Game* pTheGame = reinterpret_cast<Game*>(pGame);
#else
    ServerGame* pTheGame = reinterpret_cast<ServerGame*>(pGame);
#endif

    // Iterate through activations and find one that is null
    for (i = 0; i < MAX_ACTIVATIONS; i++)
    {
        if (pTheGame->m_arActivations[i] != 0 &&
            pTheGame->m_arActivations[i]->m_nType == pActivation->m_nType)
        {
            // At least one duplicate activation found.
            return 1;
        }
    }

    // No duplicate found
    return 0;
}
