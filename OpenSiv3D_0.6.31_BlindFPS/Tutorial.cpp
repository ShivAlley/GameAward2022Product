#include "Tutorial.h"

Tutorial::Tutorial(const InitData& init) : IScene(init),Game(init)
{
	Scene::SetBackground(Palette::Black);
	tutorialErase();
}

void Tutorial::update()
{
	
	Game::tutorial();
}

void Tutorial::draw() const
{
	Game::tutorialDraw();
}
