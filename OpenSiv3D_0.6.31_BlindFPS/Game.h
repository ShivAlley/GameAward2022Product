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
	bool toggBillboard = true;
#endif // _DEBUG
	void LoadNavigate(int32 maxNode);
	void ControlPlayer();
	void EnableDeadProtocol();
	void NavigationRoot();
	void ControlEnemy();
	void Draw2D()const;
	void Draw2DLightBloomed()const;
	void HitConfirmPlayerShot();
	void ControlFirearm();
	bool isExistRightside(Vec3 v1,Vec3 v2);
	float CalcRadLeftAndRightDetail(Vec3 v1,Vec3 v2);
	void SonarAround();
	bool menu = false;
	bool isPaused = false;
	bool isGoal = false;
	bool callonce = true;
	bool isRT = false;
	bool canPlay = false;
	bool endingExplain = true;
	int32 skipoverrideCount = 0;
	mutable Vec2 Padpos = Scene::Center();
	void drawCur(Vec2 pos) const
	{
		Circle(Arg::center(pos), 50).drawFrame(3.0, Palette::Royalblue);
		Shape2D::Plus(70, 4, pos).draw(Palette::Royalblue);
	}
	
	Stopwatch deadProtocol{ StartImmediately::No };
	enum class Logging
	{
		DetectEnemy = 0,
		DestroyEnemy,
		Damaged,
		UpdateNavigate,
		ReverseNavigate,
		CollideWall,
		Reloading,
		ReloadEnd,
		NoAmmo,
		PoorAim,
		DeadProtocolFirst,
		DeadProtocol,
		Tutorial,
		Tutorial2,
		TutorialDesc,
		TutorialDescCaution,
		startCaution,
		startCaution2,
		ending,
		ending2,
		ending3,
		ending4,
	};
	mutable Stopwatch walltimer{StartImmediately::Yes};
	class Terminal
	{
	private:
		Terminal() = default;
		~Terminal() = default;

	public:
		static Terminal& GetInst() {
			static Terminal terminal;
			return terminal;
		}
		Stopwatch streamTimer{ StartImmediately::No };
		std::deque<Logging> txtIndex;
		HashTable<Logging,String> command;
		String currentStreamingCommand;
		//mutable std::deque<String> logs;
		mutable std::deque<std::pair<Logging,String>> logs;
		void StreamTxt();
		bool isHighlightedOrange(char32 glyph) {
			return
			(
			(glyph == U'0')
			or (glyph == U'1')
			or (glyph == U'2')
			or (glyph == U'3')
			or (glyph == U'4')
			or (glyph == U'5')
			or (glyph == U'6')
			or (glyph == U'7')
			or (glyph == U'8')
			or (glyph == U'9')
			);
		}
	};
	
	//Array<String> consLogs;
	struct PlayerUI
	{
		double arrowRadian{ 0_deg };
		Array<double> damageIndicaterAngle{};

	};
	struct ClearanceSonar
	{
		Ray sonar;
		std::optional<float> distanceToBox;
	};

	class Player
	{
	public:
		Player() { collider.setSize(4, 4, 4); }
		Box collider{};
		void SetOldEyePosition(const Vec3& pos) { oldEyePosition = pos; }
		const Vec3& GetOldPosition()const { return oldEyePosition; }
		PlayerUI interfaces;
		//プレイヤーの初期位置　配置する壁が長さ４の正方形のとき6,6だとスポーンした時に埋まらない（Tiledで1,1を空白になっていることが条件
		Vec3 eyePosition{ 6,2,6 };
		void SetRadian(double ang) { angle = ang; }
		const double& GetRadian()const { return angle; }
		void SetNextNodeDistance(double dist) { nextNodeDistance = dist; }
		const double& GetNextNodeDistance()const { return nextNodeDistance; }
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
		int32 nextNodeDistance = 0;
		Stopwatch m_regenerateTimer{ StartImmediately::No };
		Stopwatch m_reloadTimer{ StartImmediately::No };
		Stopwatch m_fireCoolTimer{ StartImmediately::No };
		double angle = 0_deg;
		int32 passedNode = 0;
		static constexpr int32 maxMagazine = 8;
		static constexpr Duration reloadTime = 2.2s;
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
		Enemy(Vec3 pos) : m_collider(pos,radius)
		{}
		//Sphere collider;
		virtual void Move(Player& player) = 0;
		virtual void PlayNoticeSound() = 0;
		virtual const double GetVelocity()const = 0;
		virtual const int32 GetAtkRange()const = 0;
		virtual const int32 GetAtk()const = 0;
		static inline bool canplay = false;
		void foundPlayer() {
		//isNoticeがトグルするときだけ音を鳴らす
			if (isNotice == false)
			{
				PlayNoticeSound();
				Terminal::GetInst().command[Logging::DetectEnemy] = U"方位{:.0f} | 距離{:.0f}に敵を感知"_fmt(ToDegrees(GetAngleDiffs()),GetDistanceToPlayer().value());
				Terminal::GetInst().txtIndex.push_back(Logging::DetectEnemy);
				AudioAsset(U"v感知").playOneShot();
			}
			isNotice = true;
			noticeTimer().reset();
		}
		void LostSightOfPlayer(){
		foundAtkCoolTimer().reset();
		isNotice = false;}
		const bool GetIsNotice()const { return isNotice; }
		const bool GetIsFired()const { return isFired; }
		void SetIsFire(bool b) { isFired = b; }
		const double GetAngleDiffs()const{return angleDiffs;}
		void SetAngleDiffs(double angle){angleDiffs = angle;}
		const double GetAudioPanRad()const{return audioPanRad;}
		void SetAudioPanRad(double angle) {audioPanRad = angle;}
		const double GetAudioFalloff()const{return audioFalloff;}
		void SetAudioFalloff(double gain) { audioFalloff = gain;}
		const std::optional<float>& GetDistanceToPlayer()const {return distanceToPlayer;}
		void SetDistanceToPlayer(const std::optional<float>& dist){distanceToPlayer = dist;}
		const std::optional<float>& GetDistanceNearestBox()const {return distanceNearestBox;}
		void SetDistanceNearestBox(const std::optional<float>& dist){distanceNearestBox = dist;}
		const Vec3& GetOldPosition()const{return oldPosition;}
		void SetOldPosition(const Vec3& pos){oldPosition = pos;}
		Sphere& collider(){return m_collider; }
		const Sphere& GetCollider()const {return m_collider;}
		Stopwatch& noticeTimer(){return m_noticeTimer;}
		const Stopwatch& GetNoticeTimer() const{return m_noticeTimer;}
		Stopwatch& foundAtkCoolTimer(){return m_foundAtkCoolTimer;}
		const Stopwatch& GetFoundAtkCoolTimer()const{return m_foundAtkCoolTimer;}
		Stopwatch& atkInterval(){return m_atkInterval;}
		const Stopwatch& GetAtkInterval()const{return m_atkInterval;}
		Stopwatch& fireIndicater(){return m_fireIndicater;}
		const Stopwatch& GetFireIndicater()const{return m_fireIndicater;}
		Stopwatch& footStepTimer(){return m_footStepTimer;}
		const Stopwatch& GetFootStepTimer()const{return m_footStepTimer;}
	private:
		
		//static constexpr int32 health = DEV_ONLY_MAGIC_NUM;
		static constexpr int32 radius = 2;
		bool isNotice = false;
		bool isFired = false;
		double angleDiffs = 0_deg;
		double audioPanRad = 0_deg;
		double audioFalloff = 0_deg;
		std::optional<float> distanceToPlayer = 0;
		std::optional<float> distanceNearestBox = Math::Inf;
		Vec3 oldPosition{};
		Sphere m_collider;
		Stopwatch m_noticeTimer{StartImmediately::No};
		Stopwatch m_foundAtkCoolTimer{StartImmediately::No};
		Stopwatch m_atkInterval{StartImmediately::No};
		Stopwatch m_fireIndicater{StartImmediately::No};
		Stopwatch m_footStepTimer{StartImmediately::Yes};
	};
	class RangeEnemy : public Enemy
	{
	public:
		RangeEnemy() = default;
		//RangeEnemy(char) : Enemy('t') {}
		RangeEnemy(Vec3 pos) : Enemy(pos) {}
		void Move(Player& player)override;
		void PlayNoticeSound()override;
		const double GetVelocity()const override { return velocity; }
		const int32 GetAtkRange()const override { return atkRange; }
		const int32 GetAtk()const override { return atk; }
		
	private:
		static constexpr double velocity = 3;
		static constexpr int32 atkRange = 18;
		static constexpr int32 atk = 80;

	};
	class MeleeEnemy : public Enemy
	{
	public:
		MeleeEnemy() = default;
		//MeleeEnemy(char) : Enemy('t') {}
		MeleeEnemy(Vec3 pos) : Enemy(pos) {}

		void Move(Player& player)override;
		void PlayNoticeSound()override;
		const double GetVelocity()const override { return velocity; }
		const int32 GetAtkRange()const override { return atkRange; }
		const int32 GetAtk()const override { return atk; }

	private:
		static constexpr double velocity = 5;
		static constexpr int32 atkRange = 9;
		static constexpr int32 atk = 300;
	};
	class TutoEnemy : public Enemy
	{
	public:
		TutoEnemy() = default;
		//TutoEnemy(char) : Enemy('t') {}
		TutoEnemy(Vec3 pos) : Enemy(pos) {}
		void Move(Player& player)override;
		void PlayNoticeSound()override{AudioAsset(U"notice").playOneShot(); }
		const double GetVelocity()const override { return velocity; }
		const int32 GetAtkRange()const override { return atkRange; }
		const int32 GetAtk()const override { return atk; }
	private:
		static constexpr double velocity = 3;
		static constexpr int32 atkRange = 18;
		static constexpr int32 atk = 80;

	};

	Array<std::shared_ptr<Enemy>> enemys;

	class NavigateNode
	{
		public:
		NavigateNode(Vec3 pos):m_node(pos,radius){}
		const int32 GetRadius()const{return radius;}
		const double GetNextNodeDistance()const {return nextNodeDistance;}
		double SetNextNodeDistance(double dist){nextNodeDistance = dist;}
		Sphere& node(){return m_node;}
		const Sphere& GetNode()const{return m_node;}
		private:
		Sphere m_node;
		double nextNodeDistance;
		static const int32 radius = 2;
	};

	Array<NavigateNode> navigatePass;


	//Model manequin{ U"model/testmanequin_0001.obj" };
	const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();
	const Texture uvChecker{ U"example/texture/uv.png", TextureDesc::MippedSRGB };
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	
	//Vec3 eyePosition{ 0, 2, -16 };
	//double angle = 0_deg;
	double fov = 90_deg;
	BasicCamera3D camera{ renderTexture.size(), fov, Vec3{ 0, 16, -32 }, GetFocusPosition({0,2,-16}, 0_deg),Vec3{ 0, 1, 0 } ,0.2};

	

	CSV terrCSV;
	CSV enemyCSV;
	CSV navigateCSV;
	Grid<int8> terrGrid;
	Grid<int8> enemyGrid;
	Grid<int8> navigateGrid;

	Array<Box> boxes;
	


	const RenderTexture gaussianA1{ Scene::Size() }, gaussianB1{ Scene::Size() };
	const RenderTexture gaussianA4{ Scene::Size() / 4 }, gaussianB4{ Scene::Size() / 4 };
	const RenderTexture gaussianA8{ Scene::Size() / 8 }, gaussianB8{ Scene::Size() / 8 };
	const Texture compass{ U"Image/UI_.png" };

protected:
		void tutorial();
		void tutorialDraw()const;
		void tutorialErase() { enemys.clear(); }
		Stopwatch tutorialTimer{ StartImmediately::No };
		bool statusDesc = true;
		int32 descState = 0;
		Stopwatch statusTimer{StartImmediately::Yes};
		int32 explainState = 0;
		Stopwatch skipTimer{StartImmediately::No};
		double skipCount = 0;
		
private:

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

	int GetTile(const CSV& gMap, const int x, const int y)
	{
		return Parse<int>(gMap[y][x]);
	}

};


