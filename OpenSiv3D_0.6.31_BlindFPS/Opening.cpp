#include "Opening.h"

Opening::Opening(const InitData& init) : IScene(init)
{
	Cursor::SetDefaultStyle(CursorStyle::Hidden);
	Terminal::GetInst().logs.clear();
	Terminal::GetInst().txtIndex.clear();
	AudioAsset(U"v音声戦闘支援").play();
	//AudioAsset(U"ambientNoize").play();
}

void Opening::update()
{
	auto pad = XInput(0);

	

	
	if (getData().watchedOpening)
	{
		if (pad.isConnected())
		{
			if (pad.buttonB.pressed())
			{
				overridingCount = KeyB.pressedDuration().count() * 200;
				
			}
			if (pad.buttonB.up())
			{
				overridingCount = 0;

			}
		}
		else
		{
			if (KeyB.pressed())
			{
				overridingCount = KeyB.pressedDuration().count() * 200;
			}
			if (KeyB.up())
			{
				overridingCount = 0;
			}
			overridingCount = s3d::Clamp(overridingCount, 0, 300);
		}
		if (overridingCount > 290)
		{
			overriding = true;
			overridingDrawingEnd = true;
		}
	}
	if (AudioAsset(U"v音声戦闘支援").isPlaying())
	{
		return;//HACK:ヤバいreturn
	}
	static int32 sleepingCount = 0;
	if(sleepingCount > 3 and not helloWorld)
	{
		pressAny = true;
		lightAni = 1.0 - static_cast<double>(sleepingCount) / 10;
		lightAni = Min(lightAni,1.0);
	}
	if (helloWorld)
	{
		pressAny = false;
	}

	if (KeyA.down() or pad.buttonA.down())
	{
		helloWorld = true;
	}
	if (awakeTimer.elapsed() > 2s
		and helloWorld == false)
	{
		if (IsEven(sleepingCount))
		{
			Terminal::GetInst().command[Logging::Opening] = U"Conscious rebooting...";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v00op").play();
		}
		else
		{
			Terminal::GetInst().command[Logging::Opening] = U"Failed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v01op").play();
		}
		++sleepingCount;
		awakeTimer.restart();
	}
	if (awakeTimer.elapsed() > 2s
		and helloWorld)
	{
		if (IsEven(sleepingCount))
		{
			Terminal::GetInst().command[Logging::Opening] = U"Conscious rebooting...";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v00op").play();
		}
		else
		{
			Terminal::GetInst().command[Logging::Opening] = U"Success";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v02op").play();
			wakeUp = true;
		}
		++sleepingCount;
		awakeTimer.restart();
		if (wakeUp)
		{
			awakeTimer.reset();
			controlTimer.start();
			sleepingCount = 0;
		}
	}
	if (wakeUp and controlTimer.elapsed() > 1s)
	{
		switch (opCount)
		{
		case 0:
			Terminal::GetInst().command[Logging::Opening] = U"initializing...";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v03op").play();
			break;
		case 1:
			Terminal::GetInst().command[Logging::Opening] = U"vital - stable";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v04op").play();
			break;
		case 2:
			Terminal::GetInst().command[Logging::Caution] = U"CAUTION:Voltage dropped";
			Terminal::GetInst().txtIndex.push_back(Logging::Caution);
			AudioAsset(U"v05op").play();
			break;
		case 3:
			Terminal::GetInst().command[Logging::Opening] = U"Auxilary Power Unit - ON";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v06op").play();
			break;
		case 4:
			Terminal::GetInst().command[Logging::Opening] = U"Intrusion Countermeasures Electronics / <I.C.E.> active";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v07op").play();
			break;
		case 5:
			Terminal::GetInst().command[Logging::ErrorOpening] = U"ERROR:S.L.A.M. visualize failed";
			Terminal::GetInst().txtIndex.push_back(Logging::ErrorOpening);
			Terminal::GetInst().command[Logging::ErrorOpening2] = U"ERROR:Access violation / No such device";
			Terminal::GetInst().txtIndex.push_back(Logging::ErrorOpening2);
			Terminal::GetInst().command[Logging::ErrorOpening3] = U"ERROR:Operation abort";
			Terminal::GetInst().txtIndex.push_back(Logging::ErrorOpening3);
			Terminal::GetInst().command[Logging::ErrorOpening4] = U"ERROR:LEVEL 3 HALTED";
			Terminal::GetInst().txtIndex.push_back(Logging::ErrorOpening4);
			AudioAsset(U"v08op").play();
			break;
		case 12:
			Terminal::GetInst().command[Logging::Opening2] = U"Alternative LiDAR S.L.A.M. system constructed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening2);
			AudioAsset(U"v09op").play();
			break;
		case 15:
			Terminal::GetInst().command[Logging::Opening] = U"Armament check";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v10op").play();
			break;
		case 16:
			Terminal::GetInst().command[Logging::Opening3] = U"Weapon Master Arm - ON";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening3);
			AudioAsset(U"v17op").play();
			break;
		case 17:
			Terminal::GetInst().command[Logging::Opening] = U"Hybrid Electrothermal Light-gas gun - Available";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v11op").play();
			break;
		case 18:
			Terminal::GetInst().command[Logging::Opening] = U"Vectronics Module - Available";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v12op").play();
			break;
		case 20:
			Terminal::GetInst().command[Logging::Opening] = U"DATALINK to HQ - Failed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v13op").play();
			break;
		case 23:
			Terminal::GetInst().command[Logging::Opening] = U"Location:<UNKNOWABLE AREA>";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v14op").play();
			break;
		case 26:
			Terminal::GetInst().command[Logging::Opening] = U"Request surgery / Required permission ";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			AudioAsset(U"v15op").play();
			break;
		default:
			break;
		}
		if (not overriding)
		{
			controlTimer.restart();
		}
		opCount++;
	}

	if (pad.isConnected())
	{
		if (pad.buttonA.pressed())
		{
			touchCount = pad.buttonA.pressedDuration().count() * 300;
		}
		if (pad.buttonA.up())
		{
			touchCount = 0;
		}
	}
	else
	{
		if (KeyA.pressed())
		{
			touchCount = KeyA.pressedDuration().count() * 300;
		}
		if (KeyA.up())
		{
			touchCount = 0;
		}
	}
	touchCount = s3d::Clamp(touchCount,0,512);
	if (overriding)
	{
		if (controlTimer.isRunning() or awakeTimer.isRunning())
		{
			Terminal::GetInst().command[Logging::overriding] = U"CAUTION:Unauthorized override";
			Terminal::GetInst().txtIndex.push_back(Logging::overriding);
			AudioAsset(U"v16op").play();
		}
		controlTimer.reset();
		awakeTimer.reset();
	}
	//maxCase + 1
	if (opCount > 27 or overriding)
	{
		canAttach = true;
	}

	if (touchCount > 500)
	{
		displayIcon = true;
		moveLerpTimer.start();
		Terminal::GetInst().logs.clear();
	}
	if (moveLerpTimer.elapsed() > 4s)
	{
		moveLngIcon = { 0,Scene::Size().y * 0.6 };
		moveLngTxt = { 0,Scene::Size().y * 0.8 };
		time = Min(moveLerpTimer.sF() - 4.0,1.0);
		elapse = EaseInCirc(time);

		moveLngIcon *= elapse;
		moveLngTxt *= elapse;
	}
	//Print << moveLngIcon;
	//Print << moveLngTxt;
	//Print << moveLerpTimer.sF();
	//Print << elapse;

	if (moveLerpTimer.elapsed() > 5s)
	{
		if (getData().watchedTutorial == false)
		{
			changeScene(GameState::Tutorial,0);
			//AudioAsset(U"ambientNoize").stop();
		}
		else
		{
			changeScene(GameState::Game);
			//AudioAsset(U"ambientNoize").stop();
		}
		getData().watchedOpening = true;
	}

