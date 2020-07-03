#include "PleyErea.h"

PleyErea::PleyErea() :_ereaSize({300,600})
{
}

PleyErea::~PleyErea()
{
}

void PleyErea::UpDate()
{
}

void PleyErea::Init(void)
{
	_input = std::make_unique<Input>(new Input());
}
