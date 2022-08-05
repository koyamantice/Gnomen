#pragma once
#include<vector>
#include <memory>
#include<string>
#include"Object3d.h"
#include<DirectXMath.h>

class ActorComponent;

class Actor 
{
private:
	static int ID;
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	//名前
	std::string name;
	//付与されているタグへのポインタ
	std::string tag = "None";
protected:
	//オブジェクト識別番号(絶対に被ることはない数字)
	static int id;
	//機能中か
	bool isActive = true;
	//削除
	bool isRemove = false;
public:
	virtual ~Actor() = default;
	//座標の取得
	void SetPosition(const DirectX::XMFLOAT3& pos) { obj->SetPosition(pos); }
	const DirectX::XMFLOAT3& GetPosition() { return obj->GetPosition(); }
	//角度の取得
	void SetRotation(const DirectX::XMFLOAT3& rot) { obj->SetRotation(rot); }
	const DirectX::XMFLOAT3& GetRotation() { return obj->GetRotation(); }

	//isAliveセッタ＆ゲッタ
	void SetHp(const float& hp) { this->hp = hp; };
	const float& GetHp() { return hp; }

	//
	virtual const XMFLOAT3& GetLockPos() { return obj->GetPosition(); }
	//ID取得
	const int& GetID()const { return id; }

	//名前セッタ＆ゲッタ
	void SetName(const std::string& Name) { name = Name; }
	const std::string& GetName() { return name; }

	//タグのセット
	void SetTag(const std::string& Tag){tag = Tag;};
	//タグが一致するか
	const std::string& GetTag() { return tag; }

	//isAliveセッタ＆ゲッタ
	void SetIsActive(const bool& Active) {isActive = Active;};
	const bool& GetIsActive() { return isActive; }

	//isRemoveセッタ＆ゲッタ
	void SetIsRemove(const bool& Remove) {isRemove = Remove;};
	const bool& GetIsRemove() { return isRemove; }

	//canMoveセッタ＆ゲッタ
	void SetCanMove(const bool& canMove) { this->canMove = canMove; };
	const bool& GetCanMove() { return canMove; }

	//カメラ角度のセッタ＆ゲッタ
	void SetAngle(const float& angle) { this->angle = angle; }
	const float& GetAngle() { return angle; }
	//
	void SetStock(const int& Stock) {this->stock = Stock;};
	const int& GetStock() { return stock; }

	//virtualにしようか悩み中//解決済み
	void Initialize(Model* model,const std::string& tag="None", ActorComponent* compornent = nullptr);	//初期化処理
	void Update();		//更新処理
	void Demo();		//更新処理
	void Draw();	//描画処理
	void DemoDraw();	//描画処理
	void Finalize();	//終了処理

	virtual void OnInit() {};
	virtual void OnUpda() {};
	virtual void OnDraw() {};
	virtual void OnFinal() {};
	virtual void DebugUpdate() {};

	virtual void OnCollision(){};
protected:
	std::unique_ptr<Object3d> obj;
	float hp = 0;
	float angle = 0;
	bool canMove = true;

	int stock = 0;
	//コンポーネント
	ActorComponent* compornent = nullptr;
};