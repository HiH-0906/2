#pragma once
#include <memory>
#include <functional>
#include "Vector2.h"

using ButtonFunc = std::function<bool(Vector2& pos)>;

class Button
{
public:
	Button();
	Button(Vector2&& pos, Vector2&& size, ButtonFunc&& func);
	~Button();
	void Update(Vector2& pos);
private:

protected:
	// 座標関係
	Vector2 pos_;
	Vector2 size_;

	// ﾎﾞﾀﾝ押下時実行用関数
	ButtonFunc func_;

	bool CheckHitButton(Vector2& pos, bool flag);
};

using sharedButton = std::shared_ptr<Button>;