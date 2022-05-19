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
	terrCSV.load(U"csv/Stage1._コイン.csv");
	navigateCSV.load(U"csv/testNavigate.csv");
	//terrCSV.load(U"csv/Stage1._コイン.csv");
	//terrCSV.load(U"Stage/Stage1._地形.csv");
	//enemyCSV.load(U"Stage/Stage1._敵.csv");
	//goal = InitializeGoal(Vec2(13200, 650));
	//Cursor::SetDefaultStyle(CursorStyle::Hidden);
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
	//Size terrainSize = { terrCSV.columns(1), terrCSV.rows() };//マップの大きさ｛ｘ、ｙ｝
	Size terrainSize = { terrGrid.width(),terrGrid.height() };//マップの大きさ｛ｘ、ｙ｝

	for (auto p : step(Size{ terrGrid.width(),terrGrid.height() })) {
		if (terrGrid[p.y][p.x] == 1)
			boxes << Box{p.x * 4.0,2,p.y * 4.0,4.0};

	}

	LoadNavigate(6);//内部に無限ループの可能性を含んでいる

	

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
	command[Logging::DetectEnemy] = str;
	command[Logging::DestroyEnemy] = lpsum;
	//GlobalAudio::BusSetHighPassFilter(MixBus1,0,1000,0.6,1.0);
}

