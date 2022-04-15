#pragma once
#include "common.h"
class Result : public App::Scene
{
public:
	Result(const InitData& init);
	void update()override;
	void draw()const override;
private:

};

