#include <DxLib.h>
#include "NumPad.h"
#include "ImageMng.h"
#include "../Scene/BaseScene.h"


NumPad::NumPad(const Vector2& pos, BaseScene& scene) :rect_(pos, { 132,196 }), scene_(scene)
{
	currentPos_ = {};
	drawPadScreen_ = MakeScreen(rect_.size.w, rect_.size.h, true);
	drawNumScreen_ = MakeScreen(rect_.size.w * 15, rect_.size.h, true);
	lpImageMng.GetID("Num", "Image/Number.png", { 33,49 }, { 5,3 });
	numPadStr_.emplace_back(std::pair<std::string, int>{ "7", 7});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "8", 8});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "9", 9});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "BS", 12});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "4", 4});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "5", 5});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "6", 6});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "BS", 12});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "1", 1});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "2", 2});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "3", 3});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "EN", 12});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "C", 11});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "0", 0});
	numPadStr_.emplace_back(std::pair<std::string, int>{ ".", 10});
	numPadStr_.emplace_back(std::pair<std::string, int>{ "EN", 12});
	init();
}

NumPad::~NumPad()
{
}

bool NumPad::UpDate(const Vector2& pos, bool flag)
{
	if (rect_.IsHitWithPoint(pos))
	{
		auto hitPos = pos - rect_.pos;
		hitPos.x /= 33;
		hitPos.y /= 49;
		currentPos_ = { hitPos.x * 33,hitPos.y * 49 };
		if (flag)
		{
			auto cursor = static_cast<__int64>(hitPos.x) + (static_cast<__int64>(hitPos.y) * 4);
			
			if(numPadStr_[cursor].first == "C")
			{
				inputNum_.clear();
			}
			else if (numPadStr_[cursor].first == "BS")
			{
				if (inputNum_.size()!= 0)
				{
					inputStr_.pop_back();
					inputNum_.pop_back();
				}
			}
			else if (numPadStr_[cursor].first == "EN")
			{
				return true;
			}
			else
			{
				inputStr_ += numPadStr_[cursor].first;
				inputNum_.emplace_back(numPadStr_[cursor].second);
			}
		}
	}
	Draw();
	return false;
}

void NumPad::Draw(void)
{
	SetDrawScreen(drawPadScreen_);
	ClsDrawScreen();

	Vector2 drawPos = { 0,rect_.size.h - 49 };

	DrawGraph(drawPos.x, drawPos.y, lpImageMng.GetID("Num")[11], true);
	DrawString(drawPos.x + 8, drawPos.y + 17, "AC", 0xffff00);
	drawPos.x += 33;
	DrawGraph(drawPos.x, drawPos.y, lpImageMng.GetID("Num")[0], true);
	drawPos.x += 33;
	DrawGraph(drawPos.x, drawPos.y, lpImageMng.GetID("Num")[10], true);

	drawPos = { 0,drawPos.y - 49 };

	for (int i = 1; i <= 9; i++)
	{
		DrawGraph(drawPos.x, drawPos.y, lpImageMng.GetID("Num")[i], true);
		drawPos.x += 33;
		if (i % 3 == 0)
		{
			drawPos = { 0,drawPos.y - 49 };
		}
	}
	Vector2 boxSize(33, 49);

	if (currentPos_.x >= 99)
	{
		currentPos_.y /= 98;
		currentPos_.y *= 98;
		boxSize.y *= 2;
	}

	DrawBox(currentPos_.x, currentPos_.y, currentPos_.x + boxSize.x, currentPos_.y + boxSize.y, 0x55ff55, false);


	// âÊëúÇ™Ç»Ç¢ÇΩÇﬂâºÉRÅ[Éh
	DrawString(112, 33, "B\nS", 0x66ff66);
	DrawString(112, 131, "E\nN", 0x66ff66);

	scene_.AddDrawQue(DrawQue{ rect_.pos, {rect_.size.w,rect_.size.h},1.0,0,drawPadScreen_,0 });

	SetDrawScreen(drawNumScreen_);
	ClsDrawScreen();
	Vector2 numPos = { 0,0 };
	for (auto num : inputNum_)
	{
		DrawGraph(numPos.x, numPos.y, lpImageMng.GetID("Num")[num], true);
		numPos.x += 33;
	}

	scene_.AddDrawQue(DrawQue{ {100,100}, {rect_.size.w * 15,rect_.size.h},1.0,0,drawNumScreen_,0 });
}

const std::string& NumPad::GetInputStr(void) const
{
	return inputStr_;
}

void NumPad::init(void)
{
	inputStr_.clear();
	inputStr_ = "";
	inputNum_.clear();
}
