#pragma once
#include<map>
#include"../common/Vector2.h"
#include "../State/PUYO_ID.h"
#include "../State/INPUT_ID.h"

// 上下左右ﾋﾞｯﾄﾌｨｰﾙﾄﾞ
struct DirBit
{
    unsigned int up : 1;
    unsigned int down : 1;
    unsigned int left : 1;
    unsigned int right : 1;
};
// 方向判別用共用体
union DirPermit 
{
    unsigned int per;
    DirBit perbit;
};

using puyoColor = std::map<PUYO_ID, int>;
class Puyo 
{
public:
    Puyo();
    Puyo(Vector2&& pos,PUYO_ID id);
    virtual ~Puyo();
    bool Update(void);
    void Draw(int cnt);
    void Move(const INPUT_ID& id);
    virtual void SetMunyonBit(DirPermit dirpermit);
    bool dirpermit(DirPermit dirpermit);
    bool GetDownNow(void);
    const DirPermit dirpermit(void);
    const Vector2& pos(void);
    void pos(Vector2&& pos);
    const Vector2 GetGrid(int size);
    PUYO_ID& id(void);
    void SetSoftDrop(void);
    virtual bool SetPuyonCnt(void);             // ぷよーんｶｳﾝﾄｾｯﾄ 継承先でぷよーんさせたくないぷよの場合return false
    virtual void SetMuyonCnt(void);
    bool CheckPuyonCnt(void);
    bool CheckMuyonCnt(void);
    void ChengeSpeed(int speed,int cnt);
    bool activ(void);
    void activ(bool flag);
    void playPuyo(bool flag);
    int GetColor(void);
protected:
    static puyoColor colorList_;
    int puyonCnt_;
    int puyonMax_;
    int muyonCnt_;
    int muyonMax_;
    int downCnt_;
    int downNum_;
    bool playPuyo_;                              // 操作の中心かどうか
    DirPermit dirpermit_;
    DirPermit oldDirpermit_;
    DirPermit munyonmit_;
    std::map<INPUT_ID, Vector2> vec_;
    PUYO_ID id_;
    Vector2 pos_;
    int rad_;
    bool activ_;
};

