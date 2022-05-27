#pragma once
#include "common.h"
class Caution :public App::Scene
{
public:
	Caution(const InitData& init);
	void update()override;
	void draw() const override;
private:
	Stopwatch timer{StartImmediately::Yes};
};

