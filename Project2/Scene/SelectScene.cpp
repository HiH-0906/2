#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../ImageMng.h"
#include "../Input/keyState.h"
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../_debug/_DebugConOut.h"

SelectScene::SelectScene()
{
	input_.resize(lpSceneMng.playNum());
	for (int i = 0; i < lpSceneMng.playNum(); i++)
	{
		input_[i].try_emplace(CON_ID::KEY, std::make_shared<Input*>(new keyState));
		input_[i].try_emplace(CON_ID::PAD, std::make_shared<Input*>(new PadState));
		input_[i].try_emplace(CON_ID::MOUSE, std::make_shared<Input*>(new MouseState));
		idVec_.emplace_back(CON_ID::KEY);
		start_.emplace_back(false);
	}
	lpImageMng.GetID("���[", "image/key.png");
	lpImageMng.GetID("�ς���", "image/pad.png");
	lpImageMng.GetID("�܂���", "image/mouse.png");
	imageKey_.try_emplace(CON_ID::KEY, "���[");
	imageKey_.try_emplace(CON_ID::PAD, "�ς���");
	imageKey_.try_emplace(CON_ID::MOUSE, "�܂���");
}

SelectScene::~SelectScene()
{
}

unipueBase SelectScene::Update(unipueBase own)
{
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	
	if (Setting())
	{
		// ������@�ʒm
		lpSceneMng.playErea()[0]->inputID(std::move(idVec_[0]));
		lpSceneMng.playErea()[1]->inputID(std::move(idVec_[1]));
		return std::make_unique<GameScene>();
	}
	// ���ݑI�𒆑�����@�m�F�p
	lpSceneMng.AddDrawList({ {128,128},IMAGE_ID(imageKey_[idVec_[0]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {630,128},IMAGE_ID(imageKey_[idVec_[1]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	return std::move(own);
}

bool SelectScene::Setting(void)
{
	// ��ڲ԰�l������
	for (int i = 0; i < lpSceneMng.playNum(); i++)
	{
		// ������ڲ԰�������ԂȂ�X�V���Ȃ�
		if (start_[i])
		{
			continue;
		};
		// Input�̱����ްčX�V
		for (auto id : CON_ID())
		{
			(*input_[i][id])->Update(i, 0);
		}
		// �ς��₷���悤�ɂ�������int��
		int tmpid = static_cast<int>(idVec_[i]);
		// �c�{�����@�I���ͷ��ް�ނ�
		if ((*input_[i][CON_ID::KEY])->GetKeyTrg(INPUT_ID::LEFT))
		{
			--tmpid;
			if (tmpid < 0)
			{
				tmpid = 2;
			}
			TRACE("PL%d:%d\n", i + 1, tmpid);
		}
		if ((*input_[i][CON_ID::KEY])->GetKeyTrg(INPUT_ID::RIGHT))
		{
			++tmpid;
			if (tmpid >= 3)
			{
				tmpid = 0;
			}
			TRACE("PL%d:%d\n", i + 1, tmpid);
		}
		idVec_[i] = static_cast<CON_ID>(tmpid);
		// �I�����ꂽ������@���߯�ނ̏ꍇ�ǂ̺��۰װ���N�Ȃ̂����ʂ���K�v��������̂ŕ���
		if (idVec_[i] == CON_ID::PAD)
		{
			// �ڑ�����Ă��Ȃ��ꍇ�̑ҋ@
			if (lpSceneMng.fCnt() / 30 % 2)
			{
				ReSetupJoypad();
			}
			for (int j = 1; j <= GetJoypadNum(); j++)
			{
				// ��l�Ƃ��߯�ނ̏ꍇ��ɉ������l��1P�ɂȂ����Ⴄ
				if ((GetJoypadInputState(j) & PAD_INPUT_1))
				{
					// �ق��̐l�Ɣ���ĂȂ��� ���󂾓�l��ڲ�z��
					if (lpSceneMng.playErea()[i ^ 1]->padNum() != j)
					{
						lpSceneMng.playErea()[i]->padNum(j);
						start_[i] = true;
					}
					break;
				}
			}
		}
		else
		{
			// �I�΂�Ă�����۰װ�����݂Ō���
			if ((*input_[i][idVec_[i]])->GetKeyTrg(INPUT_ID::LROTA))
			{
				start_[i] = true;
			}
		}
	}
	// ��l�Ƃ�����������true
	return start_[0] && start_[1];
}
