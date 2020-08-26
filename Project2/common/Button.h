#pragma once
#include <memory>
#include <functional>
#include "Vector2.h"

using ButtonFunc = std::function<bool(void)>;

class Button
{
public:
	Button();
	Button(Vector2&& pos, Vector2&& size, ButtonFunc&& func);
	~Button();
	void CheckHitButton(Vector2& pos);
private:

protected:
	// ���W�֌W
	Vector2 pos_;
	Vector2 size_;

	// ���݉��������s�p�֐�
	ButtonFunc func_;
};

using sharedButton = std::shared_ptr<Button>;