void Game::update()
{
#if _DEBUG
	//InInitialize later
	if (KeyU.down())
	{
		enemys << std::make_shared<RangeEnemy>('t');
	}
	if (KeyI.down())
	{
		enemys << std::make_shared<MeleeEnemy>('t');
	}
	if (KeyO.down())
	{
		terminal.txtIndexManager.push_back(Logging::DetectEnemy);
	}
	if (KeyP.down())
	{
		terminal.txtIndexManager.push_back(Logging::DestroyEnemy);
	}
	if (KeyJ.down())
	{
		terminal.txtIndexManager.push_back(Logging::Damaged);
	}
	command[Logging::Damaged] = U"You look at{:.0f}degrees"_fmt(player.GetRadian());
#endif





	ClearPrint();

	//ポーズ画面への移行

	if (KeyEnter.down())
	{
		isPaused = !isPaused;       // 0コンのスタートボタンが押されたらポーズ状態が反転
		menu = !menu;
		/*auto m = SimpleGUI::Slider(U"マウス感度{:.2f}"_fmt(getData().sensi), getData().sensi, 0.0, 1.0, Vec2{ 770,400 }, 200, 200);*/
	}
	if (SimpleGUI::Button(U"決定", Vec2{ 1100, 600 }))
	{
		menu = !menu;
		isPaused = !isPaused;
	}

	if (isPaused) return;           // この時点でポーズ中ならリターン

	menu = false;
	isPaused = false;




	//s3d::Print << navigatePass.size();
	

	const double speed = (Scene::DeltaTime() * 2.0 * 5);
	double rate = 1.0;
	if (KeyA.pressed() || KeyD.pressed()) {
		if (KeyS.pressed() || KeyW.pressed()) {
			//斜めの場合は、移動量の倍率係数を０．７１に設定
			rate = 0.71;
		}
	}

	if (KeyW.pressed())
	{
		player.eyePosition += (GetDirection(player.GetRadian()) * speed * rate);
	}

	if (KeyS.pressed())
	{
		player.eyePosition += (-GetDirection(player.GetRadian()) * speed * rate);
	}

	if (KeyA.pressed())
	{
		player.eyePosition += (GetDirection(player.GetRadian() - 90_deg) * speed * rate);

	}

	if (KeyD.pressed())
	{
		player.eyePosition += (GetDirection(player.GetRadian() + 90_deg) * speed * rate);
	}


	ControlFirearm();

	//if (KeyLeft.pressed())
	player.SetRadian(player.GetRadian() + Cursor::DeltaRaw().x * 0.001);
	//angle += Cursor::DeltaRaw().x * 0.001;

	if (player.GetRadian() < 0_degF)
	{
		player.SetRadian(player.GetRadian() + 360_degF);
	}

	if (360_deg < player.GetRadian())
	{
		player.SetRadian(player.GetRadian() - 360_degF);
	}

	// 位置・注目点情報を更新
	//入力検知のあとで更新する必要がある
	camera.setView(player.eyePosition, GetFocusPosition(player.eyePosition, player.GetRadian()));
	s3d::Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(player.GetRadian()));
	s3d::Print << U"direction: {:.2f}"_fmt(GetDirection(player.GetRadian()));
	s3d::Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	s3d::Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
	//Print << U"meiryo" << FontAsset(U"meiryo").height();
	Graphics3D::SetCameraTransform(camera);

	//s3d::Print << U"degNorm" << ToDegrees(player.GetRadian()) / 180 - 1.0;


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
		if (enemy->GetDistanceToPlayer() < enemy->GetDistanceNearestBox())
		{
			enemy->SetAudioPanRad(CalcRadLeftAndRightDetail(front, playerLookAtEnemy));
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
			double dot = Math::Acos(GetDot(Normalize(front), Normalize(playerLookAtEnemy)));
			//dotに入っているのはラジアン
			
		
			//ここでは常に角度をラジアンで扱っている
			double angDiffRad = dot;
			//angDiffRad = asinf(DirectX::XMVectorGetY(xmResult)) > 0 ? angDiffRad :  2 * Math::PiF - angDiffRad;
			angDiffRad = isExistRightside(front, playerLookAtEnemy) ? angDiffRad : 2 * Math::PiF - angDiffRad;
			enemy->SetAngleDiffs(angDiffRad);
			//s3d::Print << U"angDiffRad" << angDiffRad;
			//s3d::Print << U"angDiffRadNorm" << ToDegrees(angDiffRad) / 180 - 1.0;
			s3d::Print << U"angPan" << CalcRadLeftAndRightDetail(front, playerLookAtEnemy);

			
			
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

	//ナビゲーションの処理
	NavigateNode nextDestination = navigatePass.at(player.GetPassedNode());
	if (player.collider.intersects(nextDestination.GetNode())
		and nextDestination.GetNode().center not_eq navigatePass.back().GetNode().center)
	{
		//プレイヤーのコライダーと接触したとき最後のノードでなければ次のノードを指す
		//andを外すとOut of range
		player.incrementPassedNode();
	}
	if (player.GetPassedNode() > 0)
	{
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
		if (betweenPlayerAndNode.length() > betweenNodes.length() + 2 * 2/*NodeRadius * 2*/)
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
			player.SetPassedNode(nearestNodeIndex);
		}
	}

	s3d::Print << U"passedNodeCount" << player.GetPassedNode();
	//次のナビゲーションノードとカメラの視線ベクトルの内積を使って角度を求め、
	//外積を使って角度を３６０度系に正規化
	//常にラジアンで計算している
	Vec3 naviVec = nextDestination.GetNode().center - camera.getEyePosition();
	double dot = Math::Acos(GetDot(camera.getLookAtVector(), Normalize(naviVec)));
	dot = isExistRightside(camera.getLookAtVector(), Normalize(naviVec)) ? dot : 2 * Math::PiF - dot;
	player.interfaces.arrowRadian = dot;


	s3d::Print << camera.screenToRay(Scene::Center());

	s3d::Print << Scene::Size();
	Cursor::SetPos(Scene::Center());

	//プレイヤーの当たり判定BOX
	player.collider.setPos(camera.getEyePosition());
	
	player.SetOldEyePosition(player.eyePosition);

	for (const auto& box : boxes)
	{
		const bool intersects = player.collider.intersects(box);
		if (intersects)
		{
			player.eyePosition = player.GetOldPosition();
			if (KeyW.pressed())
			{
				player.eyePosition -= (GetDirection(player.GetRadian()) * speed);

			}

			if (KeyS.pressed())
			{
				player.eyePosition -= (-GetDirection(player.GetRadian()) * speed);
			}

			if (KeyA.pressed())
			{
				player.eyePosition -= (GetDirection(player.GetRadian() - 90_deg) * speed);
			}

			if (KeyD.pressed())
			{
				player.eyePosition -= (GetDirection(player.GetRadian() + 90_deg) * speed);
			}
		}
	}
	SonarAround();
	//0.01秒毎に1ずつ回復
	//->1秒毎に100回復する
	if (player.GetRegenerateTimer().elapsed() > 5s
		and player.GetRegenerateTimer().ms() % 10 == 0)
	{
		player.SetHealth(player.GetHealth() + 1);
		
	}

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
		s3d::Print << U"sonar  " << sensor.distanceToBox.value_or(9999);
	}
	
}

