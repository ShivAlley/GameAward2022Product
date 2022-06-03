#pragma once
#include "common.h"
class Opening :public App::Scene
{
public:
	Opening(const InitData& init);
	void update()override;
	void draw() const override;
	void drawFadeOut(double t)const override;
private:
	//FontAsset::Register(U"meiryo", 30, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryo.ttc");
	//FontAsset::Register(U"meiryob", 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryob.ttc");
	const Font opmeiryobFont{ 100, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryob.ttc" };
	const Font opmeiryoFont{ 50, FileSystem::GetFolderPath(SpecialFolder::SystemFonts) + U"meiryo.ttc" };
	int32 opCount = 0;
	bool canAttach = false;
	bool displayIcon = false;
	bool overriding = false;
	bool overridingDrawingEnd = false;
	bool pressAny = false;
	double lightAni = 0.0;
	int32 overridingCount = 0;
	Stopwatch playerTouch{ StartImmediately::No};
	int32 touchCount = 0;
	mutable String currentStr;
	double time = 0;
	double elapse = 0;
	Vec2 moveLngIcon = {0,0};
	Vec2 moveLngTxt = {0,0};
	Stopwatch moveLerpTimer{StartImmediately::No};
	
	const RenderTexture gaussianA1{ Scene::Size() }, gaussianB1{ Scene::Size() };
	const RenderTexture gaussianA4{ Scene::Size() / 4 }, gaussianB4{ Scene::Size() / 4 };
	const RenderTexture gaussianA8{ Scene::Size() / 8 }, gaussianB8{ Scene::Size() / 8 };
	void Draw2D()const;
	void Draw2DLightBloomed()const;
	enum class Logging
	{
		Opening = 0,
		Caution,
		Opening2,
		Opening3,
		ErrorOpening,
		ErrorOpening2,
		ErrorOpening3,
		ErrorOpening4,
		overriding,
	};
	class Terminal
	{
	private:
		Terminal() = default;
		~Terminal() = default;

	public:
		static Terminal& GetInst() {
			static Terminal terminal;
			return terminal;
		}
		Stopwatch streamTimer{ StartImmediately::No };
		Stopwatch hyphenTimer{ StartImmediately::No };
		std::deque<Logging> txtIndex;
		HashTable<Logging, String> command;
		String currentStreamingCommand;
		//mutable std::deque<String> logs;
		mutable std::deque<std::pair<Logging, String>> logs;
		void outputTxt();
		bool isHighlightedOrange(char32 glyph) {
			return
				(
				(glyph == U'0')
				or (glyph == U'1')
				or (glyph == U'2')
				or (glyph == U'3')
				or (glyph == U'4')
				or (glyph == U'5')
				or (glyph == U'6')
				or (glyph == U'7')
				or (glyph == U'8')
				or (glyph == U'9')
				);
		}
	};

	Stopwatch awakeTimer{ StartImmediately::Yes};
	mutable Stopwatch strTimer{ StartImmediately::No};
	Stopwatch controlTimer{ StartImmediately::No};
	bool helloWorld = false;
	bool wakeUp = false;

};

