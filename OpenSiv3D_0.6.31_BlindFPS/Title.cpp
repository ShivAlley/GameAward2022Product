#include "Title.h"
Title::Title(const InitData& init)
	:IScene(init)
{
	Cursor::SetDefaultStyle(CursorStyle::Default);
}

void Title::update()
{
	menu = 0;
	auto pad = XInput(0);
	Circle cir{Arg::center(Padpos),50};
	if(START.region(pos1).intersects(cir) and pad.buttonA.down())changeScene(GameState::Opening);
	if(Option.region(pos2).intersects(cir) and pad.buttonA.down())option = 1;
	if(TutorialName.region(pos3).intersects(cir) and pad.buttonA.down())tutorial = 1;
	if(Exit.region(pos4).intersects(cir) and pad.buttonA.down())System::Exit();
	if ((START.region(pos1).mouseOver() )
		and MouseL.down() and option==0 and tutorial ==0)
	{
		changeScene(GameState::Opening);
	}
	if ((Option.region(pos2).mouseOver() )
		and MouseL.down() and tutorial ==0)
	{
		option = 1;
	}
	
	if ((TutorialName.region(pos3).mouseOver() )
		and MouseL.down() and option ==0)
	{
		tutorial = 1;
	}
	if ((Exit.region(pos4).mouseOver() )
		and MouseL.down() and option == 0 and tutorial == 0)
	{
		System::Exit();
	}

	if (option == 1)
	{
		if (SimpleGUI::Button(U"決定", Vec2{ 1050,600 }) or pad.buttonB.down()
			or (SimpleGUI::ButtonRegionAt(U"決定", Vec2{ 1050,600 }).intersects(cir) and pad.buttonA.down()))
		{
			option = 0;
		}
	}
	if (tutorial == 1)
	{
		if (SimpleGUI::ButtonAt(U"決定", Vec2{ Scene::Size().x * 0.6,Scene::Size().y * 0.8 }) or pad.buttonB.down()
			or (SimpleGUI::ButtonRegionAt(U"決定", Vec2{ Scene::Size().x * 0.6,Scene::Size().y * 0.8 }).intersects(cir) and pad.buttonA.down()))
		{
			tutorial= 0;
		}
		if (SimpleGUI::ButtonAt(U"もう一度チュートリアルをプレイする", Vec2{ Scene::Size().x * 0.25,Scene::Size().y * 0.8 })
			or (SimpleGUI::ButtonRegionAt(U"もう一度チュートリアルをプレイする", Vec2{ Scene::Size().x * 0.25,Scene::Size().y * 0.8 }).intersects(cir) and pad.buttonA.down()))
		{
			changeScene(GameState::Tutorial);
		}
	}
	if (START.region(pos1).mouseOver() or START.region(pos1).intersects(cir)) menu = 1;
	if (START.region(pos2).mouseOver() or START.region(pos2).intersects(cir)) menu = 2;
	if (START.region(pos3).mouseOver() or START.region(pos3).intersects(cir)) menu = 3;
	if (START.region(pos4).mouseOver() or START.region(pos4).intersects(cir)) menu = 4;
	if (pad.isConnected())
	{
		SimpleGUI::Slider(U"エイム感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{ 770,400 }, 200, 200);
	}
	else
	{
		auto m = SimpleGUI::Slider(U"エイム感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{ 770,400 }, 200, 200);
	}
	if (pad.buttonLB.down())
	{
		getData().sensi -= 0.1;
	}
	if (pad.buttonRB.down())
	{
		getData().sensi += 0.1;
	}
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
	auto pad = XInput(0);
	texture.draw();

	titleName.scaled(1.2,1.2).draw(position.x,position.y);
	START.draw(pos1);
	Option.draw(pos2);
	TextureAsset(U"tutorial").draw(pos3);
	Exit.scaled(0.5,0.5).draw(pos4);

	if (option == 1)
	{
		auto x =Rect{ 750,300,600 }.draw();

		auto m=SimpleGUI::Slider(U"エイム感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{770,400}, 200, 200);
		if (pad.isConnected())
		{
			FontAsset(U"meiryob")(U"L").draw(Vec2{ 950,400 },Palette::Darkorange);
			FontAsset(U"meiryob")(U"R").draw(Vec2{ 970 + 200,400 },Palette::Darkorange);
		}
		SimpleGUI::Button(U"決定", Vec2{ 1050,600 });
	}
	if (tutorial == 1)
	{
		Rect{ Scene::Size() }.draw(ColorF{ 0,0,0,0.8 });
		if (pad.isConnected())
		{
			TextureAsset(U"controllerDesc")
				.drawAt(Scene::Center().x , Scene::Center().y - 150);
		}
		else
		{

			TextureAsset(U"description")
				.scaled(0.2, 0.2)
				.drawAt(Scene::Center());
		}
		SimpleGUI::ButtonAt(U"決定", Vec2{ Scene::Size().x * 0.6,Scene::Size().y * 0.8 });
		SimpleGUI::ButtonAt(U"もう一度チュートリアルをプレイする", Vec2{Scene::Size().x * 0.25,Scene::Size().y * 0.8 });
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
	
	if (pad.leftThumbX)
	{
		Padpos.x += pad.leftThumbX * Scene::DeltaTime() * 500;
	}
	if (pad.leftThumbY)
	{
		Padpos.y -= pad.leftThumbY * Scene::DeltaTime() * 500;
	}
	if (pad.isConnected())
	{
		drawCur(Padpos);
	}
}
