#include "Game.h"


Game::Game(const InitData& init) :IScene(init)
{
	//Cursor::SetDefaultStyle(CursorStyle::Hidden);

}

void Game::update()
{
#if _DEBUG
	if (KeyU.down())
	{
		enemys << std::make_shared<RangeEnemy>('t');
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
	{
		angle += Cursor::DeltaRaw().x * 0.001;

		if (angle < 0_deg)
		{
			angle += 360_deg;
		}
	}

	//if (KeyRight.pressed())
	{
		//angle += Cursor::DeltaRaw().x * 0.1;

		if (360_deg < angle)
		{
			angle -= 360_deg;
		}
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

		double rad = Math::Atan2(ToEnemyVec.z, ToEnemyVec.x);
		rad -= 180_deg;
		Print << Math::ToDegrees(rad);
		DirectX::XMFLOAT3 storeVec3Front = DirectX::XMFLOAT3{ (float)front.x,(float)front.y,(float)front.z };
		DirectX::XMFLOAT3 storeVec3Toenemy = DirectX::XMFLOAT3{ (float)ToEnemyVec.x,(float)ToEnemyVec.y,(float)ToEnemyVec.z };

		DirectX::XMVECTOR xmFront = DirectX::XMLoadFloat3(&storeVec3Front);
		DirectX::XMVECTOR xmToEnemy = DirectX::XMLoadFloat3(&storeVec3Toenemy);

		DirectX::XMVECTOR xmResult = DirectX::XMVector3Cross(xmFront, xmToEnemy);


		if (Math::Asin(DirectX::XMVectorGetX(xmResult)) > 0)
		{
			s3d::Print << U"right";
		}

		s3d::Print << U"xmvecx" << DirectX::XMVectorGetX(xmResult);
	}


	s3d::Print << camera.getEyePosition().y;
	s3d::Print << sampleEnemy.collider.center.y;

	s3d::Erase_if(enemys, [](std::shared_ptr<Enemy> enemy){return enemy->collider.r == 0;});

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
			if (timer >1)
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
				const ScopedRenderStates3D rasterizer{ RasterizerState::WireframeCullNone };
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
		//playerbox = Box{ camera.getEyePosition().x,camera.getEyePosition().y,camera.getEyePosition().z,4 }.draw(ColorF{ 0.8, 0.6, 0.4 }.removeSRGBCurve());
		//auto v = Box{ -8,2,0,4 }.draw(ColorF{ 0.8, 0.6, 0.4 }.removeSRGBCurve());
		//Sphere{ 0,2,0,2 }.draw(ColorF{ 0.4, 0.8, 0.6 }.removeSRGBCurve());
		//Cylinder{ 8, 2, 0, 2, 4 }.draw(ColorF{ 0.6, 0.4, 0.8 }.removeSRGBCurve());
		//Draw(v, camera.screenToRay(Scene::Center()));
		//manequin.draw();
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
		//teseff.drawAt(Scene::Center());
		//font(U"tes").drawAt(100, 300);
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


