#pragma once
#include<map>
#include "Obj.h"
#include "State/INPUT_ID.h"

struct bit
{
    char left : 1;
    char right : 1;
    char up : 1;
    char down : 1;
    char rrota : 1;
    char lrota : 1;
};

union moveflag 
{
    char flag;
    bit bit;
};

class Puyo :
    public Obj
{
public:
    Puyo(Vector2Flt&& pos,float&& rad);
    ~Puyo();
    void Update(void);
    void Draw(void);
    const Vector2Flt GetMovePos(INPUT_ID id);
    void Move(const INPUT_ID& id);
private:
    moveflag _moveflag;
    std::map<INPUT_ID, Vector2Flt> _vec;
};

