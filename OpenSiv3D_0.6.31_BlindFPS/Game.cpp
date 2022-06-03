#include "Game.h"

Game::Game(const InitData& init) :IScene(init)
{
	//四角生成
	for (int z = 0; z < 20; z++)//y on 2d
	{
		for (int x = 0; x < 3; x++)//x on 2d
		{
			//boxes << Box{ (x * 4.0), 2, 50 + (z * 4.0), 4.0 };
		}
	}
	terrCSV.load(U"csv/GameAwa_terrain.csv");
	navigateCSV.load(U"csv/GameAwa_navigation.csv");
	enemyCSV.load(U"csv/GameAwa_enemy.csv");
	Terminal::GetInst().logs.clear();
	//terrCSV.load(U"csv/Stage1._コイン.csv");
	//terrCSV.load(U"Stage/Stage1._地形.csv");
	//enemyCSV.load(U"Stage/Stage1._敵.csv");
	//goal = InitializeGoal(Vec2(13200, 650));
	Cursor::SetDefaultStyle(CursorStyle::Hidden);
	//Cri3dposInitialize();
TERRAIN_:
	terrGrid.resize(terrCSV.columns(1), terrCSV.rows());
	for (auto z : step(terrCSV.rows()))
	{
		for (auto x : step(terrCSV.columns(1)))
		{
			int tile = GetTile(terrCSV, x, z);
			terrGrid[z][x] = tile;
		}
	}
	//Size terrainSize = { terrCSV.columns(1), terrCSV.rows() };//マップの大きさ｛ｘ、ｙ｝
	Size terrainSize = { terrGrid.width(),terrGrid.height() };//マップの大きさ｛ｘ、ｙ｝

	for (auto p : step(Size{ terrGrid.width(),terrGrid.height() })) {
		if (terrGrid[p.y][p.x] == 1)
			boxes << Box{p.x * 4.0,2,p.y * 4.0,4.0};

	}
NAVIGATE_:
	navigateGrid.resize(navigateCSV.columns(1), navigateCSV.rows());
	for (auto z : step(navigateCSV.rows()))
	{
		for (auto x : step(navigateCSV.columns(1)))
		{
			int tile = GetTile(navigateCSV, x, z);
			navigateGrid[z][x] = tile;
		}
	}
ENEMY_:
	enemyGrid.resize(enemyCSV.columns(1),enemyCSV.rows());
	for (auto z : step(enemyCSV.rows()))
	{
		for (auto x : step(enemyCSV.columns(1)))
		{
			int tile = GetTile(enemyCSV, x, z);
			enemyGrid[z][x] = tile;
		}
	}
	for (auto p : step(Size{ enemyGrid.width(),enemyGrid.height() })) {
		if (enemyGrid[p.y][p.x] == 1)
			enemys << std::make_shared<RangeEnemy>(Vec3{ p.x * 4.0,2,p.y * 4.0 });
		if (enemyGrid[p.y][p.x] == 2)
			enemys << std::make_shared<MeleeEnemy>(Vec3{ p.x * 4.0,2,p.y * 4.0 });

	}

	LoadNavigate(100);

	

	String str
	{
		U"The clear winds of Ihatovo,"
		U"the blue sky that is cool at the bottom even in summer,"
		U"the city of Molio decorated with beautiful forests,"
		U"and the waves of shimmering grass in the suburbs."
	};
	String lpsum
	{
		U"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
	};
	//logs.at(Logging::DetectEnemy) = str;
	
	player.collider.setPos(player.eyePosition);
}

void Game::update()
{
#if _DEBUG
	//InInitialize later
	if (KeyTab.down())
	{
		toggBillboard = not toggBillboard;
	}
	if (KeyU.down())
	{
		//enemys << std::make_shared<RangeEnemy>('t');
	}
	if (KeyI.down())
	{
		//enemys << std::make_shared<MeleeEnemy>('t');
	}
	if (KeyO.down())
	{
		Terminal::GetInst().txtIndex.push_back(Logging::DetectEnemy);
	}
	
	if (KeyK.down())
	{
		System::Exit();
	}
	ClearPrint();
	//Print << navigatePass.back().GetRadius();
	if (isGoal)
	{
		changeScene(GameState::Title);
	}
	//Terminal::GetInst().command[Logging::Damaged] = U"You look at{:.0f}degrees"_fmt(player.GetRadian());
#endif
	auto pad = XInput(0);
	Circle cir{ Arg::center(Padpos),50 };
	if (callonce)
	{
		Terminal::GetInst().command[Logging::startCaution] = U"警告：周囲に敵味方不明機を多数感知";
		Terminal::GetInst().command[Logging::startCaution2] = U"全てを敵機としてマークします";
		Terminal::GetInst().txtIndex.push_back(Logging::startCaution);
		Terminal::GetInst().txtIndex.push_back(Logging::startCaution2);
		AudioAsset(U"v敵味方不明機").playOneShot();
		callonce = false;
	}
	
	if(not AudioAsset(U"v経路更新").isPlaying()
		and not AudioAsset(U"v経路逸脱").isPlaying()
		and not AudioAsset(U"v衝突").isPlaying()
		and not AudioAsset(U"v命中せず").isPlaying()
		and not AudioAsset(U"vリロード中").isPlaying()
		and not AudioAsset(U"vリロード完了").isPlaying()
		and not AudioAsset(U"v弾倉ゼロ").isPlaying()
		and not AudioAsset(U"v撃破").isPlaying())
		{
			canPlay = true;
		}
	else
	{
		canPlay = false;
	}

	if (AudioAsset(U"v0end").isPlaying())
	{
		canPlay = false;
		endingExplain = false;
	}

	Enemy::canplay = canPlay;
	



	//ポーズ画面への移行

	if ((KeyEscape.down() or pad.buttonStart.down())
		and player.GetHealth() > 0)
	{
		isPaused = !isPaused;       // ESCが押されたらポーズ状態が反転
		menu = !menu;
		
	}


	if (isPaused)
	{
		if (SimpleGUI::ButtonAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100 })
			or (SimpleGUI::ButtonRegionAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100 }).intersects(cir) and pad.buttonA.down()))
		{
			changeScene(GameState::Title);
		}
		Cursor::RequestStyle(CursorStyle::Default);
		return;           // この時点でポーズ中ならリターン
	}
	menu = false;
	isPaused = false;

	player.SetOldEyePosition(player.eyePosition);

	ControlPlayer();

	

	ControlEnemy();


	

	NavigationRoot();

	if (isGoal)
	{
		changeScene(GameState::Result);
	}


	Cursor::SetPos(Scene::Center());

	player.collider.setPos(camera.getEyePosition());
	
	for (const auto& box : boxes)
	{
		const bool intersects = player.collider.intersects(box);
		if (intersects)
		{
			player.eyePosition = player.GetOldPosition();
			
		}
	}
	SonarAround();
	
	

}

