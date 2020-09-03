#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../PleyErea.h"
#include "BaseScene.h"
#include "../State/CON_ID.h"
#include "../Input/Input.h"
#include "../Input/keyState.h"


class SelectScene :
	public BaseScene
{
public:
	SelectScene(void);
	~SelectScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	bool Setting(void);
	bool game_;

	std::array<std::unique_ptr<keyState>,2> input_;
	std::array <int, 2> padNum_;

	std::vector<CON_ID> idVec_;
	std::vector<bool> start_;

	std::array<std::map<CON_ID, std::string>,2> imageKey_;
	PlayEreaVec playErea_;
};

