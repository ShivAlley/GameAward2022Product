#include "Opening.h"

Opening::Opening(const InitData& init) : IScene(init)
{
	Cursor::SetDefaultStyle(CursorStyle::Hidden);
	Terminal::GetInst().logs.clear();
	Terminal::GetInst().txtIndex.clear();

}

void Opening::update()
{
	if (getData().watchedOpening)
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
		if (overridingCount > 290)
		{
			overriding = true;
			overridingDrawingEnd = true;
		}
	}
	static int32 sleepingCount = 0;
	if (Key0.down())
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
		}
		else
		{
			Terminal::GetInst().command[Logging::Opening] = U"Failed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
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
		}
		else
		{
			Terminal::GetInst().command[Logging::Opening] = U"Success";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			wakeUp = true;
		}
		++sleepingCount;
		awakeTimer.restart();
		if (wakeUp)
		{
			awakeTimer.reset();
			controlTimer.start();
		}
	}
	if (wakeUp and controlTimer.elapsed() > 1s)
	{
		switch (opCount)
		{
		case 0:
			Terminal::GetInst().command[Logging::Opening] = U"initializing...";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 1:
			Terminal::GetInst().command[Logging::Opening] = U"vital - stable";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 2:
			Terminal::GetInst().command[Logging::Caution] = U"CAUTION:Voltage dropped";
			Terminal::GetInst().txtIndex.push_back(Logging::Caution);
			break;
		case 3:
			Terminal::GetInst().command[Logging::Opening] = U"Auxilary Power Unit - ON";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 4:
			Terminal::GetInst().command[Logging::Opening] = U"Intrusion Countermeasures Electronics / <I.C.E.> active";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
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
			break;
		case 6:
			Terminal::GetInst().command[Logging::Opening2] = U"Alternative LiDAR S.L.A.M. system constructed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening2);
			break;
		case 7:
			Terminal::GetInst().command[Logging::Opening] = U"Armament check";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 8:
			Terminal::GetInst().command[Logging::Opening3] = U"Weapon Master Arm - ON";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening3);
			break;
		case 9:
			Terminal::GetInst().command[Logging::Opening] = U"Hybrid Electrothermal Light-gas gun - Available";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 10:
			Terminal::GetInst().command[Logging::Opening] = U"Vectronics Module - Available";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 11:
			Terminal::GetInst().command[Logging::Opening] = U"DATALINK to HQ - Failed";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 12:
			Terminal::GetInst().command[Logging::Opening] = U"Location:<UNKNOWABLE AREA>";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		case 13:
			Terminal::GetInst().command[Logging::Opening] = U"Request surgery / Required permission ";
			Terminal::GetInst().txtIndex.push_back(Logging::Opening);
			break;
		default:
			break;
		}
		controlTimer.restart();
		opCount++;
	}
	if (KeyA.pressed())
	{
		touchCount = KeyA.pressedDuration().count() * 300;
	}
	if (KeyA.up())
	{
		touchCount = 0;
	}
	touchCount = s3d::Clamp(touchCount,0,512);
	if (opCount > 13 or overriding)
	{
		canAttach = true;
	}

	if (touchCount > 500)
	{
		displayIcon = true;
		//timerstart
		Terminal::GetInst().logs.clear();
	}

	if (KeyK.down())
	{
		changeScene(GameState::Title);
	}
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

void Opening::Draw2D()const
{
	Rect{ Scene::Size() }.draw(Palette::Black);
	if (getData().watchedOpening and overridingDrawingEnd == false)
	{
		Rect{ Arg::topRight(Scene::Size().x,30),300,64 }.drawFrame(1.0, Palette::Grey);
		Rect{ Arg::topLeft(Scene::Size().x - 300,30),overridingCount,64 }.draw(Palette::Grey);
		FontAsset(U"meiryo")(U"B長押しでスキップ").draw(Scene::Size().x - 286, 50);
	}
	if (canAttach and not displayIcon)
	{
		Rect{ Arg::leftCenter(Scene::Center() - Point{226,0}),touchCount,128 }.draw(Palette::Orangered);
	}
	if (displayIcon)
	{
		TextureAsset(U"interfaceLogo").drawAt(Scene::Center().x,Scene::Size().y * 0.25);
		
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
		const size_t length = strTimer.elapsed() / 30ms;
		currentStr = opmeiryoFont(U"ROBOTMIX  INTERFACE  ESTABLISHED").text.substr(0, length);
		Vec2 glyphPos = Vec2{ Scene::Size().x * 0.25,Scene::Size().y * 0.6 };
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