void Game::SonarAround()
{
	for (auto&& [i,sensor] : IndexedRef(player.sensors()))
	{
		sensor.sonar.setOrigin(Float3{player.collider.center});
		sensor.sonar.setDirection(GetDirection(player.GetRadian() + 45_deg * i));
		sensor.distanceToBox = none;
		for (auto& box : boxes)
		{
			if (const std::optional<float>& dist = sensor.sonar.intersects(box))
			{
				if (dist < sensor.distanceToBox.value_or(Math::Inf))
				{
					sensor.distanceToBox = dist;
				}
			}
			
		}
	}
	
}

void Game::draw()const
{
	auto pad = XInput(0);
	// 3D 描画
	{
		// renderTexture を背景色で塗りつぶし、
		// renderTexture を 3D 描画のレンダーターゲットに
		const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
		Plane{ 64 }.draw(uvChecker);

		for (auto& enemy : enemys)
		{
			enemy->GetCollider().draw();
		}
		for (auto& box : boxes)
		{
			box.draw();
		}
		for (auto& navi : navigatePass)
		{
			navi.GetNode()
			.scaled(0.25)
			.draw(Linear::Palette::Green);
		}
	}

	// 通常のシーン描画
	// 3D シーンを 2D シーンに描画
	{
		Graphics3D::Flush();
		renderTexture.resolve();
		Shader::LinearToScreen(renderTexture);
	}

	Draw2D();
	Draw2DLightBloomed();
	if (isPaused)
	{
		Rect{Scene::Size()}.draw(ColorF{0,0,0,0.8});
		SimpleGUI::Slider(U"エイム感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{ Scene::Center().x - 500,150 }, 500, 400);
		if (pad.isConnected())
		{
			TextureAsset(U"controllerDesc")
				.scaled(0.9,0.9)
				.drawAt(Scene::Center() + Vec2{ 0, 150 });
		}
		else
		{

			TextureAsset(U"description")
				.scaled(0.2, 0.2)
				.drawAt(Scene::Center() + Vec2{ 0,200 });
		}

		if (SimpleGUI::ButtonAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100}))
		{
			//changeScene(GameState::Title);
		}

		FontAsset(U"meiryob")
		(	U"///////////////////////////////////////////////"
			U"PAUSE"
			U"///////////////////////////////////////////////")
			.drawAt(Vec2{ Scene::Center().x,300 },Palette::Darkorange);

		if (pad.leftThumbX)
		{
			Padpos.x += pad.leftThumbX * Scene::DeltaTime() * 500;
		}
		if (pad.leftThumbY)
		{
			Padpos.y -= pad.leftThumbY * Scene::DeltaTime() * 500;
		}
		if (pad.buttonLB.down())
		{
			getData().sensi -= 0.1;
		}
		if (pad.buttonRB.down())
		{
			getData().sensi += 0.1;
		}
		if (pad.isConnected())
		{
			drawCur(Padpos);
			FontAsset(U"meiryob")(U"L").draw(Vec2{ Scene::Center().x - 500 + 450,170 }, Palette::Gray);
			FontAsset(U"meiryob")(U"R").draw(Vec2{ Scene::Center().x - 500 + 500 + 400,170 }, Palette::Gray);
		}
		
		return;//本来ここでReturnすべきでないがライトブルームを消すためにリターンする
	}
	
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



bool Game::isExistRightside(Vec3 v1, Vec3 v2)
{
	auto castXMVector = [=](Vec3 src)->DirectX::XMVECTOR {
		DirectX::XMFLOAT3 temp{
			static_cast<float>(src.x),
			static_cast<float>(src.y),
			static_cast<float>(src.z),
		};
		return DirectX::XMLoadFloat3(&temp);
	};

	DirectX::XMVECTOR xmv1 = castXMVector(v1);
	DirectX::XMVECTOR xmv2 = castXMVector(v2);

	xmv1 = DirectX::XMVector3Normalize(xmv1);
	xmv2 = DirectX::XMVector3Normalize(xmv2);
	DirectX::XMVECTOR xmResult = DirectX::XMVector3Cross(xmv1, xmv2);

	if (DirectX::XMVectorGetY(xmResult) > 0)
	{
		return true;
	}
	return false;

	//正の値であるとき敵の中心は右にある
	//ArcSinでマイナス９０度～９０度の範囲での角度が出るが
	//あくまで左右判定のみに使用し角度の取り出しには使わない
	//内積を３６０度系に正規化する
	//ここでは常に角度をラジアンで扱っている
}

float Game::CalcRadLeftAndRightDetail(Vec3 v1, Vec3 v2)
{
	auto castXMVector = [=](Vec3 src)->DirectX::XMVECTOR {
		DirectX::XMFLOAT3 temp{
			static_cast<float>(src.x),
			static_cast<float>(src.y),
			static_cast<float>(src.z),
		};
		return DirectX::XMLoadFloat3(&temp);
	};

	DirectX::XMVECTOR xmv1 = castXMVector(v1);
	DirectX::XMVECTOR xmv2 = castXMVector(v2);

	xmv1 = DirectX::XMVector3Normalize(xmv1);
	xmv2 = DirectX::XMVector3Normalize(xmv2);
	DirectX::XMVECTOR xmResult = DirectX::XMVector3Cross(xmv1, xmv2);
	return DirectX::XMVectorGetY(xmResult);

}


void Game::LoadNavigate(int32 maxNode)
{

	//Size terrainSize = { terrGrid.width(),terrGrid.height() };//マップの大きさ｛ｘ、ｙ｝
	int32 loadingCurrentNode = 1;
	while (true)
	{
		bool isLastNode = true;
		//無限ループを回して二次元配列の中の数字を一つずつ順番に読み込む
		for (auto p : step(Size(navigateGrid.width(), navigateGrid.height()))) {
			if (navigateGrid[p.y][p.x] == 0)
				player.eyePosition = Vec3{ p.x * 4.0,2,p.y * 4.0 };
			if (navigateGrid[p.y][p.x] == loadingCurrentNode)
			{
				navigatePass << NavigateNode{ Vec3{ p.x * 4.0,2,p.y * 4.0} };
				isLastNode = false;
			}
		}
		//最大数に達したらBreak 無限ループ回避のために100回回ると強制的に脱出する
		if(isLastNode == true)
		{
			break;
		}
		if (loadingCurrentNode == maxNode)
		{
			break;
		}
		loadingCurrentNode++;
	}
	
}

