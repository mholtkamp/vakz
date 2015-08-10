#ifndef SM_MENU_H
#define SM_MENU_H

enum MenuStateEnum
{
    MENU_STATE_LOADING  = 0,
    MENU_STATE_MAIN     = 1,
    MENU_STATE_REGISTER = 2,
    MENU_STATE_LOGIN    = 3,
    MENU_STATE_DECK     = 4,
    MENU_STATE_QUEUE    = 5,
    MENU_STATE_SHOP     = 6
};

class Menu
{
public:

    Menu();
    ~Menu();

    void Update();

private:

    int m_nState;

};

#endif