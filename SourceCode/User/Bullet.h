#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Texture.h"
#include"ImageManager.h"

using namespace DirectX;
class Bullet {
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct FlockSystem {
		XMFLOAT3 average{};
		//分離
		float isolateRadius = 2.0f;
	
		float ctrDirX;
		float ctrDirY;
		float contX;
		float contY;
		XMFLOAT2 vel;
		float weight;
		float aveAngle;
		float disvel=(float)(rand()%5);
		float dx = disvel* cosf(aveAngle * (XM_PI / 180));
		float dy = disvel* sinf(aveAngle * (XM_PI / 180));

	};
public:
	enum command {
		Wait = 0,
		Attack,
		Slow,
		Dead,
	};
public:
	Bullet();
	virtual ~Bullet() = default;

	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { fbxObj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return fbxObj->GetPosition(); }
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { fbxObj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return fbxObj->GetRotation(); }

	//ID取得
	const int& GetID()const { return ID; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }

	//カメラ角度のセッタ＆ゲッタ
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//
	void SetContX(const float& contX) { this->flocking.contX = contX; }
	const float& GetContX() { return flocking.contX; }
	//
	void SetContY(const float& contY) { this->flocking.contY = contY; }
	const float& GetContY() { return flocking.contY; }

	void SetVel(const XMFLOAT2& vel) { this->flocking.vel= vel; }
	const XMFLOAT2& GetVel() { return this->flocking.vel; }



	//
	void SetCommand(const int& command, XMFLOAT3 pos = { 0,0,0 });
	const int& GetCommand() { return command; }
	//
	void SetDeadFlag(const bool& DeadFlag) { this->DeadFlag = DeadFlag; }
	//基本処理
	void Initialize(FBXModel* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	void Demo();		//更新処理
	void IntroUpdate(const int& Timer);
	void ResultUpdate(const int& Timer);
	void Draw(DirectXCommon* dxCommon);	//描画処理

	void DemoDraw(DirectXCommon* dxCommon);	//描画処理

	void Finalize();	//終了処理

	virtual void OnCollision(const std::string& Tag) {};

protected:
	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const int& Timer) {};
	virtual void ResultOnUpdate(const int& Timer) {};

	void BoidAverage();
	void Move();

	void WaitUpda();
	void SlowUpda();
	void AttackUpda();
	void Follow2Enemy();
	void Follow2Player();
	void WaitBullet();

	void KnockBack();
	bool knockBacking = false;

	void DamageInit();
	XMFLOAT3 rebound{};
	XMFLOAT3 distance{};
	XMFLOAT3 exploPos{};
	float damageframe = 0.0f;
	float fall = 0.4f;
	bool isLeft = false;
	bool throwReady = false;
	float frame = 0.0f;


	void DeadEnd();
	float deadframe = 0;
	float vanishHight = 0.1f;
	float vanishAlpha = 1.0f;
	int CoolTime = 0;

	float vel = 0.8f;
	bool burning = false;
	void BurnOut();
	float scale = 0.0f;
	float effectRate = 0.0f;
	bool follow = false;
	XMFLOAT2 vel_follow{};
protected:
	//バレット専用
	int ID;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
	//
	int command = 0;
	std::unique_ptr<FBXObject3d> fbxObj;
	std::unique_ptr<Texture> Shadow = nullptr;
	std::unique_ptr<Texture> Status = nullptr;
	std::unique_ptr<Texture> Explo = nullptr;
	std::unique_ptr<Texture> CharaDead = nullptr;
	enum DeathColor{
		Red=ImageManager::RedDead,
		Green,
	};
	int Color = 0;
	float angle = 0;
	bool DeadFlag = false;


	XMFLOAT3 oldPos = {};
	XMFLOAT3 AftaerPos{};

	Actor* enemy = nullptr;
	Actor* player = nullptr;

	XMFLOAT3 landing{};

	FlockSystem flocking;
	float dx=0;
	float dy=0;

	float hight = 100;
};

