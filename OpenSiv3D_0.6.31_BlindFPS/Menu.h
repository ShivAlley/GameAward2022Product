#pragma once
#include "common.h"
class Menu :public App::Scene
{
public:
	Menu(const InitData& init);
	void update()override;
	void draw() const override;
private:
	const Image image{ 1920, 1080, Color{ 0, 0, 0 } };
	const Texture texture{ image };
	const ColorF backgroundColor = ColorF{ 0, 0, 0 };
};