#if _DEBUG


	if (KeyK.down())
	{
		changeScene(GameState::Title);
	}
#endif // _DEBUG
}

void Opening::draw() const
{
	Draw2D();
	Draw2DLightBloomed();
	
	
	{
		// ガウスぼかし用テクスチャにもう一度シーンを描く
		// ここで重ねて描画した2Dテクスチャはライトブルーム効果が付く
		{
			const ScopedRenderTarget2D target2{ gaussianA1.clear(Palette::Black) };
			const ScopedRenderStates2D blend2{ BlendState::Additive };
			Draw2DLightBloomed();
		}

		// オリジナルサイズのガウスぼかし (A1)
		// A1 を 1/4 サイズにしてガウスぼかし (A4)
		// A4 を 1/2 サイズにしてガウスぼかし (A8)
		Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		Shader::Downsample(gaussianA1, gaussianA4);
		Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		Shader::Downsample(gaussianA4, gaussianA8);
		Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
		const ScopedRenderStates2D blend2{ BlendState::Additive };
		gaussianA1.resized(Scene::Size()).draw(ColorF{ 0.1 });
		gaussianA4.resized(Scene::Size()).draw(ColorF{ 0.4 });
		gaussianA8.resized(Scene::Size()).draw(ColorF{ 0.8 });
	}
}

