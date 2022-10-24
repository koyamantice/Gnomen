#pragma once
#include"Actor.h"
#include"FBXObject3d.h"
#include"Texture.h"
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
	void Draw(DirectXCommon* dxCommon);	//描画処理

	void DemoDraw(DirectXCommon* dxCommon);	//描画処理

	void Finalize();	//終了処理

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};

	virtual void OnCollision(const std::string& Tag) {};

	void BoidAverage();
	void Move();
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
};

