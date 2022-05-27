#include <Siv3D.hpp>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Caution.h"
#include "Result.h"
#include "Opening.h"
#include "Tutorial.h"



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
	AudioAsset::Register(U"testFootStep",GMInstrument::Piano2,PianoKey::C4,1.0s);
	AudioAsset::Register(U"testFootStep2",GMInstrument::Piano2,PianoKey::D4,1.0s);
#endif // _DEBUG
	AudioAsset::Register(U"rangeFootStep", U"Sound/rangeFootStep.mp3");
	AudioAsset::Register(U"meleeFootStep", U"Sound/meleeFootStep.mp3");
	AudioAsset::Register(U"meleeAtk", U"Sound/meleeAtk.mp3");
	AudioAsset::Register(U"rangeAtk", U"Sound/rangeAtk.mp3");
	AudioAsset::Register(U"brokenEnemy", U"Sound/brokenEnemy.mp3");
	AudioAsset::Register(U"notice", U"Sound/notice.mp3");
	AudioAsset::Register(U"reloadStart", U"Sound/reloadStart.mp3");
	AudioAsset::Register(U"reloadEnd", U"Sound/reloadEnd.mp3");
	AudioAsset::Register(U"shot", U"Sound/playerShot.mp3");
	AudioAsset::Register(U"noAmmo", U"Sound/noAmmo.mp3");


	TextureAsset::Register(U"HELgunIcon", U"Image/HELgunIcon.png");
	TextureAsset::Register(U"indicater", U"Image/indicater.png");
	//TextureAsset::Register(U"indicater2", U"Image/indicater2.png");
	TextureAsset::Register(U"crossHair", U"Image/crossHair.png");
	TextureAsset::Register(U"navigateArrow", U"Image/navigateArrow.png");
	TextureAsset::Register(U"HPGaugeFrame", U"Image/HPGaugeFrame.png");
	TextureAsset::Register(U"HPGauge", U"Image/HPGauge.png");
	TextureAsset::Register(U"lidar", U"Image/lidar.png");
	TextureAsset::Register(U"radar", U"Image/radar.png");
	TextureAsset::Register(U"radarCenter", U"Image/radarCenter.png");
	TextureAsset::Register(U"description", U"Image/description.png");
	TextureAsset::Register(U"tutorial", U"Image/tutorial.png");
	TextureAsset::Register(U"interfaceLogo", U"Image/interfaceLogo.png");
	TextureAsset::Register(U"head", U"🎧"_emoji);

	TextureAsset::Register(U"compass", U"Image/compass.png");


}


void Main()
{
	Scene::Resize(Size{ 1920,1080 });
	Window::Resize(Size{ 1920,1080 });
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetTitle(U"BlindFPS");
	Window::SetFullscreen(true);
	System::SetTerminationTriggers(UserAction::NoAction);
	//Window::ResizeVirtual(Size{1920,1080});

#ifdef _DEBUG
	const Font font{ 50 };
	Window::SetFullscreen(true);
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
	manager.add<Caution>(GameState::Menu);
	manager.add<Opening>(GameState::Opening);
	manager.add<Result>(GameState::Result);
	manager.add<Tutorial>(GameState::Tutorial);
	manager.init(GameState::Game);
	while (System::Update())
	{	
		if (not manager.update())
		{
			break;
		}



	}
}



