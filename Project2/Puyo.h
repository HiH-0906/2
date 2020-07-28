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
class Puyo;
using puyoColor = std::map<PUYO_ID, int>;
using PuyoUnit = std::shared_ptr<Puyo>;
class Puyo 
{
public:
    Puyo(Vector2&& pos,PUYO_ID id);
    ~Puyo();
    bool Update(void);
    void Draw(std::vector<PuyoUnit*> list);
    void Move(const INPUT_ID& id);
    void SetMunyonBit(DirPermit dirpermit);
    bool dirpermit(DirPermit dirpermit);
    bool GetDownNow(void);
    const DirPermit dirpermit(void);
    const Vector2& pos(void);
    void pos(Vector2&& pos);
    const Vector2 GetGrid(int size);
    PUYO_ID& id(void);
    void SetSoftDrop(void);
    void SetPuyonCnt(void);
    void SetMuyonCnt(void);
    bool CheckPuyonCnt(void);
    bool CheckMuyonCnt(void);
    void ChengeSpeed(int speed,int cnt);
    bool activ(void);
    void activ(bool flag);
    const DirPermit pairMit(void);
    void pairMit(DirPermit pairMit);
private:
    static puyoColor colorList_;
    int puyonCnt_;
    int puyonMax_;
    int muyonCnt_;
    int muyonMax_;
    int downCnt_;
    int downNum_;
    DirPermit dirpermit_;
    DirPermit oldDirpermit_;
    DirPermit munyonmit_;
    DirPermit pairMit_;
    std::map<INPUT_ID, Vector2> vec_;
    PUYO_ID id_;
    Vector2 pos_;
    int rad_;
    bool activ_;
};