void Game::draw()const
{
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
			navi.GetNode().draw();
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
	//s3d::Print << U"xmvecy" << asinf(DirectX::XMVectorGetY(xmResult));
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
		//無限ループを回して二次元配列の中の数字を一つずつ順番に読み込む
		for (auto p : step(Size(navigateGrid.width(), navigateGrid.height()))) {
			if (navigateGrid[p.y][p.x] == loadingCurrentNode)
				navigatePass << NavigateNode{ Vec3{ p.x * 4.0,2,p.y * 4.0} };

		}
		//最大数に達したらBreak　ここの最大数はCSV依存なのでプログラムで制御出来ない
		if (loadingCurrentNode == maxNode)
		{
			break;
		}
		loadingCurrentNode++;
	}
	
}

void Game::Draw2D() const
{
	//uvChecker.scaled(5,5).drawAt(Scene::Center(),Palette::Black);
	TextureAsset(U"navigateArrow")
		.rotated(player.interfaces.arrowRadian)
		.drawAt(Scene::Center().x, Scene::Height() - 150);
	TextureAsset(U"HPGaugeFrame").drawAt(Scene::Center().x,200);
	//現在値/最大値＝割合
	//(player.GetHealth()/100) * texture2d_desc.width
	double ratio = static_cast<double>(player.GetHealth()) / 1000;
	TextureAsset(U"HPGauge")(0,0, ratio * 640, 64).draw(Scene::Center().x - 320, 166);

	#if true //仮クリアランスソナー
	TextureAsset(U"testLidar")
		.scaled(0.2,0.2)
		.drawAt(Scene::Size().x - 256,256);
	for (const auto& [i, sensor] : Indexed(player.GetSensors()))
	{
		if (sensor.distanceToBox.has_value()
		and sensor.distanceToBox.value() < 30)
		{
			
			TextureAsset(U"indicaterLidar")
			.scaled(0.5,0.5)
			.rotated(i * 45_deg)
			.drawAt(Scene::Size().x - 256,256);
		}
	}
	#endif
}

void Game::Draw2DLightBloomed() const
{
	TextureAsset(U"crossHair").drawAt(Scene::Center());
	TextureAsset(U"HELgunIcon").drawAt(Scene::Size() * 0.9);
	FontAsset(U"meiryob")(U"00{} | ∞"_fmt(player.GetMagazine()))
		.drawAt(Scene::Size().x - 500,Scene::Size().y - 100);




	DrawTxt();
	for (const auto& enemy : enemys)
	{
		if (enemy->GetIsNotice())
		{
			
			if (enemy->GetIsFired())
			{
				TextureAsset(U"indicater2")
					.scaled(0.5, 0.5)
					.rotated(enemy->GetAngleDiffs())
					.drawAt(Scene::Center(),Palette::Red);
			}
			else
			{
				TextureAsset(U"indicater2")
					.scaled(0.5, 0.5)
					.rotated(enemy->GetAngleDiffs())
					.drawAt(Scene::Center());
			}
		}
	}
	//ラジアンから度の変換
	int anglenomal = ToDegrees(player.GetRadian());
	// 画像の (angle, 0) から幅 600, 高さ 50 の長方形部分
	compass(anglenomal * 5.3 + 200, 0, 400, 64).draw(800, 50);
	Print << anglenomal;
}


