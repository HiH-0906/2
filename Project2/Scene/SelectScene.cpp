#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../ImageMng.h"
#include "../SoundMng.h"
#include "../Input/keyState.h"
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../_debug/_DebugConOut.h"

SelectScene::SelectScene()
{
	for (int i = 0; i < 2; i++)
	{
		idVec_.emplace_back(CON_ID::KEY);
		start_.emplace_back(false);
	}
	lpImageMng.GetID("���[1", "image/KeyPL1.png");
	lpImageMng.GetID("���[2", "image/KeyPL2.png");
	lpImageMng.GetID("�ς���", "image/pad.png");
	lpImageMng.GetID("�܂���", "image/mouse.png");
	lpImageMng.GetID("�܂���2", "image/mousePL2.png");
	lpImageMng.GetID("��������", "image/����.png");
	lpImageMng.GetID("SelectBG", "image/SelectBG.png");
	lpImageMng.GetID("����", "image/����.png");
	imageKey_[0].try_emplace(CON_ID::KEY, "���[1");
	imageKey_[0].try_emplace(CON_ID::PAD, "�ς���");
	imageKey_[0].try_emplace(CON_ID::MOUSE, "�܂���");
	imageKey_[1].try_emplace(CON_ID::KEY, "���[2");
	imageKey_[1].try_emplace(CON_ID::PAD, "�ς���");
	imageKey_[1].try_emplace(CON_ID::MOUSE, "�܂���2");
	
	for (size_t i = 0; i < input_.size(); i++)
	{
		input_[i] = std::make_unique<keyState>();
		input_[i]->Setting(i, i);
		padNum_[i] = 0;
	}
	game_ = false;
	FadeInit(false);
}

SelectScene::~SelectScene()
{
}

unipueBase SelectScene::Update(unipueBase own)
{
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	// ���ݑI�𒆑�����@�m�F�p
	lpSceneMng.AddDrawList({ {256,384},IMAGE_ID(imageKey_[0][idVec_[0]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {768,384},IMAGE_ID(imageKey_[1][idVec_[1]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		if (game_)
		{
			for (size_t i = 0; i < start_.size(); i++)
			{
				if (start_[i])
				{
					lpSceneMng.AddDrawList({ { static_cast<int>(256 + (512 * i)),384 }, IMAGE_ID("��������")[0], 1.0, 0.0, 1, SCREEN_ID::FRONT, DATA_TYPE::IMG, true });
					lpSceneMng.AddDrawList({ { static_cast <int>(256 + (512 * i)),384 }, IMAGE_ID("SelectBG")[0], 1.0, 0.0, 1, SCREEN_ID::PLAY, DATA_TYPE::IMG, true });
				}
			}
			// ������@�ʒm
			playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 0,0 }, idVec_[0]));
			playErea_[0]->padNum( padNum_[0]);
			playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 512, 0 }, idVec_[1]));
			playErea_[1]->padNum(padNum_[1]);
			return std::make_unique<GameScene>(std::move(playErea_));
		}
		game_ = Setting();
		for (size_t i = 0; i < start_.size(); i++)
		{
			if (start_[i])
			{
				lpSceneMng.AddDrawList({ {static_cast <int> (256 + (512 * i)),384 }, IMAGE_ID("��������")[0], 1.0, 0.0, 1, SCREEN_ID::FRONT, DATA_TYPE::IMG, true });
				lpSceneMng.AddDrawList({ { static_cast <int>(256 + (512 * i)),384 }, IMAGE_ID("SelectBG")[0], 1.0, 0.0, 1, SCREEN_ID::PLAY, DATA_TYPE::IMG, true });
			}
		}
	}
	
	return std::move(own);
}

bool SelectScene::Setting(void)
{
	// �ڑ�����Ă��Ȃ��ꍇ�̑ҋ@
	if (lpSceneMng.fCnt() / 60 % 2)
	{
		ReSetupJoypad();
	}
	// ��ڲ԰�l������
	for (int i = 0; i < 2; i++)
	{
		// ������ڲ԰�������ԂȂ�X�V���Ȃ�
		if (start_[i])
		{
			continue;
		};
		// Input�̱����ްčX�V
		input_[i]->Update();
		// �ς��₷���悤�ɂ�������int��
		int tmpid = static_cast<int>(idVec_[i]);
		// �c�{�����@�I���ͷ��ް�ނ�
		if (input_[i]->GetKeyTrg(INPUT_ID::LEFT))
		{
			--tmpid;
			if (tmpid < 0)
			{
				tmpid = 2;
			}
			TRACE("PL%d:%d\n", i + 1, tmpid);
		}
		if (input_[i]->GetKeyTrg(INPUT_ID::RIGHT))
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
			for (int j = 1; j <= GetJoypadNum(); j++)
			{
				// ��l�Ƃ��߯�ނ̏ꍇ��ɉ������l��1P�ɂȂ����Ⴄ
				if ((GetJoypadInputState(j) & PAD_INPUT_2))
				{
					// �ق��̐l�Ɣ���ĂȂ��� ���󂾂Ɠ�l��ڲ�z��
					if (padNum_[i ^ 1] != j)
					{
						PlaySoundMem(lpSoundMng.GetHandle("����"), DX_PLAYTYPE_BACK);
						padNum_[i] = j;
						start_[i] = true;
					}
					break;
				}
			}
		}
		else if (idVec_[i] == CON_ID::MOUSE)
		{
			if (idVec_[i ^ 1] == CON_ID::MOUSE)
			{
				return false;
			}
		}
		else
		{
			// ���ް�ނ̏ꍇ�������Ȃ�
		}
		if (input_[i]->GetKeyTrg(INPUT_ID::RROTA))
		{
			PlaySoundMem(lpSoundMng.GetHandle("����"), DX_PLAYTYPE_BACK);
			start_[i] = true;
		}
	}
	// ��l�Ƃ�����������true
	return start_[0] && start_[1];
}