void Game::ControlPlayer()
{
	auto pad = XInput(0);
	const double speed = (Scene::DeltaTime() * 2.0 * 5);
	double rate = 1.0;
	if (KeyA.pressed() || KeyD.pressed()) {
		if (KeyS.pressed() || KeyW.pressed()) {
			//斜めの場合は、移動量の倍率係数を０．７１に設定
			rate = Math::Constants::InvSqrt2;
		}
	}
	if (pad.leftThumbX < -0.1 or pad.leftThumbX > 0.1) {
		if (pad.leftThumbY < -0.1 or pad.leftThumbY > 0.1){
			rate = Math::Constants::InvSqrt2;
		}
	}
	if (player.GetHealth() > 0)
	{

		
		if (KeyW.pressed() or pad.leftThumbY > 0.1)
		{
			player.eyePosition += (GetDirection(player.GetRadian()) * speed * rate);
		}

		if (KeyS.pressed() or pad.leftThumbY < -0.1)
		{
			player.eyePosition += (-GetDirection(player.GetRadian()) * speed * rate);
		}

		if (KeyA.pressed() or pad.leftThumbX < -0.1)
		{
			player.eyePosition += (GetDirection(player.GetRadian() - 90_deg) * speed * rate);

		}

		if (KeyD.pressed() or pad.leftThumbX > 0.1)
		{
			player.eyePosition += (GetDirection(player.GetRadian() + 90_deg) * speed * rate);
		}


		ControlFirearm();
		//0.015秒毎に1ずつ回復
		if (player.GetRegenerateTimer().elapsed() > 5s
			and player.GetRegenerateTimer().elapsed() - 5s > 15ms)
		{
			player.SetHealth(player.GetHealth() + 1);
			player.SetHealth(s3d::Min(player.GetHealth(),1000));
		}

	}
	else
	{
		EnableDeadProtocol();
	}

	player.SetRadian(player.GetRadian() + Cursor::DeltaRaw().x * 0.0009 * getData().sensi);

	player.SetRadian(player.GetRadian() + pad.rightThumbX * 0.01 * getData().sensi);

	if (player.GetRadian() < 0_deg)
	{
		player.SetRadian(player.GetRadian() + 360_deg);
	}

	if (360_deg < player.GetRadian())
	{
		player.SetRadian(player.GetRadian() - 360_deg);
	}

	// 位置・注目点情報を更新
	//入力検知のあとで更新する必要がある
	camera.setView(player.eyePosition, GetFocusPosition(player.eyePosition, player.GetRadian()));
	//s3d::Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(player.GetRadian()));
	//s3d::Print << U"direction: {:.2f}"_fmt(GetDirection(player.GetRadian()));
	//s3d::Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	//s3d::Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
	Graphics3D::SetCameraTransform(camera);

}

void Game::EnableDeadProtocol()
{
	//Terminal::GetInst().txtIndex.clear();
	enemys.clear();
	player.regenerateTimer().reset();
	deadProtocol.start();
	if (deadProtocol.elapsed() > 2s)
	{
		static int32 progress = 0;
		switch (progress)
		{
		case 0:
			Terminal::GetInst().command[Logging::DeadProtocolFirst] = U"危険：致命的な損傷";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocolFirst);
			AudioAsset(U"v致命的な損傷").playOneShot();
			break;
		case 1:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"強制的に完全防御システムを起動します";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			break;
		case 3:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"[PHILOSOPCHICAL_DEADLOCK]プロトコル作動";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			AudioAsset(U"v哲学的な死").playOneShot();
			break;
		case 4:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"3...";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			break;
		case 5:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"2...";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			AudioAsset(U"v15game").playOneShot();
			break;
		case 6:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"1...";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			AudioAsset(U"v16game").playOneShot();
			break;
		case 7:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"You lost control";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			AudioAsset(U"vコントロールロスト").playOneShot();
			break;
		case 8:
			Terminal::GetInst().command[Logging::DeadProtocol] = U"GOODBYE";
			Terminal::GetInst().txtIndex.push_back(Logging::DeadProtocol);
			AudioAsset(U"vさようなら").playOneShot();
			break;
		case 9:
			progress = 0;
			changeScene(GameState::Title);
			return;
			break;
		default:
			break;
		}
		deadProtocol.restart();
		++progress;
	}
	//changeScene(GameState::Title);
}

void Game::NavigationRoot()
{
	//ナビゲーションの処理
	NavigateNode nextDestination = navigatePass.at(player.GetPassedNode());
	Vec3 nextNode = nextDestination.GetNode().center - camera.getEyePosition();
	player.SetNextNodeDistance(nextNode.length());
	if (player.collider.intersects(nextDestination.GetNode())
		and nextDestination.GetNode().center not_eq navigatePass.back().GetNode().center)
	{
		//プレイヤーのコライダーと接触したとき最後のノードでなければ次のノードを指す
		//andを外すとOut of range
		player.incrementPassedNode();
		Terminal::GetInst().command[Logging::UpdateNavigate] = U"目標への経路を更新";
		Terminal::GetInst().txtIndex.push_back(Logging::UpdateNavigate);
		if (canPlay)
		{
			AudioAsset(U"v経路更新").play();
		}
	}
	if (player.collider.intersects(nextDestination.GetNode())
		and (nextDestination.GetNode() == navigatePass.back().GetNode()))
		{
			isGoal = true;
		}
	if (nextDestination.GetNode() == navigatePass.back().GetNode()
		and player.GetNextNodeDistance() < 60)
	{
		if(endingExplain)
		{
			AudioAsset(U"v0end").play();
			Terminal::GetInst().command[Logging::ending] = U"高エネルギー放射検知 | 高次元物質の低次元展開を観測";
			Terminal::GetInst().txtIndex.push_back(Logging::ending);
			Terminal::GetInst().command[Logging::ending2] = U"単純接触によるエネルギー供給を行えば";
			Terminal::GetInst().txtIndex.push_back(Logging::ending2);
			Terminal::GetInst().command[Logging::ending3] = U"７４％ほどの確率で損傷した視床の回復が可能です";
			Terminal::GetInst().txtIndex.push_back(Logging::ending3);
		}
	}
	if (player.GetPassedNode() > 0)
	{
		//HACK:よく見たら距離をとっているだけで前進したときもここの処理を走っている
		// 雑なフェイルセーフとしては機能しているのでこのままでもいい
		//通過したノードが１以上あるときナビゲーションを逆走した場合に
		//自分の一番近いノードを指す
		Vec3 betweenNodes{
			navigatePass.at(player.GetPassedNode()).GetNode().center
			- navigatePass.at(player.GetPassedNode() - 1).GetNode().center
		};
		Vec3 betweenPlayerAndNode{
			camera.getEyePosition()
			- navigatePass.at(player.GetPassedNode()).GetNode().center
		};
		if (betweenPlayerAndNode.length() > betweenNodes.length() + 2 * 4/*NodeRadius + playerRadius*/)
		{
			//player.decrementPassedNode();
			float minDistance = Math::Inf;
			int32 nearestNodeIndex = 0;//何かが起こってもとりあえず一番初めのノードを指す
			for (auto&& [i, node] : Indexed(navigatePass))
			{
				Vec3 distance = camera.getEyePosition() - node.GetNode().center;
				if (distance.length() < minDistance)
				{
					minDistance = distance.length();
					nearestNodeIndex = i;
				}
			}
			if (navigatePass.at(player.GetPassedNode()).GetNode() not_eq navigatePass.at(nearestNodeIndex).GetNode())
			{
				Terminal::GetInst().command[Logging::ReverseNavigate] = U"警告：経路から逸脱...ルートを再構築しました";
				Terminal::GetInst().txtIndex.push_back(Logging::ReverseNavigate);
				if (canPlay)
				{
					AudioAsset(U"v経路逸脱").play();
				}
			}
			
			player.SetPassedNode(nearestNodeIndex);
		}
	}

	//次のナビゲーションノードとカメラの視線ベクトルの内積を使って角度を求め、
	//外積を使って角度を３６０度系に正規化
	//常にラジアンで計算している
	Vec3 naviVec = nextDestination.GetNode().center - camera.getEyePosition();
	double dot = Math::Acos(GetDot(camera.getLookAtVector(), Normalize(naviVec)));
	dot = isExistRightside(camera.getLookAtVector(), Normalize(naviVec)) ? dot : 2 * Math::PiF - dot;
	player.interfaces.arrowRadian = dot;
}

