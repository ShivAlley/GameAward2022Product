#include "Caution.h"

Caution::Caution(const InitData& init)
	:IScene(init)
{
	Scene::SetBackground(Palette::Lightsteelblue);
}

void Caution::update()
{
	auto pad = getData().controller = XInput(0);

	if (timer.elapsed() > 5s
		or MouseL.down()
		or KeyEnter.down()
		or KeySpace.down()
		or pad.buttonA.down())
	{
		changeScene(GameState::Title);
	}
}

void Caution::draw() const
{
	TextureAsset(U"head")
		.scaled(1.2, 1.2)
		.drawAt(Scene::Center());
	FontAsset(U"meiryob")(U"ヘッドホンをつけてのプレイを推奨します。").draw(500, 680);
}