void Game::DrawTxt()const
{
	if (not terminal.txtIndexManager.empty())
	{
		terminal.streamTimer.start();
		const size_t length = terminal.streamTimer.elapsed() / 20ms;//20ms/a character
		FontAsset(U"meiryo")(command.at(terminal.txtIndexManager.front())
				 .substr(0, length))
			.draw(20, Scene::Height() - FontAsset(U"meiryo").height() * 2);
		if (length > command.at(terminal.txtIndexManager.front()).length())
		{
			logs.emplace_front(command.at(terminal.txtIndexManager.front()));
			terminal.txtIndexManager.pop_front();
			terminal.streamTimer.reset();
		}
	}

	for (const auto& [index, msg] : Indexed(logs))
	{
		int32 proportionalMerginY = -(FontAsset(U"meiryo").height()) * index - (FontAsset(U"meiryo").height() * 3);
		FontAsset(U"meiryo")(msg)
			.draw(20,
			Scene::Height() + proportionalMerginY);
	}
	if (logs.size() > Scene::Height() / FontAsset(U"meiryo").height())
	{
		logs.pop_back();
	}

	//for (const auto& [index,msg] : Indexed(logs))
	//{
	//	//int32 merginY = -(FontAsset(U"meiryo").height() * 3);
	//	int32 proportionalMerginY = -(FontAsset(U"meiryo").height()) * index - (FontAsset(U"meiryo").height() * 3);
	//	FontAsset(U"meiryo")(msg)
	//		.draw(20,
	//		Scene::Height() + proportionalMerginY);
	//}
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
		if (camera.screenToRay(Scene::Center()).intersects(enemy->GetCollider())
			and MouseL.down())
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
		}
	}
}
void Game::ControlFirearm()
{
	//Rを押したときプレイヤーの現在の弾倉が最大より少なく、
	//リロード中でなければリロードが開始される
	if (KeyR.down()
		and player.GetMagazine() < player.GetMaxMagazine()
		and not player.GetReloadTimer().isRunning())
	{
		player.reloadTimer().start();
	}
	//3秒経ったらリロードが完了する
	if (player.GetReloadTimer().elapsed() > player.GetReloadTime())
	{
		player.HaveReloadedGun();
		player.reloadTimer().reset();
	}
	//左クリックしたときプレイヤーが前回の射撃から、
	//ある程度時間が経過しており、
	//弾倉が１以上あるときに、
	//リロード中でいなければ撃てる
	if (MouseL.down()
		and not player.GetFireCoolTimer().isRunning()
		and player.GetMagazine() > 0
		and not player.reloadTimer().isRunning())
	{
		player.Fire();
		//当たったかどうかの確認をここで行う
		HitConfirmPlayerShot();
		player.fireCoolTimer().start();
		gunShot.playOneShot(MixBus1, 0.5,0);
	}

	if (player.GetFireCoolTimer().elapsed() > player.GetFireCoolTime())
	{
		player.fireCoolTimer().reset();
	}
	s3d::Print << camera.screenToRay(Scene::Center()).getDirection();
}

void Game::RangeEnemy::Move(Player& player)
{
	s3d::Print << U"rangeenemy";
	//s3d::Print << GetDistanceToPlayer();
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
				player.SetHealth(player.GetHealth() - GetAtk());
				player.regenerateTimer().restart();
				fireIndicater().start();
				SetIsFire(true);
			}
			
		}
	}
	if (GetAtkInterval().elapsed() > 500ms)
	{
		atkInterval().reset();
	}
	
	
}

void Game::MeleeEnemy::Move(Player& player)
{
	s3d::Print << U"mellee";
	//s3d::Print << GetDistanceNearestBox();
	//s3d::Print << GetDistanceToPlayer();
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
				player.SetHealth(player.GetHealth() - GetAtk());
				player.regenerateTimer().restart();
				fireIndicater().start();
				SetIsFire(true);
			}
			
		}
	}
	if (GetAtkInterval().elapsed() > 2000ms)
	{
		atkInterval().reset();
	}
	
}

void Game::RangeEnemy::PlayNoticeSound()
{
	AudioAsset(U"piano").playOneShot(MixBus0,1.0,GetAudioPanRad());
}
void Game::MeleeEnemy::PlayNoticeSound()
{
	AudioAsset(U"piano2").playOneShot(MixBus0, 1.0, GetAudioPanRad());
}