void Game::ControlEnemy()
{
	for (auto&& enemy : enemys)
	{
		//HACK:ここらへんの処理のフロー気持ち悪いので改善したい
		//敵からのRayを生成
		Vec3 norm = Normalize(camera.getEyePosition() - enemy->GetCollider().center);
		Ray ray{ enemy->GetCollider().center,norm };
		enemy->SetDistanceNearestBox(Math::Inf);
		enemy->SetDistanceToPlayer(ray.intersects(player.collider));
		for (const auto& box : boxes)
		{
			if (const std::optional<float>& dist = ray.intersects(box))
			{
				if (*dist < enemy->GetDistanceNearestBox())
				{
					enemy->SetDistanceNearestBox(dist.value());
				}
			}
		}

		Vec3 front = camera.getLookAtVector();
		Vec3 playerLookAtEnemy = enemy->GetCollider().center - camera.getEyePosition();

		//敵とプレイヤーの距離と敵からの一番近い壁との距離を比較
		//プレイヤーの方が距離が短ければ気づかれる
		if (enemy->GetDistanceToPlayer().value() < enemy->GetDistanceNearestBox().value() and enemy->GetDistanceToPlayer().value() < 50)
		{
			enemy->SetAudioPanRad(CalcRadLeftAndRightDetail(front, playerLookAtEnemy));
			//敵との角度の差
			double angDiffRad = Math::Acos(GetDot(Normalize(front), Normalize(playerLookAtEnemy)));

			angDiffRad = isExistRightside(front, playerLookAtEnemy) ? angDiffRad : 2 * Math::PiF - angDiffRad;
			enemy->SetAngleDiffs(angDiffRad);
			enemy->foundPlayer();
		}


		if (enemy->GetIsNotice())
		{

			enemy->SetOldPosition(enemy->GetCollider().center);
			Vec3 vec = camera.getEyePosition() - enemy->GetCollider().center;
			Vec3 moveLng = Normalize(vec) * Scene::DeltaTime() * enemy->GetVelocity();
			if (not enemy->GetCollider().intersects(player.collider))
			{
				enemy->collider().center += moveLng;
			}
			//敵との角度の差
			double angDiffRad = Math::Acos(GetDot(Normalize(front), Normalize(playerLookAtEnemy)));


			//ここでは常に角度をラジアンで扱っている
			angDiffRad = isExistRightside(front, playerLookAtEnemy) ? angDiffRad : 2 * Math::PiF - angDiffRad;
			enemy->SetAngleDiffs(angDiffRad);



			//Enemyの種類別の動きはoverrideした派生クラス内の関数に記述する
			enemy->Move(player);

			for (const auto& box : boxes)
			{
				if (box.intersects(enemy->GetCollider()))
				{
					enemy->collider().center = enemy->GetOldPosition();
				}
			}
			//敵に見つかっている時、
			//遮蔽に隠れて5秒以上経つと敵はプレイヤーを見失う
			//フラグ管理のため、
			//::foundPlayer()でnoticeTimerをリセットしている
			if (enemy->GetDistanceToPlayer() > enemy->GetDistanceNearestBox())
			{
				enemy->noticeTimer().start();
				if (enemy->GetNoticeTimer().elapsed() > 5000ms)
				{
					enemy->LostSightOfPlayer();
				}
			}

		}



	}

	s3d::Erase_if(enemys, [](std::shared_ptr<Enemy> enemy) {return enemy->GetCollider().r == 0; });
}

void Game::Draw2D() const
{
#if _DEBUG
	if (toggBillboard)
	{
		Rect{ Scene::Size() }.draw(Palette::Black);
	}

#else
	Rect{ Scene::Size() }.draw(Palette::Black);

#endif // _DEBUG

	if (not player.GetFireCoolTimer().isRunning())
	{
		RectF{ Arg::leftCenter(Scene::Center().x - 250,250),Size{500,50} }.draw(Palette::White);
		FontAsset(U"meiryo")(U"RDY GUN").drawAt(Vec2{ Scene::Center().x, 250 }, Palette::Orangered);
	}
	else
	{
		RectF{ Arg::leftCenter(Scene::Center().x - 250,250),SizeF{player.GetFireCoolTimer().ms(),50} }.draw(Palette::Grey);
		FontAsset(U"meiryo")(U"{:.2f}"_fmt(500 - player.GetFireCoolTimer().msF())).drawAt(Vec2{ Scene::Center().x, 250 }, Palette::Tomato);
	}
	TextureAsset(U"navigateArrow")
		.scaled(1.2,1.2)
		.rotated(player.interfaces.arrowRadian)
		.drawAt(Scene::Center().x, Scene::Height() - 150);
	TextureAsset(U"HPGaugeFrame").drawAt(Scene::Center().x,180);
	//現在値/最大値＝割合
	//(player.GetHealth()/100) * texture2d_desc.width
	double ratio = static_cast<double>(player.GetHealth()) / 1000;
	TextureAsset(U"HPGauge")(0,0, ratio * 640, 64).draw(Scene::Center().x - 320, 146);
	TextureAsset(U"radar")
		.scaled(0.2,0.2)
		.drawAt(Scene::Size().x - 300,300);
		TextureAsset(U"radarCenter")
		.scaled(0.15,0.15)
		.drawAt(Scene::Size().x - 300,300,Palette::Orangered);

	for (const auto& [i, sensor] : Indexed(player.GetSensors()))
	{
		if (sensor.distanceToBox.has_value()
		and sensor.distanceToBox.value() < 30)
		{
			
			TextureAsset(U"lidar")
			.scaled(0.25,0.25)
			.rotated(i * 45_deg)
			.drawAt(Scene::Size().x - 300,300);
			

		}
		if (sensor.distanceToBox.has_value()
		and sensor.distanceToBox.value() < 10)
		{
			TextureAsset(U"lidar")
				.scaled(0.25, 0.25)
				.rotated(i * 45_deg)
				.drawAt(Scene::Size().x - 300, 300,Palette::Yellow);


		}
		if (sensor.distanceToBox.has_value()
		and sensor.distanceToBox.value() < 3.5)
		{
			TextureAsset(U"lidar")
				.scaled(0.25, 0.25)
				.rotated(i * 45_deg)
				.drawAt(Scene::Size().x - 300, 300,Palette::Red);
			if (walltimer.elapsed() > 3.0s and player.GetHealth() > 0)//HACK:ifでのバグ潰しはなるべく避ける
			{
				walltimer.restart();
				Terminal::GetInst().command[Logging::CollideWall] = U"警告：壁と衝突しています";
				Terminal::GetInst().txtIndex.push_back(Logging::CollideWall);
				if (canPlay)
				{
					AudioAsset(U"v衝突").play();
				}
			}

		}
		
	}
}

