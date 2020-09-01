#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../PleyErea.h"
#include "BaseScene.h"
#include "../State/CON_ID.h"
#include "../Input/Input.h"


class SelectScene :
	public BaseScene
{
public:
	SelectScene(std::vector<std::shared_ptr<PleyErea>>&& playErea);
	~SelectScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	bool Setting(void);

	std::vector<CON_ID> idVec_;
	std::vector<bool> start_;

	std::map<CON_ID, std::string> imageKey_;
	PlayEreaVec playErea_;
};

