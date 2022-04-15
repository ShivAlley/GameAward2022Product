#include "Game.h"

Game::Game(const InitData& init) :IScene(init)
{

}

void Game::update()
{
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
	Cursor::SetPos(Scene::Center());


	// 位置・注目点情報を更新
	camera.setView(eyePosition, GetFocusPosition(eyePosition, angle));
	Print << U"angle: {:.1f}°"_fmt(Math::ToDegrees(angle));
	Print << U"direction: {:.2f}"_fmt(GetDirection(angle));
	Print << U"eyePositon: {:.1f}"_fmt(camera.getEyePosition());
	Print << U"focusPosition: {:.1f}"_fmt(camera.getFocusPosition());
	Graphics3D::SetCameraTransform(camera);

	Vec3 vecter = camera.getEyePosition() - sampleEnemy.coll.center.xyz();
	Vec3 normedVec = Normalize(vecter);

	Vec3 moveLng = normedVec * deltaTime;

	sampleEnemy.coll.center += moveLng;
}

void Game::draw()const
{
	// 3D 描画
	{
		// renderTexture を背景色で塗りつぶし、
		// renderTexture を 3D 描画のレンダーターゲットに
		const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
		Plane{ 64 }.draw(uvChecker);
		auto v = Box{ -8,2,0,4 }.draw(ColorF{ 0.8, 0.6, 0.4 }.removeSRGBCurve());
		//Sphere{ 0,2,0,2 }.draw(ColorF{ 0.4, 0.8, 0.6 }.removeSRGBCurve());
		Cylinder{ 8, 2, 0, 2, 4 }.draw(ColorF{ 0.6, 0.4, 0.8 }.removeSRGBCurve());
		Draw(v, camera.screenToRay(Scene::Center()));
		billboard.draw(camera.billboard(GetFocusPosition(eyePosition, angle), 0.9), uvChecker, Palette::Black);
		sampleEnemy.coll.draw(Linear::Palette::Darkgreen);
		if (camera.screenToRay(Scene::Center()).intersects(sampleEnemy.coll) and MouseL.down())
		{
			sampleEnemy.coll.scaled(0, 0);
		}
	}

	// 通常のシーン描画
	// 3D シーンを 2D シーンに描画
	{
		Graphics3D::Flush();
		renderTexture.resolve();
		Shader::LinearToScreen(renderTexture);
		font(U"tes").drawAt(100, 300);
	}


	{
		// ガウスぼかし用テクスチャにもう一度シーンを描く
		{
			const ScopedRenderTarget2D target2{ gaussianA1.clear(Palette::Black) };
			const ScopedRenderStates2D blend2{ BlendState::Additive };
			//Shader::LinearToScreen(renderTexture);
			font(U"tes").drawAt(100, 300);
		}

		// オリジナルサイズのガウスぼかし (A1)
		// A1 を 1/4 サイズにしてガウスぼかし (A4)
		// A4 を 1/2 サイズにしてガウスぼかし (A8)
		Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		Shader::Downsample(gaussianA1, gaussianA4);
		Shader::GaussianBlur(gaussianA4, gaussianB4, gaussianA4);
		Shader::Downsample(gaussianA4, gaussianA8);
		Shader::GaussianBlur(gaussianA8, gaussianB8, gaussianA8);
	}

	if (lightBloom)
	{
		const ScopedRenderStates2D blend2{ BlendState::Additive };
		gaussianA1.resized(sceneSize).draw(ColorF{ 0.1 });
		gaussianA4.resized(sceneSize).draw(ColorF{ 0.4 });
		gaussianA8.resized(sceneSize).draw(ColorF{ 0.8 });
	}
}

