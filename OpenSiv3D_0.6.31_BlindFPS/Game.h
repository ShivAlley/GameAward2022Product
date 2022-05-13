﻿#pragma once
#include "common.h"
#include "3Dpos.h"


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
	Mesh pyramid{ MeshData::Pyramid(3.0, 3.0) };
	AppObj cri_obj{ 0 };
	void Cri3dposInitialize();
	void DrawTxt()const;
	const Font font{ 30,Typeface::Regular };
	const Font font1{ 30,Typeface::Bold };
	const Font font2{ 30,Typeface::Heavy };
	const Font font3{ 30,Typeface::Regular,FontStyle::Italic };
	const Font font4{ 30,Typeface::Regular,FontStyle::Bitmap };
	enum class Logging
	{
		DetectEnemy = 0,
		destroyEnemy,
	};
	mutable struct LoggingController
	{
		Stopwatch streamTimer{ StartImmediately::No };
		std::deque<Logging> txtIndexManager;
	}loggerController;
	HashTable<Logging,String> logs;
	
	mutable Array<String> loggs;
	//Array<String> consLogs;
	struct PlayerUI
	{
		double arrowRadian{ 0_deg };
		Array<double> damageIndicaterAngle{};

	};

	class Player
	{
	public:
		Player() { collider.setSize(4, 4, 4); }
		Box collider{};
		void AddTimer() { ++timer; }
		void ResetTimer() { timer = 0; }
		const int32& GetTimer()const { return timer; }
		void SetOldEyePosition(const Vec3& pos) { oldEyePosition = pos; }
		const Vec3& GetOldPosition()const { return oldEyePosition; }
		PlayerUI interfaces;
		Vec3 eyePosition{ 0,2,-16 };
		void SetAngle(double ang) { angle = ang; }
		const double& GetAngle()const { return angle; }
		void incrementPassedNode(){++passedNode;}
		void decrementPassedNode(){--passedNode;}
		void SetPassedNode(int32 num){passedNode = num;}
		const int32 GetPassedNode()const {return passedNode;}
	private:
		//プレイヤーの過去位置
		Vec3 oldEyePosition{};
		//タイマー
		int32 timer = 0;
		int32 health = 100;
		Stopwatch RegenerateTimer{ StartImmediately::Yes };
		double angle = 0_deg;
		int32 passedNode = 0;
	}player;


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
		virtual void Move() = 0;
		void foundPlayer() { isNotice = true; }
		const bool& GetIsNotice()const { return isNotice; }
		const double& GetAngleDiffs()const{return angleDiffs;}
		void SetAngleDiffs(double angle){angleDiffs = angle;}
	private:
		static const double velocity;
		static const int32 atk;
		static const int32 health;
		static const int32 atkRange;
		bool isNotice = false;
		double angleDiffs = 0_deg;
	};
	class RangeEnemy : public Enemy
	{
	public:
		RangeEnemy() = default;
		RangeEnemy(char) : Enemy('t') {}
		void Move()override final {
			s3d::Print << U"rangeenemy";
		}
	private:

	};
	class MeleeEnemy : public Enemy
	{
	public:
		MeleeEnemy() = default;
		MeleeEnemy(char) : Enemy('t') {}
		void Move()override final {
			s3d::Print << U"mellee";
		}
	private:

	};

	Array<std::shared_ptr<Enemy>> enemys;

	class NavigateNode
	{
		public:
		NavigateNode(Vec3 pos):m_node(pos,radius){}
		const int32 GetRadius()const{return radius;}
		Sphere& node(){return m_node;}
		const Sphere& GetNode()const{return m_node;}
		private:
		Sphere m_node;
		static const int32 radius = 2;
	};

	Array<NavigateNode> navigatePass;


	//Model manequin{ U"model/testmanequin_0001.obj" };
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const Texture compass{ U"App/Image/SampleCompass.png", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	
	//Vec3 eyePosition{ 0, 2, -16 };
	double angle = 0_deg;
	BasicCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 0, 16, -32 }, GetFocusPosition({0,2,-16}, 0_deg) };

	// ビルボード表示する板
	const Mesh billboard{ MeshData::Billboard() };
	

	

	Array<Box> boxes;
	


	Size sceneSize{ Scene::Size() };
	const RenderTexture gaussianA1{ sceneSize }, gaussianB1{ sceneSize };
	const RenderTexture gaussianA4{ sceneSize / 4 }, gaussianB4{ sceneSize / 4 };
	const RenderTexture gaussianA8{ sceneSize / 8 }, gaussianB8{ sceneSize / 8 };
	const Texture compass{ U"Image/UI_.png" };
	

	Vec3 GetDirection(double cameraAngle)const
	{
		const Vec2 dir = Circular{ 1.0, cameraAngle };
		return{ dir.x, 0.0, -dir.y };
	}

	Vec3 GetFocusPosition(const Vec3& eyePosition, double cameraAngle)const
	{
		return (eyePosition + GetDirection(cameraAngle));

	}

	double GetDot(const Vec3& v1, const Vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

};


