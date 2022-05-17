#include "Game.h"
#include "App\cri_sound\日本ゲーム大賞のSE_project_acf.h"
#include "App\cri_sound\WorkUnit\CueSheet_0.h"

Game::Game(const InitData& init) :IScene(init)
{
	//四角生成
	for (int z = 1; z < 2; z++)
	{
		for (int x = 1; x < 2; x++)
		{
			boxes << Box{ (x * 3), 2, (z * 3), 4 };
		}
	}
	//Cursor::SetDefaultStyle(CursorStyle::Hidden);
	//Cri3dposInitialize();

	if (cri_obj.acb_hn == CRI_NULL)
	{
		//Window::Resize(100, 100);
	}

	for (auto st : step(5))
	{
		navigatePass << NavigateNode{Vec3{10 * st,2,0}};
	}

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
	logs[Logging::DetectEnemy] = str;
	logs[Logging::destroyEnemy] = lpsum;
}

void Game::Cri3dposInitialize()
{
	/* リスナーの初期位置を設定(この時点ではまだ実際に反映されていない) */
	//cri_obj.listener_pos.x = 0.0f;		cri_obj.listener_pos.y = 0.0f;		cri_obj.listener_pos.z = 0.0f;
	//cri_obj.listener_front.x = 0.0f;	cri_obj.listener_front.y = 0.0f;	cri_obj.listener_front.z = 1.0f;
	//cri_obj.listener_top.x = 0.0f;		cri_obj.listener_top.y = 1.0f;		cri_obj.listener_top.z = 0.0f;

	/* D-Basの作成（最大ストリーム数はここで決まる） */
	cri_obj.dbas_id = criAtomDbas_Create(NULL, NULL, 0);
	/* ACFファイルの読み込みと登録 */
	int32 calcsize = criAtomEx_CalculateWorkSizeForRegisterAcfFile(NULL, "cri_sound/WorkUnit/ADX2_samples.acf");
	void* space = malloc(calcsize);
	bool b = criAtomEx_RegisterAcfFile(NULL, "cri_sound/WorkUnit/ADX2_samples.acf", &space, calcsize);

	/* DSP設定のアタッチ */
	criAtomEx_AttachDspBusSetting(CRI_日本ゲーム大賞のSE_PROJECT_ACF_DSPSETTING_DSPBUSSETTING_0, NULL, 0);

	CriAtomExStandardVoicePoolConfig vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&vpool_config);
	vpool_config.num_voices = 8;
	vpool_config.player_config.max_sampling_rate = 48000 * 2;
	vpool_config.player_config.streaming_flag = CRI_TRUE;
	cri_obj.voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&vpool_config, NULL, 0);

	cri_obj.acb_hn = criAtomExAcb_LoadAcbFile(NULL, "App/cri_sound/WorkUnit_日本ゲーム大賞のSE/CueSheet_0.acb", NULL, NULL, NULL, 0);

	/* プレーヤの作成 */
	cri_obj.player = criAtomExPlayer_Create(NULL, NULL, 0);

	/* 3Dリスナーハンドルと3D音源ハンドルを作成 */
	cri_obj.listener = criAtomEx3dListener_Create(NULL, NULL, 0);
	cri_obj.source = criAtomEx3dSource_Create(NULL, NULL, 0);

	/* プレーヤにリスナーと音源のハンドルを登録 */
	criAtomExPlayer_Set3dListenerHn(cri_obj.player, cri_obj.listener);
	criAtomExPlayer_Set3dSourceHn(cri_obj.player, cri_obj.source);

	/* リスナーの位置の設定 */
	criAtomEx3dListener_SetPosition(cri_obj.listener, &(cri_obj.listener_pos));

	/* リスナーの向きの設定 */
	criAtomEx3dListener_SetOrientation(cri_obj.listener, &(cri_obj.listener_front), &(cri_obj.listener_top));

	/* リスナーのパラメータを実際に反映 */
	criAtomEx3dListener_Update(cri_obj.listener);

	/* 音源の位置の設定＆反映 */
	criAtomEx3dSource_SetPosition(cri_obj.source, &(cri_obj.source_pos));

	/* 音源のパラメータを実際に反映 */
	criAtomEx3dSource_Update(cri_obj.source);
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
		loggerController.txtIndexManager.push_back(Logging::DetectEnemy);
	}
	if (KeyP.down())
	{
		loggerController.txtIndexManager.push_back(Logging::destroyEnemy);
	}
