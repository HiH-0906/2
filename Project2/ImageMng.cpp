#include <DxLib.h>
#include "ImageMng.h"

std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::s_Instance(new ImageMng);

const std::vector<int>& ImageMng::GetID(const std::string& key)
{
	return GetID(key, key);
}

const std::vector<int>& ImageMng::GetID(const std::string& key, const std::string& fileName)
{
	// ‚·‚Å‚É‚ ‚é‚©Áª¯¸
	if (imgMap_.find(key) == imgMap_.end())
	{
		// ì¬
		imgMap_[key].resize(1);
		imgMap_[key][0] = LoadGraph(fileName.c_str(),true);
	}
	return imgMap_[key];
}

const std::vector<int>& ImageMng::GetID(const std::string& key, const std::string& fileName, const Vector2& divSize, const Vector2& divCnt)
{
	// ‚·‚Å‚É‚ ‚é‚©Áª¯¸
	if (imgMap_.find(key) == imgMap_.end())
	{
		// ì¬
		imgMap_[key].resize(divCnt.x * divCnt.y);
		LoadDivGraph(fileName.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap_[key][0]);
	}
	return imgMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
