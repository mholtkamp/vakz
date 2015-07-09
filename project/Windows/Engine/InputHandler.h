#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class InputHandler
{

public:

    virtual void KeyDown(int nKey) = 0;

    virtual void KeyUp(int nKey) = 0;
};


#endif
