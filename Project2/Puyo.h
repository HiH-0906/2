#pragma once
#include<map>
#include "Obj.h"
#include "State/INPUT_ID.h"

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

class Puyo :
    public Obj
{
public:
    Puyo(Vector2Flt&& pos,float&& rad);
    ~Puyo();
    void Update(void);
    void Draw(void);
    const Vector2Flt pos(void);
    void Move(const INPUT_ID& id);
    bool dirpermit(DirPermit dirpermit);
private:
    DirPermit _dirpermit;
    std::map<INPUT_ID, Vector2Flt> _vec;
};

