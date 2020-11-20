#include "RotatScen.h"

RotatScen::RotatScen(uniqueBase old, uniqueBase next)
{
	oldScene_ = std::move(old);
	nextScene_ = std::move(next);
}

RotatScen::~RotatScen()
{
}

uniqueBase RotatScen::Update(uniqueBase own)
{
	return uniqueBase();
}
