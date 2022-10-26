#pragma once
#include"Sprite.h"
#include <memory>

class PauseUI {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PauseUI();
	~PauseUI();

	void Initialize();
	void Update();
	void Draw();
	void Reset();

	const bool& GetEase() { return ease; }
	const int& GetBar() { return nowBar; }
private:
	enum {
		Sheet,
		TitleBack,
		Option,
		ZBack,
		Bar,
		Max
	};
	std::unique_ptr<Sprite> UI[Max];

	XMFLOAT2 move{};

	float SetPos = 250;
	int nowBar = 0;
	bool ease = false;
	float frame = 0;
	XMFLOAT2 pos={640,360};
	XMFLOAT2 size[Max]{  };
};
