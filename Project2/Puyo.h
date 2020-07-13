#pragma once
#include<map>
#include "Obj.h"
#include "State/INPUT_ID.h"


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
    bool _move;
    std::map<INPUT_ID, Vector2Flt> _vec;
};