void Game::Draw2DLightBloomed() const
{
	TextureAsset(U"crossHair").drawAt(Scene::Center());
	
	TextureAsset(U"HELgunIcon").drawAt(Scene::Size() * 0.9);
	FontAsset(U"meiryob")(U"00{} | ∞"_fmt(player.GetMagazine()))
		.drawAt(Scene::Size().x - 500,Scene::Size().y - 100);
	FontAsset(U"meiryo")(U"{:.0f}"_fmt(ToDegrees(player.GetRadian())))
		.drawAt(Scene::Center().x,130);

	FontAsset(U"meiryob")(U"{:.0f}Unit"_fmt(player.GetNextNodeDistance()))
		.drawAt(Scene::Center().x, Scene::Height() - 150,Palette::Orangered);
	FontAsset(U"meiryob")(U"目標地点：{}/{}"_fmt(player.GetPassedNode(),navigatePass.size()))
		.drawAt(Scene::Size().x - 256,Scene::Size().y * 0.8);

	Terminal::GetInst().StreamTxt();
	//DrawTxt();
	for (const auto& enemy : enemys)
	{
		if (enemy->GetIsNotice())
		{
			
			if (enemy->GetIsFired())
			{
				TextureAsset(U"indicater")
					.scaled(0.25, 0.25)
					.rotated(enemy->GetAngleDiffs())
					.drawAt(Scene::Center(),Palette::Red);
			}
			else
			{
				TextureAsset(U"indicater")
					.scaled(0.25, 0.25)
					.rotated(enemy->GetAngleDiffs())
					.drawAt(Scene::Center());
			}
		}
	}
	//ラジアンから度の変換
	int anglenomal = ToDegrees(player.GetRadian());
	// 画像の (angle, 0) から幅 600, 高さ 50 の長方形部分
	//compass(anglenomal * 5.3 + 200, 0, 400, 64).draw(800, 50);
	const double region = (11335.0 - 1665.0 * 2.0) / 360.0;//一度進むたびにTextureRegionのｘ座標をどれだけずらすか
		TextureAsset(U"compass")(710.0 + ToDegrees(player.GetRadian()) * region,0,Scene::Size().x,267.0)
		.scaled(0.25,0.25)
		.drawAt(Scene::Center().x, 64);
		//TextureAsset(U"compass")
		//.drawAtClipped(Scene::Center().x, 64,RectF{300});
	/*TextureAsset(U"compass")
		(ToDegrees(player.GetRadian()) * region,0,Size{1000,500})
		.drawAt(Scene::Center().x, 400);*/
}



void Game::HitConfirmPlayerShot()
{
	Optional<float> distBox = Math::Inf;
	Optional<float> distEnemy = Math::Inf;
	Optional<float> minDistBox = Math::Inf;
	Optional<float> minDistEnemy = Math::Inf;
	Optional<size_t> enemyIndex = none;
	for (const auto& box : boxes)
	{
		if (distBox = camera.screenToRay(Scene::Center()).intersects(box))
		{
			if (*distBox < minDistBox)
			{
				minDistBox = *distBox;
			}
		}
	}
	for (const auto& [i, enemy] : Indexed(enemys))
	{
		if (camera.screenToRay(Scene::Center()).intersects(enemy->GetCollider()))
		{
			distEnemy = camera.screenToRay(Scene::Center()).intersects(enemy->GetCollider());
			if (*distEnemy < minDistEnemy)
			{
				minDistEnemy = *distEnemy;
				enemyIndex = i;
			}
		}
	}
	if (*minDistEnemy < *minDistBox)
	{
		if (enemyIndex.has_value())
		{
			enemys.at(*enemyIndex)->collider().setR(0);
			AudioAsset(U"brokenEnemy").playOneShot(MixBus0,1.0,enemys.at(*enemyIndex)->GetAngleDiffs());
			Terminal::GetInst().command[Logging::DestroyEnemy] = U"敵を撃破";
			Terminal::GetInst().txtIndex.push_back(Logging::DestroyEnemy);
			if (canPlay)
			{
				AudioAsset(U"v撃破").play();
			}
		}
	}
	else
	{
		Terminal::GetInst().command[Logging::PoorAim] = U"警告：遮蔽物に着弾 | 敵に命中せず";
		Terminal::GetInst().txtIndex.push_back(Logging::PoorAim);
		if (canPlay)
		{
			AudioAsset(U"v命中せず").play();
		}
		//外した時のテキスト要らない気がする
		// でもIntersectAtで音の定位は作るべきか？
		// 正直なくてもいい
		//弾が壁に当たった時の音
	}
}
void Game::ControlFirearm()
{
	auto pad = XInput(0);
	//Rを押したときプレイヤーの現在の弾倉が最大より少なく、
	//リロード中でなければリロードが開始される
	if ((KeyR.down() or pad.buttonX.down())
		and player.GetMagazine() < player.GetMaxMagazine()
		and not player.GetReloadTimer().isRunning())
	{
		player.reloadTimer().start();
		AudioAsset(U"reloadStart").playOneShot();
		Terminal::GetInst().command[Logging::Reloading] = U"リロード中です...";
		Terminal::GetInst().txtIndex.push_back(Logging::Reloading);
		if (canPlay)
		{
			AudioAsset(U"vリロード中").play();
		}
	}
	//3秒経ったらリロードが完了する
	if (player.GetReloadTimer().elapsed() > player.GetReloadTime())
	{
		Terminal::GetInst().command[Logging::ReloadEnd] = U"リロード完了";
		Terminal::GetInst().txtIndex.push_back(Logging::ReloadEnd);
		AudioAsset(U"reloadEnd").playOneShot();
		player.HaveReloadedGun();
		player.reloadTimer().reset();
		if (canPlay)
		{
			AudioAsset(U"vリロード完了").play();
		}
	}
	//左クリックしたときプレイヤーが前回の射撃から、
	//ある程度時間が経過しており、
	//弾倉が１以上あるときに、
	//リロード中でいなければ撃てる
	if (pad.rightTrigger == 0.0)
	{
		isRT = true;
	}
	if ((MouseL.down() or pad.rightTrigger > 0.3 and isRT)
		and not player.GetFireCoolTimer().isRunning()
		and player.GetMagazine() > 0
		and not player.reloadTimer().isRunning())
	{
		isRT = false;
		player.Fire();
		//当たったかどうかの確認をここで行う
		HitConfirmPlayerShot();
		player.fireCoolTimer().start();
		AudioAsset(U"shot").playOneShot(MixBus1, 0.7, 0);
	}

	if ((MouseL.down() or pad.rightTrigger > 0.3 and isRT)
		and not player.GetFireCoolTimer().isRunning()
		and player.GetMagazine() == 0//弾倉が０のとき
		and not player.reloadTimer().isRunning())
	{
		AudioAsset(U"noAmmo").playOneShot();
		Terminal::GetInst().command[Logging::NoAmmo] = U"警告：弾倉内の残弾数が０です";
		Terminal::GetInst().txtIndex.push_back(Logging::NoAmmo);
		if (canPlay)
		{
			AudioAsset(U"v弾倉ゼロ").play();
		}
	}

	if (pad.rightTrigger > 0.3)
	{
		isRT = false;
	}

	if (player.GetFireCoolTimer().elapsed() > player.GetFireCoolTime())
	{
		player.fireCoolTimer().reset();
	}
}