#endif

	ClearPrint();

	const double speed = (Scene::DeltaTime() * 2.0 * 5);

	if (KeyQ.pressed())
	{
		player.eyePosition.z += speed;
	}

	if (KeyW.pressed())
	{
		player.eyePosition += (GetDirection(angle) * speed);
	}

	if (KeyS.pressed())
	{
		player.eyePosition += (-GetDirection(angle) * speed);
	}

	if (KeyA.pressed())
	{
		player.eyePosition += (GetDirection(angle - 90_deg) * speed);
	}

	if (KeyD.pressed())
	{
		player.eyePosition += (GetDirection(angle + 90_deg) * speed);
	}

	//if (KeyLeft.pressed())
	angle += Cursor::DeltaRaw().x * 0.001;

	if (angle < 0_degF)
	{
		angle += 360_degF;
	}

	if (360_deg < angle)
	{
		angle -= 360_deg;
	}

	player.AddTimer();

	// 位置・注目点情報を更新
	camera.setView(player.eyePosition, GetFocusPosition(player.eyePosition, angle));
	s3d::Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(angle));
	s3d::Print << U"direction: {:.2f}"_fmt(GetDirection(angle));
	s3d::Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	s3d::Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
	Print << U"timer" << player.GetTimer();
	Graphics3D::SetCameraTransform(camera);
	player.SetAngle(Math::ToDegrees(angle));
	Print << U"playerAngle" << player.GetAngle();

	for (auto&& enemy : enemys)
	{
		//敵からのRayを生成
		Vec3 norm = Normalize(camera.getEyePosition() - enemy->collider.center);
		Ray ray{ enemy->collider.center,norm };
		float minDistance = Math::Inf;
		for (const auto& box : boxes)
		{
			if (const Optional<float> distance = ray.intersects(box))
			{
				if (*distance < minDistance)
				{
					minDistance = *distance;
				}
			}
		}

		Optional<float> playerRayLength = ray.intersects(player.collider);
		//敵とプレイヤーの距離と敵からの一番近い壁との距離を比較
		//プレイヤーの方が距離が短ければ気づかれる
		if (playerRayLength < minDistance)
		{
			enemy->foundPlayer();
		}

		if (enemy->GetIsNotice())
		{
			Vec3 vec = camera.getEyePosition() - enemy->collider.center.xyz();
			Vec3 moveLng = Normalize(vec) * Scene::DeltaTime();//*velocity
			enemy->collider.center += moveLng;
			if (camera.screenToRay(Scene::Center()).intersects(enemy->collider) and MouseL.down())
			{
				enemy->collider.setR(0);
			}
			Vec3 front = camera.getFocusPosition() - camera.getEyePosition();
			Vec3 ToEnemyVec = enemy->collider.center - camera.getEyePosition();
			//敵との角度の差
			double dot = Math::Acos(GetDot(Normalize(front), Normalize(ToEnemyVec)));
			//dotに入っているのはラジアンなので弧度法になおす
			//このDotはあとで計算に使うのでDegdotに入れる
			double toDegDot = Math::ToDegrees(dot);
			Print << U"compas" << Math::ToDegrees(dot);

			auto castXMVector = [=](Vec3 src)->DirectX::XMVECTOR {
				DirectX::XMFLOAT3 temp{
					static_cast<float>(src.x),
					static_cast<float>(src.y),
					static_cast<float>(src.z),
				};
				return DirectX::XMLoadFloat3(&temp);
			};

			DirectX::XMVECTOR xmFront = castXMVector(front);
			DirectX::XMVECTOR xmToEn = castXMVector(ToEnemyVec);

			xmFront = DirectX::XMVector3Normalize(xmFront);
			xmToEn = DirectX::XMVector3Normalize(xmToEn);
			DirectX::XMVECTOR xmResult = DirectX::XMVector3Cross(xmFront, xmToEn);




			//正の値であるとき敵の中心は右にある
			//ArcSinでマイナス９０度～９０度の範囲での角度が出るが
			//あくまで左右判定のみに使用し角度の取り出しには使わない
			//内積を３６０度系に正規化する
			//ここでは常に角度をラジアンで扱っている
			s3d::Print << U"xmvecy" << asinf(DirectX::XMVectorGetY(xmResult));
			double angDiffRad = dot;
			angDiffRad = asinf(DirectX::XMVectorGetY(xmResult)) > 0 ? angDiffRad :  2 * Math::PiF - angDiffRad;
			enemy->SetAngleDiffs(angDiffRad);
			//overrideした派生クラス内の関数に記述する
			enemy->Move();
		}
	}
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
			for (auto&& [i,node] : Indexed(navigatePass))
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



	s3d::Erase_if(enemys, [](std::shared_ptr<Enemy> enemy) {return enemy->collider.r == 0; });
	Cursor::SetPos(Scene::Center());
