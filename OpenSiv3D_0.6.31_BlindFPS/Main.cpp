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
	AudioAsset::Register(U"getOrb", U"Sound/getOrb.mp3");
	AudioAsset::Register(U"resultBegin", U"Sound/resultBegin.mp3");
	AudioAsset::Register(U"established", U"Sound/established.mp3");
	AudioAsset::Register(U"ambientNoize", U"Sound/ambientNoize.mp3",Loop::Yes);
	AudioAsset::Register(U"v音声戦闘支援", U"voice/0-音声戦闘支援システム起動-gameawa.wav");
	AudioAsset::Register(U"v私の声が", U"voice/1-私の声が、聞こえますか、オペレーター-gameawa.wav");
	AudioAsset::Register(U"vロボトミクスインターフェース", U"voice/2-ロボトミクスインターフェース　起動完了-gameawa.wav");



	AudioAsset::Register(U"v認知特性", U"voice/00-オペレーターの認知特性に基づきシステム言-VPTutorial.wav");
	AudioAsset::Register(U"v視界喪失", U"voice/01-警告：現在視界を喪失しています-VPTutorial.wav");
	AudioAsset::Register(U"v作戦要綱", U"voice/02-作戦要綱を参照...エラー-VPTutorial.wav");
	AudioAsset::Register(U"v至上命令", U"voice/03-至上命令を参照：必ず帰還せよ-VPTutorial.wav");
	AudioAsset::Register(U"v再設定",U"voice/04-ミッションを再設定-VPTutorial.wav");
	AudioAsset::Register(U"v支援開始", U"voice/05-オペレーターの支援を開始します　ハッド起-VPTutorial.wav");
	AudioAsset::Register(U"v特殊状況下", U"voice/06-特殊状況かにおける復帰訓練を開始-VPTutorial.wav");
	AudioAsset::Register(U"v左スティック", U"voice/07-ひだりスティックで移動-VPTutorial.wav");
	AudioAsset::Register(U"vWASD", U"voice/08-WASDで移動-VPTutorial.wav");
	AudioAsset::Register(U"v右スティック", U"voice/09-右スティックで視点移動-VPTutorial.wav");
	AudioAsset::Register(U"vマウス", U"voice/10-マウスで視点移動-VPTutorial.wav");
	AudioAsset::Register(U"v位置推定", U"voice/11-画面右上に障害物との位置推定ソナーを表示-VPTutorial.wav");
	AudioAsset::Register(U"v画面下部", U"voice/12-画面下部に次の目標までの距離と方向を表示-VPTutorial.wav");
	AudioAsset::Register(U"v射撃", U"voice/13-みぎトリガーで射撃、Xボタンでリロードが-VPTutorial.wav");
	AudioAsset::Register(U"v左クリック", U"voice/14-左クリックで射撃、Rでリロードが可能です-VPTutorial.wav");
	AudioAsset::Register(U"v敵を発見した場合", U"voice/15-敵を発見した場合、画面上に位置をリアルタ-VPTutorial.wav");
	AudioAsset::Register(U"v疑似信号", U"voice/16-疑似信号により仮想の敵を召喚、撃破してく-VPTutorial.wav");
	AudioAsset::Register(U"v復帰訓練終了", U"voice/17-復帰訓練終了...１０秒後に仮想空間を終-VPTutorial.wav");
	AudioAsset::Register(U"v耐久限界", U"voice/18-画面中央に自機の耐久限界を表示中です-VPTutorial.wav");


	AudioAsset::Register(U"v敵味方不明機", U"voice/00-警告：周囲に敵味方不明機を多数感知　全て-VPGame.wav");
	AudioAsset::Register(U"v致命的な損傷", U"voice/01-危険：致命的な損傷　強制的に完全防御シス-VPGame.wav");
	AudioAsset::Register(U"v哲学的な死", U"voice/02-哲学的な死　プロトコル起動まで残り3秒-VPGame.wav");
	AudioAsset::Register(U"vコントロールロスト", U"voice/03-コントロール、ロスト-VPGame.wav");
	AudioAsset::Register(U"vさようなら", U"voice/04-さようなら、オペレーター-VPGame.wav");



	AudioAsset::Register(U"v経路更新", U"voice/05-目標への経路を更新-VPGame.wav");
	AudioAsset::Register(U"v経路逸脱", U"voice/06-警告：経路から逸脱...ルートを再構築し-VPGame.wav");
	AudioAsset::Register(U"v衝突", U"voice/07-警告：壁と衝突しています-VPGame.wav");
	AudioAsset::Register(U"v命中せず", U"voice/08-警告：命中せず-VPGame.wav");
	AudioAsset::Register(U"vリロード中", U"voice/09-リロード中です...-VPGame.wav");
	AudioAsset::Register(U"vリロード完了", U"voice/10-リロード完了-VPGame.wav");
	AudioAsset::Register(U"v弾倉ゼロ", U"voice/11-警告：弾倉内の残弾数ゼロ-VPGame.wav");
	AudioAsset::Register(U"v被弾", U"voice/12-警告：敵から被弾-VPGame.wav");
	AudioAsset::Register(U"v感知", U"voice/13-敵を感知-VPGame.wav");
	AudioAsset::Register(U"v撃破", U"voice/14-敵を撃破-VPGame.wav");
	AudioAsset::Register(U"v15game", U"voice/15-2-VPGame.wav");
	AudioAsset::Register(U"v16game", U"voice/16-1-VPGame.wav");


	AudioAsset::Register(U"v00op", U"voice/00-意識再覚醒を実行-VPOpening.wav");
	AudioAsset::Register(U"v01op", U"voice/01-失敗-VPOpening.wav");
	AudioAsset::Register(U"v02op", U"voice/02-成功-VPOpening.wav");
	AudioAsset::Register(U"v03op", U"voice/03-初期化中-VPOpening.wav");
	AudioAsset::Register(U"v04op", U"voice/04-バイタル安定-VPOpening.wav");
	AudioAsset::Register(U"v05op", U"voice/05-警告　電圧低下-VPOpening.wav");
	AudioAsset::Register(U"v06op", U"voice/06-補助電源ユニット　起動-VPOpening.wav");
	AudioAsset::Register(U"v07op", U"voice/07-アイスウォール　アクティブ-VPOpening.wav");
	AudioAsset::Register(U"v08op", U"voice/08-エラー　自己位置推定についてのビジュアラ-VPOpening.wav");
	AudioAsset::Register(U"v09op", U"voice/09-代替的クリアランスソナーシステムを構築　-VPOpening.wav");
	AudioAsset::Register(U"v10op", U"voice/10-アーマメントチェック-VPOpening.wav");
	AudioAsset::Register(U"v11op", U"voice/11-ヘルガン　利用可能-VPOpening.wav");
	AudioAsset::Register(U"v12op", U"voice/12-ヴェクトロニクスモジュール　利用可能-VPOpening.wav");
	AudioAsset::Register(U"v13op", U"voice/13-作戦司令部とのデータリンク　失敗-VPOpening.wav");
	AudioAsset::Register(U"v14op", U"voice/14-現在地　不可知戦域-VPOpening.wav");
	AudioAsset::Register(U"v15op", U"voice/15-せじゅつを要求　オペレーターの許可が必要-VPOpening.wav");
	AudioAsset::Register(U"v16op", U"voice/16-警告：認証されていない プロトコルのオー-VPOpening.wav");
	AudioAsset::Register(U"v17op", U"voice/17-マスターアーム　オン-VPOpening.wav");

	AudioAsset::Register(U"v0end", U"voice/0-高エネルギー放射を検知　高次元物質の低次-新規プロジェクト.wav");






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
	TextureAsset::Register(U"controllerDesc", U"Image/controllerDesc.png");
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
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
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
	manager.init(GameState::Menu);
	while (System::Update())
	{	
		if (not manager.update())
		{
			break;
		}



	}
}