void Game::RangeEnemy::Move(Player& player)
{
	if (GetFireIndicater().elapsed() > 2s)
	{
		fireIndicater().reset();
		SetIsFire(false);
	}
	if (GetDistanceToPlayer() < GetAtkRange()
	and GetDistanceToPlayer() < GetDistanceNearestBox())
	{
		//発見されてから一秒間は攻撃されない（納得感のため）
		// ただし正確には発見されていてかつ攻撃範囲内かつ遮蔽がない場合から1秒間なので体感は長いはず
		// このタイマーはLostOfSight()でリセットされる
		//敵の攻撃性はその敵一体と見合ったとき棒立ちで何秒耐えられるかを基準にする
		//複数体に囲まれることを考慮すると１０秒以上か？要検討
		foundAtkCoolTimer().start();
		if (GetFoundAtkCoolTimer().elapsed() > 1s)
		{
		//millisecond/percent = 期待値
		//この場合の期待値は1秒間に1回80ダメージ、10秒で800、13秒で1040（死ぬ）
		//プレイヤーは自動回復するのでゆっくり進めばそうそう死なないだろうという想定
			atkInterval().start();
			if (GetAtkInterval().elapsed() > 500ms
			and RandomBool(0.5))
			{
				AudioAsset(U"rangeAtk").playOneShot(GetAudioFalloff(),GetAudioPanRad());
				player.SetHealth(player.GetHealth() - GetAtk());
				player.regenerateTimer().restart();
				fireIndicater().start();
				SetIsFire(true);
				Terminal::GetInst().command[Logging::Damaged] = U"警告：方位{:.0f} | 距離{:.0f}より被弾"_fmt(ToDegrees(GetAngleDiffs()), GetDistanceToPlayer().value());
				//方位angle()、距離Lengthより被弾;
				Terminal::GetInst().txtIndex.push_back(Logging::Damaged);
				if (canplay)
				{
					AudioAsset(U"v被弾").play();
				}
			}
			
		}
	}
	if (GetAtkInterval().elapsed() > 500ms)
	{

		atkInterval().reset();
	}
	if (GetDistanceToPlayer().value_or(9999) < 10)
	{
		SetAudioFalloff(1.0);
	}
	else
	{
		//距離50の時音の減衰率は100％
		//敵のスポーン時に何故かOptionalのBad_accessが送出されるのでとりあえず１００％の音を鳴らす
		double falloff = 1.0 - static_cast<double>(GetDistanceToPlayer().value_or(1) / 50);
		falloff = Clamp(falloff, 0.0, 1.0);
		SetAudioFalloff(falloff);
	}
	if (GetFootStepTimer().elapsed() > 500ms)
	{
		//足音
		AudioAsset(U"rangeFootStep").playOneShot(MixBus0, GetAudioFalloff(), GetAudioPanRad());
		footStepTimer().restart();
	}
	
}

void Game::MeleeEnemy::Move(Player& player)
{
	if (GetFireIndicater().elapsed() > 2s)
	{
		fireIndicater().reset();
		SetIsFire(false);
	}
	if (GetDistanceToPlayer() < GetAtkRange()
	and GetDistanceToPlayer() < GetDistanceNearestBox())
	{
		//発見されてから一秒間は攻撃されない（納得感のため）
		// ただし正確には発見されていてかつ攻撃範囲内かつ遮蔽がない場合から1秒間なので体感は長いはず
		// このタイマーはLostOfSight()でリセットされる
		//敵の攻撃性はその敵一体と見合ったとき棒立ちで何秒耐えられるかを基準にする
		//近接攻撃する敵は命中率を考慮せず攻撃力も高めがよいだろう
		//一撃で3割ほど減らしてみる　これを10秒間に3回ぐらい振るとTTKは10秒よりやや長くなる
		foundAtkCoolTimer().start();
		if (GetFoundAtkCoolTimer().elapsed() > 1s)
		{
			atkInterval().start();
			if (GetAtkInterval().elapsed() > 2000ms)
			{
				AudioAsset(U"meleeAtk").playOneShot(GetAudioFalloff(), GetAudioPanRad());
				player.SetHealth(player.GetHealth() - GetAtk());
				player.regenerateTimer().restart();
				fireIndicater().start();
				SetIsFire(true);
				Terminal::GetInst().command[Logging::Damaged] = U"警告：方位{:.0f} | 距離{:.0f}より被弾"_fmt(ToDegrees(GetAngleDiffs()),GetDistanceToPlayer().value());
				Terminal::GetInst().txtIndex.push_back(Logging::Damaged);
				if (canplay)
				{
					AudioAsset(U"v被弾").play();
				}
			}
			
		}
	}
	if (GetAtkInterval().elapsed() > 2000ms)
	{
		atkInterval().reset();
	}
	if (GetDistanceToPlayer().value_or(9999) < 10)
	{
		SetAudioFalloff(1.0);
	}
	else
	{
		//距離50の時音の減衰率は100％
		//敵のスポーン時に何故かOptionalのBad_accessが送出されるのでとりあえず１００％の音を鳴らす
		double falloff = 1.0 - static_cast<double>(GetDistanceToPlayer().value_or(1) / 50);
		falloff = Clamp(falloff, 0.0, 1.0);
		SetAudioFalloff(falloff);
	}
	if (GetFootStepTimer().elapsed() > 500ms)
	{
		//足音
		AudioAsset(U"meleeFootStep").playOneShot(MixBus0,GetAudioFalloff(),GetAudioPanRad());

		footStepTimer().restart();
	}
}

void Game::TutoEnemy::Move(Player& player)
{
	
	if (GetDistanceToPlayer().value_or(9999) < 10)
	{
		SetAudioFalloff(1.0);
	}
	else
	{
		//距離50の時音の減衰率は100％
		//敵のスポーン時に何故かOptionalのBad_accessが送出されるのでとりあえず１００％の音を鳴らす
		double falloff = 1.0 - static_cast<double>(GetDistanceToPlayer().value_or(1) / 50);
		falloff = Clamp(falloff, 0.0, 1.0);
		SetAudioFalloff(falloff);
	}
	if (GetFootStepTimer().elapsed() > 500ms)
	{
		//足音
		AudioAsset(U"rangeFootStep").playOneShot(MixBus0, GetAudioFalloff(), GetAudioPanRad());
		footStepTimer().restart();
	}
}


void Game::RangeEnemy::PlayNoticeSound()
{
	AudioAsset(U"notice").playOneShot(0.45,GetAudioPanRad());
}
void Game::MeleeEnemy::PlayNoticeSound()
{
	AudioAsset(U"notice").playOneShot(0.45, GetAudioPanRad());
}

