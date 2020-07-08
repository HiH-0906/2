#pragma once
#include "Obj.h"
class Puyo :
    public Obj
{
public:
    Puyo(Vector2Flt&& pos,float&& rad);
    ~Puyo();
    void Update(void);
    void Draw(void);
    void Move(const Vector2Flt& vec);
private:
    void Init(Vector2Flt&& pos, float&& rad);
};

