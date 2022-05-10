#pragma once
#include<Siv3D.hpp>

enum class GameState
{
	Title,
	Game,
	Menu,
	Result,
};

struct ShareGameData
{
	char dummy;
};

using App = SceneManager<GameState,ShareGameData>;