void Game::Terminal::StreamTxt()
{
	
	if(not txtIndex.empty())
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
			case Logging::Damaged:
				color = Palette::Red;
				if (isHighlightedOrange(glyph.codePoint))
				{
					color = Palette::Orangered;
				}
				break;
			case Logging::DeadProtocolFirst:
			case Logging::TutorialDescCaution:
				color = Palette::Red;
				break;
			case Logging::CollideWall:
			case Logging::ReverseNavigate:
			case Logging::NoAmmo:
			case Logging::PoorAim:
			case Logging::startCaution:
			case Logging::startCaution2:
				color = Palette::Gold;
				break;
			case Logging::Tutorial:
			case Logging::Tutorial2:
			case Logging::ending:
			case Logging::ending2:
			case Logging::ending3:
				color = Palette::Darkgreen;
				break;
			default:
				color = Palette::White;
				if (isHighlightedOrange(glyph.codePoint))
				{
					color = Palette::Orangered;
				}
			case Logging::DeadProtocol:
			case Logging::TutorialDesc:
				break;
			}
			//改行文字の場合特別な処理が必要だがそもそもTerminalに改行文字は渡さないようにする
			
			glyph.texture.draw(glyphPos + glyph.getOffset(),color);
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
					case Logging::Damaged:
						color = Palette::Red;
						if (isHighlightedOrange(glyph.codePoint))
						{
							color = Palette::Orangered;
						}
						break;
					case Logging::DeadProtocolFirst:
					case Logging::TutorialDescCaution:
						color = Palette::Red;
						break;
					case Logging::CollideWall:
					case Logging::ReverseNavigate:
					case Logging::NoAmmo:
					case Logging::PoorAim:
					case Logging::startCaution:
					case Logging::startCaution2:
						color = Palette::Gold;
						break;
					case Logging::Tutorial:
					case Logging::Tutorial2:
					case Logging::ending:
					case Logging::ending2:
					case Logging::ending3:
						color = Palette::Darkgreen;
						break;
					default:
						color = Palette::White;
						if (isHighlightedOrange(glyph.codePoint))
						{
							color = Palette::Orangered;
						}
					case Logging::DeadProtocol:
					case Logging::TutorialDesc:
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


void Game::tutorial()
{
#if _DEBUG
	//InInitialize later
	if (KeyTab.down())
	{
		toggBillboard = not toggBillboard;
	}
	if (KeyU.down())
	{
		//enemys << std::make_shared<RangeEnemy>('t');
	}
	if (KeyI.down())
	{
		//enemys << std::make_shared<MeleeEnemy>('t');
	}
	if (KeyO.down())
	{
		Terminal::GetInst().txtIndex.push_back(Logging::DetectEnemy);
	}

	if (KeyK.down())
	{
		System::Exit();
	}
	ClearPrint();
	//Print << navigatePass.back().GetRadius();
	if (isGoal)
	{
		changeScene(GameState::Result);
	}
	//Terminal::GetInst().command[Logging::Damaged] = U"You look at{:.0f}degrees"_fmt(player.GetRadian());
#endif
	auto pad = XInput(0);
	if (pad.isConnected())
	{
		skipoverrideCount = pad.buttonB.pressedDuration().count() * 100;
		if (pad.buttonB.up())
		{
			skipoverrideCount = 0;
		}
	}
	else

	{
		if (KeyB.pressed())
		{
			skipoverrideCount = KeyB.pressedDuration().count() * 100;
		}
		if (KeyB.up())
		{
			skipoverrideCount = 0;
		}
	}
	if (skipoverrideCount > 300)
	{
		changeScene(GameState::Game);
	}
	if (statusTimer.elapsed() > 5s and statusDesc)
	{
		switch (descState)
		{
		case 0:
			Terminal::GetInst().command[Logging::TutorialDesc] = U"操縦者の認知特性に基づきシステム言語を調整...完了";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDesc);
			AudioAsset(U"v認知特性").play();
			break;
		case 1:
			Terminal::GetInst().command[Logging::TutorialDescCaution] = U"警告：現在視界を喪失しています";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDescCaution);
			AudioAsset(U"v視界喪失").play();
			break;
		case 2:
			Terminal::GetInst().command[Logging::TutorialDesc] = U"作戦要綱を参照...<データが破損しています>";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDesc);
			AudioAsset(U"v作戦要綱").play();
			break;
		case 3:
			Terminal::GetInst().command[Logging::TutorialDesc] = U"至上命令を参照：必ず帰還せよ";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDesc);
			AudioAsset(U"v至上命令").play();
			break;
		case 4:
			Terminal::GetInst().command[Logging::TutorialDesc] = U"ミッションを再設定";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDesc);
			AudioAsset(U"v再設定").play();
			break;
		case 5:
			Terminal::GetInst().command[Logging::TutorialDesc] = U"操縦者の支援を開始します | HUD起動完了";
			Terminal::GetInst().txtIndex.push_back(Logging::TutorialDesc);
			AudioAsset(U"v支援開始").play();
			break;
		default:
			break;
		}
		++descState;
		statusTimer.restart();
	}
	if (descState > 6)
	{
		statusDesc = false;
	}
	if (statusDesc)
	{
		return;//状況の説明が終わるまでReturn
	}
	if (not tutorialTimer.isRunning())
	{
		Terminal::GetInst().command[Logging::Tutorial] = U"特殊状況下における復帰訓練を開始します...";
		Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
		Terminal::GetInst().command[Logging::Tutorial2] = U"B長押しで即座に終了、作戦が開始されます";
		Terminal::GetInst().txtIndex.push_back(Logging::Tutorial2);
		AudioAsset(U"v特殊状況下").play();
	}

	if (pad.isConnected())
	{
		skipCount = pad.buttonB.pressedDuration().count() * 100;
		if (pad.buttonB.up())
		{
			skipCount = 0;
		}
	}
	else

	{
		if (KeyB.pressed())
		{
			skipCount = KeyB.pressedDuration().count() * 100;
		}
		if (KeyB.up())
		{
			skipCount = 0;
		}
	}
	if (skipCount > 400.0)
	{
		changeScene(GameState::Game);
	}
	tutorialTimer.start();
	if (tutorialTimer.elapsed() > 10s)
	{
		switch (explainState)
		{
		case 0:
			if (pad.isConnected())
			{
				Terminal::GetInst().command[Logging::Tutorial] = U"左スティックで移動";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"v左スティック").play();
			}
			else
			{

				Terminal::GetInst().command[Logging::Tutorial] = U"WASDで移動";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"vWASD").play();
			}
			++explainState;
			break;
		case 1:
			if (pad.isConnected())
			{
				Terminal::GetInst().command[Logging::Tutorial] = U"右スティックで視点移動";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"v右スティック").play();
			}
			else
			{

				Terminal::GetInst().command[Logging::Tutorial] = U"マウスで視点移動";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"vマウス").play();
			}
			++explainState;
			break;
		case 2:
			Terminal::GetInst().command[Logging::Tutorial] = U"画面右上に障害物との位置推定ソナーを表示中です";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
			AudioAsset(U"v位置推定").play();
			++explainState;
			break;
		case 3:
			Terminal::GetInst().command[Logging::Tutorial] = U"画面下部に次の目標までの距離と方向を表示中です";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
			AudioAsset(U"v画面下部").play();
			++explainState;
			break;
		case 4:
			Terminal::GetInst().command[Logging::Tutorial] = U"画面中央に自機の耐久限界を表示中です";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
			AudioAsset(U"v耐久限界").play();
			++explainState;
			break;
		case 5:
			if (pad.isConnected())
			{
				Terminal::GetInst().command[Logging::Tutorial] = U"右トリガーで射撃、Xボタンでリロードが可能です";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"v射撃").play();
			}
			else
			{
				Terminal::GetInst().command[Logging::Tutorial] = U"左クリックで射撃、Rでリロードが可能です";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"v左クリック").play();
			}
			++explainState;
			break;
		case 6:
			Terminal::GetInst().command[Logging::Tutorial] = U"敵を発見した場合画面上に位置をリアルタイムで表示します";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
			Terminal::GetInst().command[Logging::Tutorial2] = U"８秒後に仮想の敵を召喚";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial2);
			AudioAsset(U"v敵を発見した場合").play();
			++explainState;
			break;
		case 7:
			boxes.clear();
			enemys << std::make_shared<TutoEnemy>(player.collider.center - Vec3{ 30,0,30 });
			Terminal::GetInst().command[Logging::Tutorial] = U"疑似信号により仮想の敵を召喚 | 撃破してください";
			Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
			AudioAsset(U"v疑似信号").play();
			++explainState;
			break;
		case 8:
			if (enemys.empty())
			{
				Terminal::GetInst().command[Logging::Tutorial] = U"復帰訓練終了...１０秒後に仮想空間を終了します";
				Terminal::GetInst().txtIndex.push_back(Logging::Tutorial);
				AudioAsset(U"v復帰訓練終了").play();
				++explainState;
			}
			break;
		default:
			++explainState;
			break;
		}
		tutorialTimer.restart();
	}
	
	if (explainState > 9)//caseMax + 1
	{
		getData().watchedTutorial = true;
		changeScene(GameState::Game);
	}




	//ポーズ画面への移行

	if ((KeyEscape.down() or pad.buttonStart.down())
		and player.GetHealth() > 0)
	{
		isPaused = !isPaused;       // ESCが押されたらポーズ状態が反転
		menu = !menu;

	}

	Circle cir{ Arg::center(Padpos),50 };
	if (isPaused)
	{
		if (SimpleGUI::ButtonAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100 })
			or (SimpleGUI::ButtonRegionAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100 }).intersects(cir) and pad.buttonA.down()))
		{
			changeScene(GameState::Title);
		}
		Cursor::RequestStyle(CursorStyle::Default);
		return;           // この時点でポーズ中ならリターン
	}
	menu = false;
	isPaused = false;

	player.SetOldEyePosition(player.eyePosition);

	ControlPlayer();



	ControlEnemy();




	NavigationRoot();


	Cursor::SetPos(Scene::Center());

	player.collider.setPos(camera.getEyePosition());

	for (const auto& box : boxes)
	{
		const bool intersects = player.collider.intersects(box);
		if (intersects)
		{
			player.eyePosition = player.GetOldPosition();

		}
	}
	SonarAround();



}


