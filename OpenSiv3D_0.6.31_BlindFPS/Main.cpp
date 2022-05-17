#include <Siv3D.hpp>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Menu.h"
#include "Result.h"



void TexRegist()
{
	FontAsset::Register(U"meiryo", 30, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryo.ttc");
	FontAsset::Register(U"meiryob", 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryob.ttc");
#ifdef _DEBUG
	TextureAsset::Register(U"testArrow", U"texture/JamArrow.png");
	TextureAsset::Register(U"sampleIndicater", U"texture/sampleIndicater.png");
	TextureAsset::Register(U"sampleConpass", U"Image/SampleCompass.png");
	TextureAsset::Register(U"testLidar", U"Image/testLidar.png");
	TextureAsset::Register(U"indicaterLidar", U"Image/indicater.png");
	AudioAsset::Register(U"piano",GMInstrument::Piano1,PianoKey::C4,1.0s);
	AudioAsset::Register(U"piano2",GMInstrument::Piano1,PianoKey::D4,1.0s);
#endif // _DEBUG
	TextureAsset::Register(U"HELgunIcon", U"Image/HELgunIcon.png");
	TextureAsset::Register(U"indicater", U"Image/indicater.png");
	TextureAsset::Register(U"crossHair", U"Image/crossHair.png");
	TextureAsset::Register(U"navigateArrow", U"Image/navigateArrow.png");
	TextureAsset::Register(U"HPGaugeFrame", U"Image/HPGaugeFrame.png");
	TextureAsset::Register(U"HPGauge", U"Image/HPGauge.png");


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
}



