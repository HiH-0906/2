#pragma once
#include <vector>
#include <map>
#include <string>
#include "BaseScene.h"
#include "../State/CON_ID.h"
#include "../Input/Input.h"

class SelectScene :
	public BaseScene
{
public:
	SelectScene();
	~SelectScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	bool Setting(void);

	std::vector <std::map<CON_ID, std::shared_ptr<Input*>>> input_;				// “ü—Í¸×½
	std::vector<CON_ID> idVec_;
	std::vector<bool> start_;

	std::map<CON_ID, std::string> imageKey_;
};

