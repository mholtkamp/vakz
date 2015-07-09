#include "VInput.h"
#include "Log.h"
#include <string.h>

static int s_arKeys[VINPUT_MAX_KEYS];

void SetKey(int nKey)
{
    s_arKeys[nKey] = 1;
}

void ClearKey(int nKey)
{
    s_arKeys[nKey] = 0;
}

void ClearAllKeys()
{
    memset(s_arKeys, 0, VINPUT_MAX_KEYS * sizeof(int));
}

int IsKeyDown(int nKey)
{
    if (nKey >= 0 &&
        nKey <  VINPUT_MAX_KEYS)
    {
        return s_arKeys[nKey];
    }
    else
    {
        LogWarning("Invalid key queried in IsKeyDown().");
        return 0;
    }
}
