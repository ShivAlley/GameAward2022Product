#pragma once
#include "common.h"
#include <variant>



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

class Game :public App::Scene
{
public:
	Game(const InitData& init);
	void update()override;
	void draw() const override;
private:
	VideoTexture teseff{ U"effects/64ed59a0b29891eb.mp4" };
	Cylinder playercollider;
	class Enemy
	{
	public:
		//Enemy() = default;
		virtual ~Enemy() {};
#if _DEBUG
		Enemy(char) { collider = Sphere{ initpos,4.0 }; }
#endif // _DEBUG
		Sphere collider;
		Vec3 initpos = { 0,2,10 };
	private:
		//最終的にはStatic constで初期化する
		double velocity;
		int32 atk;
		int32 health;
		int32 atkRange;
	};
	class RangeEnemy : public Enemy
	{
	public:
		RangeEnemy() = default;
		RangeEnemy(char) : Enemy('t') {}
	private:

	};
	class MeleeEnemy : public Enemy
	{
	public:
		MeleeEnemy() = default;
		MeleeEnemy(char) : Enemy('t'){}
	private:

	};
	Array<std::shared_ptr<Enemy>> enemys;



	//Model manequin{ U"model/testmanequin_0001.obj" };
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	Font font{ 50,Typeface::Bold };

	Vec3 eyePosition{ 0, 2, -16 };
	double angle = 0_deg;
	BasicCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 0, 16, -32 }, GetFocusPosition(eyePosition, angle) };

	// ビルボード表示する板
	const Mesh billboard{ MeshData::Billboard() };
	
	Enemy sampleEnemy{ 't' };



	Size sceneSize{ Scene::Size() };
	Point sceneCenter{ sceneSize / 2 };
	const RenderTexture gaussianA1{ sceneSize }, gaussianB1{ sceneSize };
	const RenderTexture gaussianA4{ sceneSize / 4 }, gaussianB4{ sceneSize / 4 };
	const RenderTexture gaussianA8{ sceneSize / 8 }, gaussianB8{ sceneSize / 8 };

	

	Vec3 GetDirection(double angle)const
	{
		const Vec2 dir = Circular{ 1.0, angle };
		return{ dir.x, 0.0, -dir.y };
	}

	Vec3 GetFocusPosition(const Vec3& eyePosition, double angle)const
	{
		return (eyePosition + GetDirection(angle));

	}

	double GetDot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

};



