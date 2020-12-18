#pragma once
#include <vector>
#include <array>
#include "Transition.h"

class CheckeredBlock :
    public Transition
{
public:
    /// <summary>
    /// ���ォ��؂�ւ��
    /// </summary>
    /// <param name="old">�O��scene</param>
    /// <param name="next">����scene</param>
    CheckeredBlock(uniqueBase old, uniqueBase next);
    ~CheckeredBlock();
    uniqueBase Update(uniqueBase own, const Time& now)override;
    void TransitionUpdate(void)override;
    void Draw(void)override;
    void Init(void)override;
    void DrawOwnScene(void)override;
private:
    void MakeBlockImage(const int& image, std::vector<int>& list);

    int cnt_;
    int nextImage_;
    int oldImage_;
    std::vector<int> nextBlockList_;
    std::vector<int> oldBlockList_;
    Vector2 divCnt_;
};

