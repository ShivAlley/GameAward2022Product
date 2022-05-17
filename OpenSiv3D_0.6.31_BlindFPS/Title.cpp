#include "Title.h"
Title::Title(const InitData& init)
	:IScene(init)
{

}

void Title::update()
{
	menu = 0;
	if (START.region(1150,350).mouseOver()
		and MouseL.down())
	{
		changeScene(GameState::Game);
	}
	if (exit.region(1150, 600).mouseOver()
		and MouseL.down())
	{
		option = 1;
	}
	if (option == 1)
	{
		if (SimpleGUI::Button(U"決定", Vec2{ 1100, 600 }))
		{
			option = 0;
		}
	}

	if (START.region(1150, 350).mouseOver()) menu = 1;
	if (START.region(1150, 600).mouseOver()) menu = 2;

}

void Title::draw() const
{
	texture.draw();
	titleName.scaled(1.2,1.2).draw(0,50);
	START.draw(1150, 350);
	exit.draw(1150, 600);
	if (option == 1)
	{
		auto x =Rect{ 750,300,450 }.draw();
		auto m=SimpleGUI::Slider(U"マウス感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 1.0, Vec2{770,400}, 200, 200);
		if (SimpleGUI::Button(U"決定", Vec2{ 1100, 600 }))
		{
			
		}
	}
	if (option == 0)
	{
		if (menu == 1)
		{
			mouse.draw(1150, 350);
		}
		else if (menu == 2)
		{
			mouse.draw(1150, 600);
		}
	}

}
