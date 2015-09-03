#include "Effect.h"

Effect::Effect()
{
    m_nType    = EFFECT_NO_TYPE;
    m_nEnabled = 1;
}

Effect::~Effect()
{

}

int Effect::IsEnabled()
{
    return m_nEnabled;
}

void Effect::SetEnable(int nEnable)
{
    m_nEnabled = nEnable;
}

int Effect::GetType()
{
    return m_nType;
}
