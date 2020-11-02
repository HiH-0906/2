#pragma once
#include "Transition.h"
class CrossOver :
    public Transition
{
public:
    CrossOver(uniqueBase old, uniqueBase next);
    ~CrossOver();
    uniqueBase Update(uniqueBase own)override;
    void Draw(void)override;
    void Init(void)override;
    void DrawOwnScene(void)override;
    void TransitionUpdate(void)override;
private:
    int cnt_;
};

