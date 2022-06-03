#include "Result.h"
Result::Result(const InitData& init)
	:IScene(init)
{
	AudioAsset(U"resultBegin").play();
}

void Result::update()
{
	if (resultTimer.elapsed() > 9s)
	{
		AudioAsset(U"getOrb").play();
	}
	if (videoTexture.posSec() >= videoTexture.lengthSec())
	{
		changeScene(GameState::Title);
	}
}

void Result::draw()const
{
	ClearPrint();
	videoTexture.advance();
	videoTexture
		.scaled(1).draw(0, 0);

}
