#include <Siv3D.hpp>
#include <cri_adx2le.h>
#include "3Dpos.h"
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Menu.h"
#include "Result.h"



void TexRegist()
{
#ifdef _DEBUG
	TextureAsset::Register(U"testArrow", U"texture/JamArrow.png");
	TextureAsset::Register(U"sampleIndicater", U"texture/sampleIndicater.png");

#endif // _DEBUG

}

void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	const CriChar8* errmsg;
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	printf("%s\n", errmsg);
}

void* user_malloc(void* obj, CriUint32 size)
{
	void* mem;

	// メモリの確保
	mem = malloc(size);

	return (mem);
}

// 独自のメモリ解放関数を用意
void user_free(void* obj, void* mem)
{
	// メモリの解放
	free(mem);

	return;
}

void Main()
{
	Window::SetFullscreen(true);
	Window::SetTitle(U"BlindFPS");
	Scene::SetResizeMode(ResizeMode::Actual);

#ifdef _DEBUG
	const Font font{ 50 };
	Window::SetFullscreen(false);
	Window::Resize(Size{ 1920,1080 });
	Window::SetToggleFullscreenEnabled(true);
#endif // _DEBUG
	/*CriAtomExConfig criAtomDesc;
	CriFsConfig fsDesc;
	criAtomEx_SetDefaultConfig(&criAtomDesc);
	criAtomDesc.max_virtual_voices = 16;
	criAtomDesc.max_categories = 64;
	criAtomDesc.categories_per_playback = 64;
	criFs_SetDefaultConfig(&fsDesc);
	fsDesc.num_loaders = 32;
	fsDesc.num_binders = 32;
	fsDesc.max_binds = 32;*/
	criErr_SetCallback(user_error_callback_func);

	criAtomEx_SetUserAllocator(user_malloc, user_free, NULL);

	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = 32;
	lib_config.hca_mx.output_sampling_rate = 32000;
	fs_config.num_loaders = 128;
	fs_config.num_binders = 128;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	

	TexRegist();


	App manager;
	manager.add<Title>(GameState::Title);
	manager.add<Game>(GameState::Game);
	manager.add<Menu>(GameState::Menu);
	manager.add<Result>(GameState::Result);

	manager.init(GameState::Game);
	while (System::Update())
	{	
		if (not manager.update())
		{
			break;
		}



	}
	criAtomEx_Finalize();
}



