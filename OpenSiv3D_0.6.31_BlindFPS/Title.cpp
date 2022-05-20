#include "Title.h"
Title::Title(const InitData& init)
	:IScene(init)
{

}

void Title::update()
{
	menu = 0;

	if (START.region(pos1).mouseOver()
		and MouseL.down() and option==0 and tutorial ==0)
	{
		changeScene(GameState::Game);
	}
	if (Option.region(pos2).mouseOver()
		and MouseL.down() and tutorial ==0)
	{
		option = 1;
	}
	
	if (TutorialName.region(pos3).mouseOver()
		and MouseL.down() and option ==0)
	{
		tutorial = 1;
	}
	if (Exit.region(pos4).mouseOver()
		and MouseL.down() and option == 0 and tutorial == 0)
	{
		System::Exit();
	}

	if (option == 1)
	{
		if (SimpleGUI::Button(U"決定", Vec2{ 1050,600 }))
		{
			option = 0;
		}
	}
	if (tutorial == 1)
	{
		if (SimpleGUI::Button(U"決定", Vec2{ Scene::Center().x,900 }))
		{
			tutorial= 0;
		}
	}
	if (START.region(pos1).mouseOver()) menu = 1;
	if (START.region(pos2).mouseOver()) menu = 2;
	if (START.region(pos3).mouseOver()) menu = 3;
	if (START.region(pos4).mouseOver()) menu = 4;

	//画面を揺らす
	/*if (position.y < 0) {
		speed.y += 1;

		position.y += (this->speed.y);
	}
	position.x += sinf((timer % 60) / 30.0f * PI) * 0.4f;


	++timer;
	timer++;*/
}

void Title::draw() const
{
	texture.draw();

	titleName.scaled(1.2,1.2).draw(position.x,position.y);
	START.draw(pos1);
	Option.draw(pos2);
	TextureAsset(U"tutorial").draw(pos3);
	Exit.scaled(0.5,0.5).draw(pos4);

	if (option == 1)
	{
		auto x =Rect{ 750,300,450 }.draw();
		auto m=SimpleGUI::Slider(U"マウス感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{770,400}, 200, 200);
		SimpleGUI::Button(U"決定", Vec2{ 1050,600 });
	}
	if (tutorial == 1)
	{
		Rect{ Scene::Size() }.draw(ColorF{ 0,0,0,0.8 });
		TextureAsset(U"description")
			.scaled(0.2, 0.2)
			.drawAt(Scene::Center());
		SimpleGUI::Button(U"決定", Vec2{ Scene::Center().x,900 });
	}

	if (option == 0&&tutorial ==0)
	{
		if (menu == 1)
		{
			mouse.draw(pos1);
		}
		else if (menu == 2)
		{
			mouse.draw(pos2);
		}
		else if (menu == 3)
		{
			mouse.draw(pos3);
		}
		else if (menu == 4)
		{
			mouse.scaled(0.5, 0.5).draw(pos4);
		}
	}

}
