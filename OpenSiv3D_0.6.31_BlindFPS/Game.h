#pragma once
#include "common.h"


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
#if _DEBUG
	VideoTexture teseff{ U"effects/64ed59a0b29891eb.mp4" };
	Cylinder playercollider;
	Mesh pyramid{ MeshData::Pyramid(3.0, 3.0) };
	Audio gunShot{GMInstrument::Gunshot,PianoKey::C4,1.0s};
#endif // _DEBUG

	
	void Draw2D()const;
	void Draw2DLightBloomed()const;
	void DrawTxt()const;
	void HitConfirmPlayerShot();
	void ControlFirearm();
	bool isExistRightside(Vec3 v1,Vec3 v2);
	float CalcRadLeftAndRightDetail(Vec3 v1,Vec3 v2);
	void SonarAround();
	enum class Logging
	{
		DetectEnemy = 0,
		DestroyEnemy,
		Damaged,
	};
	mutable struct TerminalController
	{
		Stopwatch streamTimer{ StartImmediately::No };
		std::deque<Logging> txtIndexManager;
	}terminal;
	HashTable<Logging,String> command;
	
	mutable std::deque<String> logs;
	//Array<String> consLogs;
	struct PlayerUI
	{
		double arrowRadian{ 0_deg };
		Array<double> damageIndicaterAngle{};

	};
	struct ClearanceSonar
	{
		Ray sonar;
		Optional<float> distanceToBox;
	};

	class Player
	{
	public:
		Player() { collider.setSize(4, 4, 4); }
		Box collider{};
		void SetOldEyePosition(const Vec3& pos) { oldEyePosition = pos; }
		const Vec3& GetOldPosition()const { return oldEyePosition; }
		PlayerUI interfaces;
		Vec3 eyePosition{ 0,2,0 };
		void SetRadian(double ang) { angle = ang; }
		const double& GetRadian()const { return angle; }
		void incrementPassedNode(){++passedNode;}
		void SetPassedNode(int32 num){passedNode = num;}
		const int32 GetPassedNode()const {return passedNode;}
		const int32 GetHealth()const{return health;}
		void SetHealth(int32 hp){health = hp;}
		const int32 GetMagazine()const {return magazine;}
		void HaveReloadedGun(){magazine = maxMagazine;}
		void Fire(){magazine--;}
		const int32 GetMaxMagazine()const{return maxMagazine;}
		const Duration& GetReloadTime()const{return reloadTime;}
		const Duration& GetFireCoolTime()const{return fireCoolTime;}

		const Stopwatch& GetRegenerateTimer()const{return m_regenerateTimer;}
		Stopwatch& regenerateTimer(){return m_regenerateTimer;}
		const Stopwatch& GetReloadTimer()const{return m_reloadTimer;}
		Stopwatch& reloadTimer(){return m_reloadTimer;}
		const Stopwatch& GetFireCoolTimer()const{return m_fireCoolTimer;}
		Stopwatch& fireCoolTimer(){return m_fireCoolTimer;}

		const std::array<ClearanceSonar,8>& GetSensors()const{return m_sensors;}
		std::array<ClearanceSonar,8>& sensors(){return m_sensors;}
	private:
		//プレイヤーの過去位置
		Vec3 oldEyePosition{};

		int32 magazine = 8;
		int32 health = 1000;
		Stopwatch m_regenerateTimer{ StartImmediately::Yes };
		Stopwatch m_reloadTimer{ StartImmediately::No };
		Stopwatch m_fireCoolTimer{ StartImmediately::No };
		double angle = 0_deg;
		int32 passedNode = 0;
		static constexpr int32 maxMagazine = 8;
		static constexpr Duration reloadTime = 3.0s;
		//RPM120相当
		//cooltime = RPM / 60 / 1.0s
		static constexpr Duration fireCoolTime = 0.5s;

		std::array<ClearanceSonar, 8> m_sensors;

	}player;


	class Enemy
	{
	public:
		//Enemy() = default;
		virtual ~Enemy() {};
#if _DEBUG
		Enemy(char)
			: m_collider(initpos, radius)
		{}
		Vec3 initpos = { 0,2,50 };
#endif // _DEBUG
		//Sphere collider;
		virtual void Move() = 0;
		virtual void PlayNoticeSound() = 0;
		virtual const double GetVelocity()const = 0;
		virtual const int32 GetAtkRange()const = 0;
		void foundPlayer() {
		//isNoticeがトグルするときだけ音を鳴らす
			if (isNotice == false)
			{
				PlayNoticeSound();
			}
			isNotice = true;
			noticeTimer().reset();
		}
		void LostSightOfPlayer(){
		atkCoolTimer().reset();
		isNotice = false;}
		const bool& GetIsNotice()const { return isNotice; }
		const double& GetAngleDiffs()const{return angleDiffs;}
		void SetAngleDiffs(double angle){angleDiffs = angle;}
		const double GetAudioPanRad()const{return audioPanRad;}
		void SetAudioPanRad(double angle) {audioPanRad = angle;}
		const Optional<float>& GetDistanceToPlayer()const {return distanceToPlayer;}
		void SetDistanceToPlayer(Optional<float> dist){distanceToPlayer = dist;}
		const Optional<float>& GetDistanceNearestBox()const {return distanceNearestBox;}
		void SetDistanceNearestBox(Optional<float> dist){distanceNearestBox = dist;}
		const Vec3& GetOldPosition()const{return oldPosition;}
		void SetOldPosition(const Vec3& pos){oldPosition = pos;}
		Sphere& collider(){return m_collider; }
		const Sphere& GetCollider()const {return m_collider;}
		Stopwatch& noticeTimer(){return m_noticeTimer;}
		const Stopwatch& GetNoticeTimer() const{return m_noticeTimer;}
		Stopwatch& atkCoolTimer(){return m_atkCoolTimer;}
		const Stopwatch& GetAtkCoolTimer()const{return m_atkCoolTimer;}
	private:
		
		static constexpr int32 atk = DEV_ONLY_MAGIC_NUM;
		static constexpr int32 health = DEV_ONLY_MAGIC_NUM;
		static constexpr int32 radius = 2;
		bool isNotice = false;
		double angleDiffs = 0_deg;
		double audioPanRad = 0_deg;
		Optional<float> distanceToPlayer = 0;
		Optional<float> distanceNearestBox = Math::Inf;
		Vec3 oldPosition{};
		Sphere m_collider;
		Stopwatch m_noticeTimer{StartImmediately::No};
		Stopwatch m_atkCoolTimer{StartImmediately::No};
	};
	class RangeEnemy : public Enemy
	{
	public:
		RangeEnemy() = default;
		RangeEnemy(char) : Enemy('t') {}
		void Move()override;
		void PlayNoticeSound()override;
		const double GetVelocity()const override { return velocity; }
		const int32 GetAtkRange()const override { return atkRange; }
	private:
		static constexpr double velocity = 3;
		static constexpr int32 atkRange = 8;

	};
	class MeleeEnemy : public Enemy
	{
	public:
		MeleeEnemy() = default;
		MeleeEnemy(char) : Enemy('t') {}
		void Move()override;
		void PlayNoticeSound()override;
		const double GetVelocity()const override { return velocity; }
		const int32 GetAtkRange()const override { return atkRange; }

	private:
		static constexpr double velocity = 5;
		static constexpr int32 atkRange = 3;
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
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	
	//Vec3 eyePosition{ 0, 2, -16 };
	//double angle = 0_deg;
	BasicCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 0, 16, -32 }, GetFocusPosition({0,2,-16}, 0_deg),Vec3{ 0, 1, 0 } ,0.2};

	// ビルボード表示する板
	const Mesh billboard{ MeshData::Billboard() };
	

	

	Array<Box> boxes;
	


	const RenderTexture gaussianA1{ Scene::Size() }, gaussianB1{ Scene::Size() };
	const RenderTexture gaussianA4{ Scene::Size() / 4 }, gaussianB4{ Scene::Size() / 4 };
	const RenderTexture gaussianA8{ Scene::Size() / 8 }, gaussianB8{ Scene::Size() / 8 };
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


