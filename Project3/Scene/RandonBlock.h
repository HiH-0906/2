#pragma once
#include "Transition.h"
class CheckeredBlock :
    public Transition
{
public:
    /// <summary>
    /// チェック柄的にトランジションする
    /// </summary>
    /// <param name="old">前のscene</param>
    /// <param name="next">次のscene</param>
    /// <param name="size">チェック柄分割数</param>
    CheckeredBlock(uniqueBase old, uniqueBase next,Vector2 size);
    ~CheckeredBlock();
    uniqueBase Update(uniqueBase own)override;
    void TransitionUpdate(void)override;
    void Draw(void)override;
    void Init(void)override;
    void DrawOwnScene(void)override;
private:
    int cnt_;
    Vector2 size_;
};

