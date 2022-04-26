# include <Siv3D.hpp>
//#include <cri_adx2le.h>
//#include <cri_le_error.h>
#include "common.h"
#include "Title.h"
#include "Game.h"
#include "Menu.h"
#include "Result.h"

void Main()
{
	Window::SetFullscreen(true);
	Window::SetTitle(U"BlindFPS");
	Scene::SetResizeMode(ResizeMode::Actual);

#ifdef _DEBUG
	const Font font{ 50 };
	Window::SetFullscreen(false);
	Window::Resize(Size{ 1280,720 });
	Window::SetToggleFullscreenEnabled(true);
#endif // _DEBUG

	//criAtomEx_Initialize(NULL, NULL, 0);

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

