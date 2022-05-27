#include "Result.h"
Result::Result(const InitData& init)
	:IScene(init)
{

}

void Result::update()
{
	if (videoTexture.posSec() == videoTexture.lengthSec())
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
