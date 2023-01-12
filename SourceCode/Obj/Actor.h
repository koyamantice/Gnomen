#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include <DirectXMath.h>
#include"ParticleManager.h"
#include "DirectXCommon.h"
#include "FBXObject3d.h"

class ActorComponent;

class Actor {
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	//付与されているタグへのポインタ
	std::string tag = "None";
public:
	//各フェイズ
	enum Phase {
		APPROCH = 0,
		UNGUARD,
		LEAVE,
		WAIT,
		DEAD,
		ATTACK,
	};
	struct HitBound {
		bool isHit;
		XMFLOAT3 HitingPos;

	};
protected:
	//オブジェクト識別番号
	static int id;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
	//objは必要化
	bool isVisible = true;
public:
	virtual ~Actor() = default;
	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//昔の座標の取得
	const DirectX::XMFLOAT3& GetOldPosition() { return oldPos; }
	//プレイヤーの残像
	DirectX::XMFLOAT3 GetAFTIMAGE(const int& num) { return XMFLOAT3{ PlayerX[num + 1],RotY[num + 1],PlayerZ[num + 1] }; }
	//座標の取得
	void SetCameraPos(const DirectX::XMFLOAT3& pos) { this->cameraPos = cameraPos; }
	const DirectX::XMFLOAT3& GetCameraPos() { return cameraPos; }
	virtual const DirectX::XMFLOAT3& GetCameraPos(const float& angle, const float& str = 10) { return cameraPos; };
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }
	//コマンド取得
	void SetCommand(const int& command) { this->command = command; };
	const int& GetCommand() { return command; }
	//isAliveセッタ＆ゲッタ
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }
	//Player固有の処理
	virtual const XMFLOAT3& GetLockPos() { return obj->GetPosition(); };
	void SetHitBound(const XMFLOAT3& pos) { hitBound.isHit = true; hitBound.HitingPos = pos; };
	const bool& GetHitBound() { return  hitBound.isHit; }
	//OBJサイズ取得（X軸のやつを取っているので拡大はまとめてするの推奨）
	const float& GetSize()const { return obj->GetSize(); }

	//タグのセット
	void SetTag(const std::string& Tag) { tag = Tag; };
	//タグが一致するか
	const std::string& GetTag() { return tag; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(const bool& Active) { isActive = Active; };
	const bool& GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(const bool& Remove) { isRemove = Remove; };
	const bool& GetIsRemove() { return isRemove; }

	//canMoveセッタ＆ゲッタ
	void SetCanMove(const bool& canMove) { this->canMove = canMove; };
	const bool& GetCanMove() { return canMove; }

	//カメラ角度のセッタ＆ゲッタ
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//ワイルドイント引数
	void SetStock(const int& Stock) { this->stock = Stock; };
	const int& GetStock() { return stock; }
	//ワイルドブール引数
	void SetPause(const bool& pause) { this->pause = pause; }
	const bool& GetPause() { return pause; }
	//衝突判定
	void SetCollide(const bool& collide) { this->collide = collide; }
	const bool& GetCollide() { return collide; }

	//virtualにしようか悩み中//解決済み
	void Initialize(Model* model, const std::string& tag = "None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	void IntroUpdate(const int& Timer);
	void ResultUpdate(const int& Timer);

	void Demo();		//更新処理

	void FirstDraw(DirectXCommon* dxCommon);	//先に描画処理
	void Draw(DirectXCommon* dxCommon);	//描画処理
	void LastDraw(DirectXCommon* dxCommon);	//後で描画処理

	void DemoDraw(DirectXCommon* dxCommon);	//描画処理
	void Finalize();	//終了処理

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnFirstDraw(DirectXCommon* dxCommon) {};
	virtual void OnDraw(DirectXCommon* dxCommon) {};
	virtual void OnLastDraw(DirectXCommon* dxCommon) {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};
	virtual void IntroOnUpdate(const int& Timer) {};
	virtual void ResultOnUpdate(const int& Timer) {};
	virtual void OnCollision(const std::string& Tag) {};
protected:
	int command = 0;
	std::unique_ptr<Object3d> obj;
	// 残像データの数
#define AFTIMAGENUM	8
	float PlayerX[AFTIMAGENUM], RotY[AFTIMAGENUM],PlayerZ[AFTIMAGENUM];
	HitBound hitBound;
	XMFLOAT3 cameraPos{};
	float hp = 0;
	float angle = 0;
	bool canMove = true;
	bool pause = false;
	bool first = false;
	bool DeadFlag = false;
	
	bool collide = false;

	XMFLOAT3 oldPos = {};
	int stock = 0;
	//コンポーネント
	ActorComponent* compornent = nullptr;
};