﻿#pragma once
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
	
	class Enemy
	{
	public:
		Enemy() = default;
		Enemy(char) { coll = Cylinder{ initpos,2,4 }; }
		Vec3 initpos = { 0,2,10 };
		Cylinder coll{};
		double speed = 0;
	};
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
	const RenderTexture gaussianA1{ sceneSize }, gaussianB1{ sceneSize };
	const RenderTexture gaussianA4{ sceneSize / 4 }, gaussianB4{ sceneSize / 4 };
	const RenderTexture gaussianA8{ sceneSize / 8 }, gaussianB8{ sceneSize / 8 };

	bool lightBloom = true;

	const double deltaTime = Scene::DeltaTime();
	const double speed = (deltaTime * 2.0);

	Vec3 GetDirection(double angle)const
	{
		const Vec2 dir = Circular{ 1.0, angle };
		return{ dir.x, 0.0, -dir.y };
	}

	Vec3 GetFocusPosition(const Vec3& eyePosition, double angle)const
	{
		return (eyePosition + GetDirection(angle));

	}

};