void Opening::drawFadeOut(double t) const
{
	Rect{ Scene::Size() }.draw(Palette::Black);
}

void Opening::Draw2D()const
{
	Rect{ Scene::Size() }.draw(Palette::Black);
	if (getData().watchedOpening and overridingDrawingEnd == false)
	{
		Rect{ Arg::topRight(Scene::Size().x,30),300,64 }.drawFrame(1.0, Palette::Grey);
		Rect{ Arg::topLeft(Scene::Size().x - 300,30),overridingCount,64 }.draw(Palette::Grey);
		FontAsset(U"meiryo")(U"B長押しでスキップ").draw(Scene::Size().x - 286, 50);
	}
	if (canAttach and not displayIcon )
	{
		Rect{ Arg::leftCenter(Scene::Center() - Point{226,0}),touchCount,128 }.draw(Palette::Orangered);
	}
	if (displayIcon and strTimer.elapsed() > 200ms)
	{
		TextureAsset(U"interfaceLogo").drawAt(Vec2{Scene::Center().x,Scene::Size().y * 0.25} - Vec2{moveLngIcon});
		if (strTimer.elapsed() < 1000ms)
		{
			AudioAsset(U"vロボトミクスインターフェース").play();
			AudioAsset(U"established").play();
		}
	}
	if (pressAny)
	{
		opmeiryoFont(U"PRESS TO A KEY").drawAt(Scene::Center().x,Scene::Size().y * 0.75).draw(ColorF{0,0,0,lightAni});
	}
}

void Opening::Draw2DLightBloomed()const
{

	
	if (canAttach and not displayIcon)
	{
		Rect{ Arg::center(Scene::Center() + Point{30,0}),512,128 }.drawFrame(1.0, Palette::Orangered);
		Circle{ Arg::center(Scene::Center() - Point{158,0}),50 }.drawFrame(3.0, Palette::Tomato);
		opmeiryobFont(U"A").drawAt(Scene::Center() - Point{ 158,0 }, Palette::Green);
		opmeiryoFont(U"ttach").drawAt(Scene::Center() - Point{ 40,-12 }, Palette::White);
	}

	if (displayIcon)
	{
		strTimer.start();
		//AudioAsset(U"ambientNoize").stop();
		const size_t length = strTimer.elapsed() / 30ms;
		currentStr = opmeiryoFont(U"ROBOTMIX  INTERFACE  ESTABLISHED").text.substr(0, length);
		Vec2 glyphPos = Vec2{ Scene::Size().x * 0.25,Scene::Size().y * 0.6 } - Vec2{moveLngTxt};
		for (const auto& glyph : opmeiryoFont.getGlyphs(currentStr))
		{
			glyph.texture.draw(glyphPos + glyph.getOffset(), Palette::White);
			glyphPos.x += glyph.xAdvance;


		}
	}
	if (not displayIcon)
	{
		Terminal::GetInst().outputTxt();
	}
	//DrawTxt();
	
	
}

