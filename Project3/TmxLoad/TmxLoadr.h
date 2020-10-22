#pragma once

#include <string>
#include <vector>
#include <map>
#include "../common/Vector2.h"
#include "../RapidXml/rapidxml.hpp"

struct Layer
{
	std::string name;
	std::string data;
};

struct mapInfo
{
	Vector2 mapSize = Vector2{};
	Vector2 chipSize = Vector2{};
	int allNum = 0;
	int columnsNum = 0;
	int lineNum = 0;
	std::string key = std::string{};
};

using mapStr = std::vector<Layer>;

namespace Loader
{
	class TmxLoadr
	{
	public:
		TmxLoadr();
		TmxLoadr(const char* filename);
		bool TmxLoad(std::string filename);			// Tmx�t�@�C�����[�h�p ������TsxLoad���Ă�ł���
		const mapStr GetmapStr(void);				// ���[�h���Ă���string��n���z
		const std::string GetMapKey(void);			// Tsx���[�h���쐬���ꂽImage�փA�N�Z�X���邽�߂̃L�[
		const mapInfo GetMapInfo(void);				// �}�b�v�쐬���ɕK�v�ȏ��Z�b�g
		~TmxLoadr();
	private:
		void VersionMap(void);						// �Ή��o�[�W�����i�[���邽�߂����̓z
		int  GetLayerSize(void);					// ���C���[�̍ő吔�l�����邾���̓z
		bool TsxLoad(std::string filename);			// Tsx�t�@�C�������[�h TmxLoad����Ăяo�����

		rapidxml::xml_document<> doc_;
		rapidxml::xml_node<>* orign_node_;			// ��ԍŏ��̃m�[�h ��{�I�ɂ�������A�N�Z�X����

		std::map<std::string, int> version_;		// ����ʃo�[�W�����ɑΉ������ۂ����ɑΉ������o�[�W�������L�[�ɂ��ēK���Ȓl��������Ƃ��ΑΉ����Ă邩�ǂ����m�F����Ƃ��Ɋy
		mapStr mapStr_;								// �ǂݍ���string�̕ۑ��ꏊ
		mapInfo info_;								// �ǂݍ��񂾃}�b�v�쐻�p�����g���₷���`�ɕς��i�[��������
	};
}

