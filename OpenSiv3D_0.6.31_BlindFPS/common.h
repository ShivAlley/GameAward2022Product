#pragma once
#include <Siv3D.hpp>



enum class GameState
{
	Title,
	Opening,
	Tutorial,
	Game,
	Menu,
	Result,
};

struct ShareGameData
{
	double sensi = 0.5;
	bool watchedOpening = true;
	bool watchedTutorial = false;
};

#ifdef _DEBUG
constexpr int DEV_ONLY_MAGIC_NUM = 42;
#endif

using App = SceneManager<GameState,ShareGameData>;
