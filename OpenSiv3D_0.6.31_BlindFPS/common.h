#pragma once
#include <Siv3D.hpp>
#include <cri_adx2le.h>


void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray);

void* user_malloc(void* obj, CriUint32 size);

void user_free(void* obj, void* mem);

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
