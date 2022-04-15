# include <Siv3D.hpp>
#include "Main.h"

Vec3 GetDirection(double angle)
{
	const Vec2 dir = Circular{ 1.0, angle };
	return{ dir.x, 0.0, -dir.y };
}

Vec3 GetFocusPosition(const Vec3& eyePosition, double angle)
{
	return (eyePosition + GetDirection(angle));
}

template <class Shape>
void Draw(const Shape& shape, const Ray& ray)
{
	if (auto pos = ray.intersectsAt(shape))
	{
		shape.draw(Linear::Palette::Orange);
		Sphere{ *pos, 0.15 }.draw(Linear::Palette::Red);
	}
	else
	{
		shape.draw(HSV{ (shape.center.x * 60), 0.1, 0.95 }.removeSRGBCurve());
	}
}

void DrawScene()
{

}


class Enemy
{
public:
	Enemy() = default;
	Enemy(char) { coll = Cylinder{ initpos,2,4 }; }
	Vec3 initpos = { 0,2,10 };
	Cylinder coll{};
	double speed = 0;
};



void Main()
{
	Window::Resize(1280, 720);
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	Font font{ 50,Typeface::Bold };

	Vec3 eyePosition{ 0, 2, -16 };
	double angle = 0_deg;
	BasicCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 0, 16, -32 }, GetFocusPosition(eyePosition, angle) };

	// ビルボード表示する板
	const Mesh billboard{ MeshData::Billboard() };

	Enemy sampleEnemy('t');



	Size sceneSize{ Scene::Size() };
	const RenderTexture gaussianA1{ sceneSize }, gaussianB1{ sceneSize };
	const RenderTexture gaussianA4{ sceneSize / 4 }, gaussianB4{ sceneSize / 4 };
	const RenderTexture gaussianA8{ sceneSize / 8 }, gaussianB8{ sceneSize / 8 };

	bool lightBloom = true;



	while (System::Update())
	{
		ClearPrint();
		const double deltaTime = Scene::DeltaTime();
		const double speed = (deltaTime * 2.0);

		

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
			billboard.draw(camera.billboard(GetFocusPosition(eyePosition, angle), 0.9), uvChecker,Palette::Black);
			sampleEnemy.coll.draw(Linear::Palette::Darkgreen);
			if (camera.screenToRay(Scene::Center()).intersects(sampleEnemy.coll) and MouseL.down())
			{
				sampleEnemy.coll.setSize(0, 0);
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
}