void Opening::Terminal::outputTxt()
{
	if (not txtIndex.empty())
	{
		streamTimer.start();
		const size_t length = streamTimer.elapsed() / 20ms;
		currentStreamingCommand = command.at(txtIndex.front()).substr(0, length);
		//FontAsset(U"meiryo")((command.at(txtIndexManager.front())).substr(0,length))
		Vec2 glyphPos = Vec2{ 20, Scene::Height() - FontAsset(U"meiryo").height() * 2 };
		for (const auto& glyph : FontAsset(U"meiryo").getGlyphs(currentStreamingCommand))
		{
			ColorF color = Palette::White;
			switch (txtIndex.front())
			{
			case Logging::ErrorOpening:
			case Logging::ErrorOpening2:
			case Logging::ErrorOpening3:
			case Logging::ErrorOpening4:
			case Logging::overriding:
				color = Palette::Red;
				break;
			case Logging::Caution:
				color = Palette::Gold;
				break;
			case Logging::Opening:
			default:
				color = Palette::White;
				if (isHighlightedOrange(glyph.codePoint))
				{
					color = Palette::Orangered;
				}
				break;
			}
			//改行文字の場合特別な処理が必要だがそもそもTerminalに改行文字は渡さないようにする
			
			glyph.texture.draw(glyphPos + glyph.getOffset(), color);
			glyphPos.x += glyph.xAdvance;
			
			
		}

		//FontAsset(U"meiryo")(currentStreamingCommand).draw(20, Scene::Height() - FontAsset(U"meiryo").height() * 2);
		if (length > currentStreamingCommand.length())
		{
			streamTimer.reset();
			//logs.emplace_front(command.at(txtIndex.front()));
			logs.emplace_front(std::make_pair(txtIndex.front(), command.at(txtIndex.front())));
			txtIndex.pop_front();
		}

	}
	for (const auto& [index, msg] : Indexed(logs))
	{
		int32 proportionalMerginY = -(FontAsset(U"meiryo").height()) * index - (FontAsset(U"meiryo").height() * 3);
		/*FontAsset(U"meiryo")(msg)
			.draw(20,
			Scene::Height() + proportionalMerginY);*/


		Vec2 glyphPos = Vec2{ 20,Scene::Height() + proportionalMerginY };
		for (const auto& glyph : FontAsset(U"meiryo").getGlyphs(msg.second))
		{
			ColorF color = Palette::White;
			//if (not msg.first.empty())
			{
				switch (msg.first)
				{
				case Logging::ErrorOpening:
				case Logging::ErrorOpening2:
				case Logging::ErrorOpening3:
				case Logging::ErrorOpening4:
				case Logging::overriding:
					color = Palette::Red;
					break;
				case Logging::Caution:
					color = Palette::Gold;
					break;
				case Logging::Opening:
				default:
					color = Palette::White;
					if (isHighlightedOrange(glyph.codePoint))
					{
						color = Palette::Orangered;
					}
					break;
				}
			}
			//文字の描画
			glyph.texture.draw(glyphPos + glyph.getOffset(), color);
			glyphPos.x += glyph.xAdvance;
		}
	}
	if (logs.size() > Scene::Height() / FontAsset(U"meiryo").height() - 2)
	{
		logs.pop_back();
	}


}
