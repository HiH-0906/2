#pragma once
#include<map>
#include "Obj.h"
#include "State/PUYO_ID.h"
#include "State/INPUT_ID.h"

// è„â∫ç∂âEÀﬁØƒÃ®∞Ÿƒﬁ
struct DirBit
{
    unsigned int up : 1;
    unsigned int down : 1;
    unsigned int left : 1;
    unsigned int right : 1;
};

union DirPermit 
{
    unsigned int per;
    DirBit perbit;
};

using puyoColor = std::map<PUYO_ID, int>;

class Puyo 
{
public:
    Puyo(Vector2&& pos,PUYO_ID id);
    ~Puyo();
    bool Update(void);
    void Draw(void);
    void Move(const INPUT_ID& id);
    void SetMunyonBit(DirPermit dirpermit);
    bool dirpermit(DirPermit dirpermit);
    const Vector2& pos(void);
    void pos(Vector2&& pos);
    const Vector2 GetGrid(int size);
    PUYO_ID& id(void);
    void SetSoftDrop(void);
    void SetPuyonCnt(void);
    void SetMuyonCnt(void);
    bool CheckMuyonCnt(void);
    void ChengeSpeed(int speed,int cnt);
    bool activ(void);
    void activ(bool flag);
private:
    static puyoColor colorList_;
    int puyonCnt_;
    int puyonMax_;
    int muyonCnt_;
    int muyonMax_;
    int downCnt_;
    int downNum_;
    DirPermit dirpermit_;
    DirPermit munyonmit_;
    std::map<INPUT_ID, Vector2> vec_;
    PUYO_ID id_;
    Vector2 pos_;
    int rad_;
    bool activ_;
};

