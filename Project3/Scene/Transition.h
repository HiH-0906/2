#pragma once
#include "BaseScene.h"
class Transition :
    public BaseScene
{
public:
    Transition();
    virtual ~Transition();
    virtual uniqueBase Update(uniqueBase own)override;
    virtual void TransitionUpdate(void) = 0;
    virtual void Draw(void)override;
private:
protected:
    uniqueBase oldScene_;
    uniqueBase nextScene_;
};

