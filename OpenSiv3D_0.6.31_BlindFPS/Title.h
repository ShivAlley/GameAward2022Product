#pragma once
#include "common.h"
class Title :
	public App::Scene
{
public:
	Title(const InitData& init);
	void update()override;
	void draw() const override;
private:
	const Image image{ 1920, 1080, Color{ 0, 0, 0 } };
	const Texture texture{ image };
	const ColorF backgroundColor = ColorF{ 0, 0, 0 };
	Texture titleName{ U"Image/TITLE.png" };
	Texture START{ U"Image/START.png" };
	Texture mouse{ U"Image/mouse.png" };
	Texture exit{ U"Image/EXIT.png" };
protected:
	int menu = 0;
	int  option = 0;
	
};

