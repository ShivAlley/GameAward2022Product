#pragma once
#include <Siv3D.hpp>



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

#ifdef _DEBUG
constexpr int DEV_ONLY_MAGIC_NUM = 42;
#endif

using App = SceneManager<GameState,ShareGameData>;
