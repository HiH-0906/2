#pragma once
#include<utility>
#include<array>
#include<map>
#include<DxLib.h>
#include"../State/CON_ID.h"
#include"../State/INPUT_ID.h"


using InputData = std::map<INPUT_ID, std::pair<bool, bool>>;

using keyPair = std::pair<INPUT_ID, int>;
using keyData = std::array<keyPair,static_cast<size_t>(INPUT_ID::MAX)>;


class Input
{
public:
	Input();
	virtual void Update(const int& p_id,const int& pad_id) = 0;					// Update�p����
	virtual CON_ID GetID(void) = 0;								// ���۰װ��ʎ擾�p
	virtual void Setting(void);									// ��������ʾèݸޕK�v�ȏꍇ�̂݌p����ɋL�q
	bool GetKeyTrg(INPUT_ID key);								// �w��ID���ضް����
	bool GetKeySty(INPUT_ID key);								// �w��ID�̂������ϔ���
private:
	void Init(void);											// ������
protected:
	InputData _input;											// �����ް��i�[��
};

