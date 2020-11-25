#pragma once
#include <vector>
#include <array>
#include "Transition.h"

class CheckeredBlock :
    public Transition
{
public:
    /// <summary>
    /// �`�F�b�N���I�Ƀg�����W�V��������
    /// </summary>
    /// <param name="old">�O��scene</param>
    /// <param name="next">����scene</param>
    /// <param name="size">�`�F�b�N�������� �c��</param>
    CheckeredBlock(uniqueBase old, uniqueBase next,Vector2 size);
    ~CheckeredBlock();
    uniqueBase Update(uniqueBase own, NowTime time)override;
    void TransitionUpdate(void)override;
    void Draw(void)override;
    void Init(void)override;
    void DrawOwnScene(void)override;
private:
    void MakeBlockImage(void);

    int cnt_;
    int image_;
    std::vector<int> blockList_;
    Vector2 divCnt_;
};

