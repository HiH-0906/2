#pragma once
#include<utility>
#include<memory>
#include"Puyo/Puyo.h"
#include"PleyErea.h"

using NextPair = std::pair<sharPuyo, sharPuyo>;
using NextList = std::list<NextPair>;

class NextPuyoCtl
{
public:
	/// <summary>
	/// NextPuyo�Ǘ��p�׽
	/// </summary>
	/// <param name="pos">�`�掞�ʒu</param>
	/// <param name="haveCount">�������ް�������I�ɕێ����邩</param>
	/// <param name="drawCount">�`��� �Ȃ��`��͈͂̊֌W��2�ڂ̔����܂ł��������Ȃ�</param>
	NextPuyoCtl(Vector2& pos,int haveCount,int drawCount);
	~NextPuyoCtl();
	void Draw(void);
	NextPair PickUp(void);				// NextPuyoList����1��Ď��o���āA1��ĕ�[

	bool Add(int no);					// next�������I�Ɏw�辯Đ���[ PickUp�œ����I�ɌĂ�ł���̂Ŏ����I�ɌĂԂ��Ƃ͂Ȃ�����

private:
	Vector2 pos_;						// �`��ʒu
	NextList nextPuyoList_;				// NextPuyo��ؽ�
	int drawCount_;						// �����`�悷�邩
	int screenID_;
};

