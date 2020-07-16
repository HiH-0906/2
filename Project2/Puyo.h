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

class Puyo 
{
public:
    Puyo(Vector2&& pos,int&& rad);
    ~Puyo();
    void Update(void);
    void Draw(void);
    void Move(const INPUT_ID& id);
    bool dirpermit(DirPermit dirpermit);
    const Vector2& pos(void);
    const Vector2 GetGrid(int size);
    PUYO_ID& id(void);
private:
    DirPermit dirpermit_;
    std::map<INPUT_ID, Vector2> vec_;
    PUYO_ID id_;
    Vector2 pos_;
    int rad_;
};

