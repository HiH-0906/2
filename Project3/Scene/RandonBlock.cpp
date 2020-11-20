#include <DxLib.h>
#include "RandonBlock.h"

CheckeredBlock::CheckeredBlock(uniqueBase old, uniqueBase next, Vector2 size) :size_(size)
{
	oldScene_ = std::move(old);
	nextScene_ = std::move(next);
	cnt_ = 0;
}

CheckeredBlock::~CheckeredBlock()
{
}

uniqueBase CheckeredBlock::Update(uniqueBase own)
{
	return uniqueBase();
}

void CheckeredBlock::TransitionUpdate(void)
{
}

void CheckeredBlock::Draw(void)
{
	DrawGraph(0, 0, drawScreen_, true);
}

void CheckeredBlock::Init(void)
{
}

void CheckeredBlock::DrawOwnScene(void)
{
}
