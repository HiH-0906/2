#pragma once
class PleyErea;

class playUnit
{
public:
	playUnit(PleyErea& erea);
	~playUnit();
	bool Update(void);
private:
	const PleyErea& playErea_;
};

