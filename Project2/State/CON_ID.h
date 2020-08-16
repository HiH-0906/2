#pragma once
#include <type_traits>

enum class CON_ID
{
	KEY,				// ���ް��
	PAD,				// �ް��߯��
	MOUSE,				// ϳ�
	MAX
};

static CON_ID operator++(CON_ID& key) {
	(key = static_cast<CON_ID>(std::underlying_type<CON_ID>::type(key) + 1));
	if (key == CON_ID::MAX)
	{
		key = CON_ID::KEY;
	}
	return key;
};
