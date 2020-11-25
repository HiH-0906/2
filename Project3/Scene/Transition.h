#pragma once
#include "BaseScene.h"
class Transition :
    public BaseScene
{
public:
    Transition();
    virtual ~Transition();
    virtual void TransitionUpdate(void) = 0;
private:
protected:
    uniqueBase oldScene_;
    uniqueBase nextScene_;
};

