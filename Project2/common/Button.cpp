#include "Button.h"

Button::Button()
{
}

Button::Button(Vector2&& pos, Vector2&& size, ButtonFunc&& func)
{
	pos_ = pos;
	size_ = size;
	func_ = func;
}

Button::~Button()
{
}

void Button::CheckHitButton(Vector2& pos)
{
	// ‚ ‚Á‚Ä‚écH
	if (pos.x > pos_.x && pos.x < (pos_.x + size_.x) && pos.y > pos_.y && pos.y < (pos_.y + size_.y))
	{
		func_();
	}
}

