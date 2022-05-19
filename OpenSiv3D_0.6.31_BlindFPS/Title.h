#pragma once
#include "common.h"
#define PI 3.14159265359
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
	Texture TutorialName{ U"Image/sample.png" };
	Texture Tutorial{ U"Image/tutorial.png" };
	Texture Option{ U"Image/OPTION.png" };
	Texture Exit{ U"Image/EXIT.png" };
	Texture mouse{ U"Image/mouse.png" };
protected:
	int menu = 0;
	int  option = 0;
	int tutorial = 0;
	Float2 pos1{ 1150,100 };
	Float2 pos3{ 1150,350 };
	Float2 pos2{ 1150,600 };
	Float2 pos4{ 1300,850 };
	Vec2    position{0,50};
	Vec2    speed;
	int timer = 0;

};

