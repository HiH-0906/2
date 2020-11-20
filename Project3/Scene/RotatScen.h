#pragma once
#include "Transition.h"
class RotatScen :
	public Transition
{
public:
	RotatScen(uniqueBase old, uniqueBase next);
	~RotatScen();
	uniqueBase Update(uniqueBase own)override;
};

