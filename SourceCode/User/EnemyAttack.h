#pragma once
#include"Object2d.h"


class EnemyAttack {
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyAttack();
	
	~EnemyAttack();
	void Init();
	void Upda();
	void Draw();

	bool Run();
	void Stamp(XMFLOAT3 pos);
private:
	bool burning = false;
	float scale = 0.0f;
	float effectRate = 0.0f;
	std::unique_ptr<Object2d> Explo = nullptr;

};