void Game::tutorialDraw()const
{
	auto pad = XInput(0);
	// 3D 描画
	{
		// renderTexture を背景色で塗りつぶし、
		// renderTexture を 3D 描画のレンダーターゲットに
		const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
		Plane{ 64 }.draw(uvChecker);

		for (auto& enemy : enemys)
		{
			enemy->GetCollider().draw();
		}
		for (auto& box : boxes)
		{
			box.draw();
		}
		for (auto& navi : navigatePass)
		{
			navi.GetNode()
				.scaled(0.25)
				.draw(Linear::Palette::Green);
		}
	}

	// 通常のシーン描画
	// 3D シーンを 2D シーンに描画
	{
		Graphics3D::Flush();
		renderTexture.resolve();
		Shader::LinearToScreen(renderTexture);
	}

	if (statusDesc)
	{
		//RectF{Scene::Size().x * 0.4,0,Scene::Size().x * 0.75,Scene::Size().y}.draw(Palette::Black);
	}
	if (statusDesc)
	{
		Rect{ Scene::Size() }.draw(Palette::Black);
		Rect{ Arg::topRight(Scene::Size().x,30),300,64 }.drawFrame(1.0, Palette::Grey);
		Rect{ Arg::topLeft(Scene::Size().x - 300,30),skipoverrideCount,64 }.draw(Palette::Grey);
		FontAsset(U"meiryo")(U"B長押しでスキップ").draw(Scene::Size().x - 286, 50);
		Terminal::GetInst().StreamTxt();
	}
	else
	{
		Draw2D();
		Draw2DLightBloomed();
	}
	if (isPaused)
	{
		Rect{ Scene::Size() }.draw(ColorF{ 0,0,0,0.8 });
		SimpleGUI::Slider(U"エイム感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 5.0, Vec2{ Scene::Center().x - 500,150 }, 500, 400);
		if (pad.isConnected())
		{
			TextureAsset(U"controllerDesc")
				.scaled(0.9, 0.9)
				.drawAt(Scene::Center() + Vec2{ 0, 150 });
		}
		else
		{

			TextureAsset(U"description")
				.scaled(0.2, 0.2)
				.drawAt(Scene::Center() + Vec2{ 0,200 });
		}
		

		if (SimpleGUI::ButtonAt(U"タイトルへ戻る", Vec2{ Scene::Center().x,100 }))
		{
			//changeScene(GameState::Title);
		}


		FontAsset(U"meiryob")
			(U"///////////////////////////////////////////////"
				U"PAUSE"
				U"///////////////////////////////////////////////")
				.drawAt(Vec2{ Scene::Center().x,300 }, Palette::Darkorange);

		if (pad.leftThumbX)
		{
			Padpos.x += pad.leftThumbX * Scene::DeltaTime() * 500;
		}
		if (pad.leftThumbY)
		{
			Padpos.y -= pad.leftThumbY * Scene::DeltaTime() * 500;
		}
		if (pad.buttonLB.down())
		{
			getData().sensi -= 0.1;
		}
		if (pad.buttonRB.down())
		{
			getData().sensi += 0.1;
		}
		if (pad.isConnected())
		{
			drawCur(Padpos);
			FontAsset(U"meiryob")(U"L").draw(Vec2{ Scene::Center().x - 500 + 450,170 },Palette::Darkorange);
			FontAsset(U"meiryob")(U"R").draw(Vec2{ Scene::Center().x - 500 + 500 + 400,170 },Palette::Darkorange);
		}

		return;//本来ここでReturnすべきでないがライトブルームを消すためにリターンする
	}


	{
		// ガウスぼかし用テクスチャにもう一度シーンを描く
		// ここで重ねて描画した2Dテクスチャはライトブルーム効果が付く
		{
			const ScopedRenderTarget2D target2{ gaussianA1.clear(Palette::Black) };
			const ScopedRenderStates2D blend2{ BlendState::Additive };
			if (statusDesc)
			{
				Terminal::GetInst().StreamTxt();
			}
			else
			{
				Draw2DLightBloomed();
			}
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


