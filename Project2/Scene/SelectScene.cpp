#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../ImageMng.h"
#include "../Input/keyState.h"
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../_debug/_DebugConOut.h"

SelectScene::SelectScene(std::vector<std::shared_ptr<PleyErea>>&& playErea)
{
	playErea_ = playErea;
	for (int i = 0; i < playErea_.size(); i++)
	{
		idVec_.emplace_back(CON_ID::KEY);
		start_.emplace_back(false);
	}
	lpImageMng.GetID("���[", "image/key.png");
	lpImageMng.GetID("�ς���", "image/pad.png");
	lpImageMng.GetID("�܂���", "image/mouse.png");
	imageKey_.try_emplace(CON_ID::KEY, "���[");
	imageKey_.try_emplace(CON_ID::PAD, "�ς���");
	imageKey_.try_emplace(CON_ID::MOUSE, "�܂���");
	FadeInit(false);
}

SelectScene::~SelectScene()
{
}

unipueBase SelectScene::Update(unipueBase own)
{
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		if (Setting())
		{
			// ������@�ʒm
			playErea_[0]->inputID(std::move(idVec_[0]));
			playErea_[1]->inputID(std::move(idVec_[1]));
			return std::make_unique<GameScene>(std::move(playErea_));
		}
	}
	// ���ݑI�𒆑�����@�m�F�p
	lpSceneMng.AddDrawList({ {128,128},IMAGE_ID(imageKey_[idVec_[0]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {630,128},IMAGE_ID(imageKey_[idVec_[1]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	return std::move(own);
}

bool SelectScene::Setting(void)
{
	// ��ڲ԰�l������
	for (int i = 0; i < playErea_.size(); i++)
	{
		// ������ڲ԰�������ԂȂ�X�V���Ȃ�
		if (start_[i])
		{
			continue;
		};
		// Input�̱����ްčX�V
		for (auto id : CON_ID())
		{
			(*playErea_[i]->GetInput()[id])->Update();
			// �ς��₷���悤�ɂ�������int��
			int tmpid = static_cast<int>(idVec_[i]);
			// �c�{�����@�I���ͷ��ް�ނ�
			if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::LEFT))
			{
				--tmpid;
				if (tmpid < 0)
				{
					tmpid = 2;
				}
				TRACE("PL%d:%d\n", i + 1, tmpid);
			}
			if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::RIGHT))
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
						// �ق��̐l�Ɣ���ĂȂ��� ���󂾂Ɠ�l��ڲ�z��
						if (playErea_[i ^ 1]->padNum() != j)
						{
							playErea_[i]->padNum(j);
							start_[i] = true;
						}
						break;
					}
				}
			}
			else if(idVec_[i] == CON_ID::MOUSE)
			{
				if (idVec_[i ^ 1] == CON_ID::MOUSE)
				{
					return false;
				}
				// �I�΂�Ă�����۰װ�����݂Ō���
				if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::LROTA))
				{
					start_[i] = true;
				}
			}
		}
	}
	// ��l�Ƃ�����������true
	return start_[0] && start_[1];
}
