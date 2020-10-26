#pragma once
#include <utility>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Vector2.h"

using ImgMap = std::map<std::string, std::vector<int>>;

#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *s_Instance;
	};
	const std::vector<int>& GetID(const std::string& key);										// ゲーム内検索用
	const std::vector<int>& GetID(const std::string& key, const std::string& fileName);			// 一枚絵用
	const std::vector<int>& GetID(const std::string& key, const std::string& fileName,			// 分割用
		const Vector2& divSize, const Vector2& divCnt);

private:
	ImgMap imgMap_;

	ImageMng();
	~ImageMng();
	struct ImageMngDeleter
	{
		void operator()(ImageMng* mng)
		{
			delete mng;
		}
	};
	static std::unique_ptr<ImageMng, ImageMngDeleter> s_Instance;
};

