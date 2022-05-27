#pragma once
#include "common.h"
#include "Game.h"
class Tutorial : public App::Scene,public Game
{
public:
	Tutorial(const InitData& init);
	void update()override;
	void draw() const override;
private:


};

