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
	const double speed = (deltaTime * 2.0 * 10);

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


	if (angle > 360_degF)
	{
		angle -= 360_degF;
	}
	
	
	
	// 位置・注目点情報を更新
	camera.setView(eyePosition, GetFocusPosition(eyePosition, angle));
	Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(angle));
	Print << U"direction: {:.2f}"_fmt(GetDirection(angle));
	Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
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
		//敵との角度の差
		double dot = Math::Acos(GetDot(Normalize(front),Normalize(ToEnemyVec)));
		Print << U"compas" << Math::ToDegrees(dot);

				
		auto castXMVector = [&](Vec3 src)->DirectX::XMVECTOR {
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
		Print << U"xmvecy" << DirectX::XMVectorGetY(xmResult);
	}


	Print << camera.getEyePosition().y;
	Print << sampleEnemy.collider.center.y;

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

