#pragma once
#include <memory>
#include <functional>
#include "Vector2.h"

using ButtonFunc = std::function<bool(Vector2& pos)>;

class Button
{
public:
	Button(Vector2&& pos, Vector2&& size);
	~Button();
	bool Update(Vector2& pos,bool flag);
	void SetButtonFunc(ButtonFunc&& func);

private:
	void Draw(void);
	int screen;
protected:
	int color;

	// ���W�֌W
	Vector2 pos_;
	Vector2 size_;

	// ���݉��������s�p�֐�
	ButtonFunc func_;

	bool CheckHitButton(Vector2& pos);
};

using sharedButton = std::shared_ptr<Button>;