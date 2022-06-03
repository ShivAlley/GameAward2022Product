#pragma once
#include <Siv3D.hpp>
#include <Siv3D\XInput.hpp>



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
	double sensi = 1.0;
	bool watchedOpening = true;
	bool watchedTutorial = false;
	s3d::detail::XInput_impl controller = XInput(0);
	bool inTuto = false;
};

#ifdef _DEBUG
constexpr int DEV_ONLY_MAGIC_NUM = 42;
#endif

using App = SceneManager<GameState,ShareGameData>;