#if false //sampleCode
	Vec3 vecter = camera.getEyePosition() - sampleEnemy.collider.center.xyz();
	Vec3 normedVec = Normalize(vecter);
	Vec3 moveLng = normedVec * Scene::DeltaTime();

	sampleEnemy.collider.center += moveLng;
	Cursor::SetPos(sceneCenter);
	if (camera.screenToRay(Scene::Center()).intersects(sampleEnemy.collider) and MouseL.down())
	{
		sampleEnemy.collider.setR(0);
	}

#endif

	//プレイヤーの当たり判定BOX
	player.collider.setPos(camera.getEyePosition());

	if (player.GetTimer() > 0)
	{
		player.SetOldEyePosition(player.eyePosition);

		player.ResetTimer();
	}

	for (const auto& box : boxes)
	{
		const bool intersects = player.collider.intersects(box);
		box.draw(intersects ? Linear::Palette::Orange : Linear::Palette::White);
		if (intersects)
		{
			player.eyePosition = player.GetOldPosition();
			if (KeyW.pressed())
			{
				player.eyePosition -= (GetDirection(angle) * speed);
			}

			if (KeyS.pressed())
			{
				player.eyePosition -= (-GetDirection(angle) * speed);
			}

			if (KeyA.pressed())
			{
				player.eyePosition -= (GetDirection(angle - 90_deg) * speed);
			}

			if (KeyD.pressed())
			{
				player.eyePosition -= (GetDirection(angle + 90_deg) * speed);
			}
		}
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

		//billboard.draw(camera.billboard(GetFocusPosition(eyePosition, angle), 1.5), uvChecker, Palette::Black);

		for (auto& enemy : enemys)
		{
			enemy->collider.draw();
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


	{
		//通常の2D描画
		TextureAsset(U"crossHair").drawAt(Scene::Center());
		DrawTxt();
		for (const auto& enemy : enemys)
		{
			TextureAsset(U"indicater")
				//.scaled(0.2, 0.2)
				.rotated(enemy->GetAngleDiffs())
				.drawAt(Scene::Center());
				s3d::Print << enemy->GetAngleDiffs();
		}
		//ラジアンから度の変換
		int anglenomal = ToDegrees(angle);
		// 画像の (angle, 0) から幅 600, 高さ 50 の長方形部分
		compass(anglenomal*5.3+200,0,400,50).draw(800,50);
		Print << anglenomal;
	}
	{
		// ガウスぼかし用テクスチャにもう一度シーンを描く
		{
			const ScopedRenderTarget2D target2{ gaussianA1.clear(Palette::Black) };
			const ScopedRenderStates2D blend2{ BlendState::Additive };
			//Shader::LinearToScreen(renderTexture);
			//font(U"tes").drawAt(100, 300);
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
		gaussianA1.resized(sceneSize).draw(ColorF{ 0.1 });
		gaussianA4.resized(sceneSize).draw(ColorF{ 0.4 });
		gaussianA8.resized(sceneSize).draw(ColorF{ 0.8 });
	}
}

void Game::DrawTxt()const
{
	
	
	if (not loggerController.txtIndexManager.empty())
	{

		loggerController.streamTimer.start();
		const size_t length = loggerController.streamTimer.ms() / 20;//20ms/a character
		font(logs.at(loggerController.txtIndexManager.front())
				 .substr(0,length))
				 .draw(20,40);
		if (length > logs.at(loggerController.txtIndexManager.front()).length())
		{
			loggerController.txtIndexManager.pop_front();
			loggerController.streamTimer.reset();
		}
	}
	
	

}

