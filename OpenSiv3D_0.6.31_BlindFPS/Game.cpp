#include "Game.h"
#include "App\cri_sound\日本ゲーム大賞のSE_project_acf.h"
#include "App\cri_sound\WorkUnit\CueSheet_0.h"

Game::Game(const InitData& init) :IScene(init)
{
	//Cursor::SetDefaultStyle(CursorStyle::Hidden);
	Cri3dposInitialize();
	
	if (cri_obj.acb_hn == CRI_NULL)
	{
		//Window::Resize(100, 100);
	}
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
	int32 calcsize = criAtomEx_CalculateWorkSizeForRegisterAcfFile(NULL, "App/cri_sound/WorkUnit/ADX2_samples.acf");
	void* space = malloc(calcsize);
	bool b = criAtomEx_RegisterAcfFile(NULL, "App/cri_sound/WorkUnit/ADX2_samples.acf", &space, calcsize);

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
	if (KeyU.down())
	{
		enemys << std::make_shared<RangeEnemy>('t');
	}
	if (KeyI.down())
	{
		enemys << std::make_shared<MeleeEnemy>('t');
	}
#endif

	ClearPrint();
	
	const double deltaTime = Scene::DeltaTime();
	const double speed = (deltaTime * 2.0 * 5);

	if (KeyQ.pressed())
	{
		eyePosition.z += speed;
	}

	if (KeyW.pressed())
	{
		eyePosition += (GetDirection(angle) * speed);
	}

	if (KeyS.pressed())
	{
		eyePosition += (-GetDirection(angle) * speed);
	}

	if (KeyA.pressed())
	{
		eyePosition += (GetDirection(angle - 90_deg) * speed);
	}


	if (KeyD.pressed())
	{
		eyePosition += (GetDirection(angle + 90_deg) * speed);
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
	
	timer++;
	
	// 位置・注目点情報を更新
	camera.setView(eyePosition, GetFocusPosition(eyePosition, angle));
	s3d:: Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(angle));
	s3d::Print << U"direction: {:.2f}"_fmt(GetDirection(angle));
	s3d::Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	s3d::Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
	Print << U"timer" << timer;
	Graphics3D::SetCameraTransform(camera);
	

	for (auto&& enemy : enemys)
	{
		Vec3 vec = camera.getEyePosition() - enemy->collider.center.xyz();
		Vec3 moveLng = Normalize(vec) * deltaTime;//*velocity
		enemy->collider.center += moveLng;
		if (camera.screenToRay(Scene::Center()).intersects(enemy->collider) and MouseL.down())
		{
			enemy->collider.setR(0);
		}
		Vec3 front = camera.getFocusPosition() - camera.getEyePosition();
		Vec3 ToEnemyVec = enemy->collider.center - camera.getEyePosition();
		double temp = Math::Acos(GetDot(Normalize(front),Normalize(ToEnemyVec)));
		if (ToDegrees(temp) > 180)
		{
			temp *= -1;
		}
		s3d::Print << U"compas" << Math::ToDegrees(temp);
		//敵との角度の差
		double dot = Math::Acos(GetDot(Normalize(front), Normalize(ToEnemyVec)));
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

		playerInterface.arrowRadian = DirectX::XMVectorGetY(xmResult);


		//正の値であるとき敵の中心は右にある
		s3d::Print << U"xmvecy" << DirectX::XMVectorGetY(xmResult);
		enemy->Move();
	}

	

	s3d::Erase_if(enemys, [](std::shared_ptr<Enemy> enemy) {return enemy->collider.r == 0; });



	

#if true sampleCode
	Vec3 vecter = camera.getEyePosition() - sampleEnemy.collider.center.xyz();
	Vec3 normedVec = Normalize(vecter);
	Vec3 moveLng = normedVec * deltaTime;

	sampleEnemy.collider.center += moveLng;
	Cursor::SetPos(sceneCenter);
	if (camera.screenToRay(Scene::Center()).intersects(sampleEnemy.collider) and MouseL.down())
	{
		sampleEnemy.collider.setR(0);
	}

	//四角生成
	Array<Box> boxes;
	for ( int z = 1;z<2;z++)
	{
		for (int x = 1; x < 2; x++)
		{
			boxes << Box{ (x * 3), 2, (z * 3), 4 };
		}
	}

		//プレイヤーの当たり判定BOX
			Box playerbox = { camera.getEyePosition().x,camera.getEyePosition().y,camera.getEyePosition().z,4 };
			if (timer >0)
			{
				oldeyeposition[0]= eyePosition;
				
				timer=0;
			}
			const Ray ray = camera.screenToRay(Cursor::PosF());
			const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };

			for (const auto& box : boxes)
			{
				const bool intersects = playerbox.intersects(box);
				box.draw(intersects ? Linear::Palette::Orange : Linear::Palette::White);
				if (intersects)
				{
					eyePosition = oldeyeposition[0];
					if (KeyW.pressed())
					{
						eyePosition -= (GetDirection(angle) * speed);
					}

					if (KeyS.pressed())
					{
						eyePosition -= (-GetDirection(angle) * speed);
					}

					if (KeyA.pressed())
					{
						eyePosition -= (GetDirection(angle - 90_deg) * speed);
					}


					if (KeyD.pressed())
					{
						eyePosition -= (GetDirection(angle + 90_deg) * speed);
					}
				}
			}

			{
				//const ScopedRenderStates3D rasterizer{ RasterizerState::WireframeCullNone };
			}

		
#endif
}

void Game::draw()const
{
	// 3D 描画
	{
		// renderTexture を背景色で塗りつぶし、
		// renderTexture を 3D 描画のレンダーターゲットに
		const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
		Plane{ 64 }.draw(uvChecker);
		Line3D ll{ camera.getEyePosition(),sampleEnemy.collider.center };
		Vec3 norm = Normalize(camera.getEyePosition() - sampleEnemy.collider.center);
		Ray r{ sampleEnemy.collider.center,  norm };
		Box box{ 2,2,2,2 };
		box.draw();
		
		//billboard.draw(camera.billboard(GetFocusPosition(eyePosition, angle), 1.5), uvChecker, Palette::Black);
		sampleEnemy.collider.draw(Linear::Palette::Darkgreen);
	
		for (auto& enemy : enemys)
		{
			enemy->collider.draw();
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
		for (auto&& enemy : enemys)
		{
			
			TextureAsset(U"testArrow")
				.scaled(0.2,0.2)
				.rotated(playerInterface.arrowRadian)
				.drawAt(Vec2{ 600,700 });
			
		}
		//ラジアンから度の変換
		int anglenomal = ToDegrees(angle);
		// 画像の (angle, 0) から幅 600, 高さ 50 の長方形部分
		compass(anglenomal*7+50,0,400,50).draw(600,50);
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


