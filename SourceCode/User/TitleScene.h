#pragma once
#include "SceneManager.h"
#include "Actor.h"
#include"TitleText.h"
#include <thread>

/// <summary>
/// タイトルシーン
/// </summary>

class TitleScene : public BaseScene {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	void Heavy();
private:
	//メンバ変数
	enum {
		BackScene,
		Button,
		moveTitle,
		nomoveTitle,
		moveDebug,
		nomoveDebug,
		SpriteMax,
	};
	std::unique_ptr<Sprite> UI[SpriteMax];
	TitleText* text;

	enum Portal {
		Title=0,
		Debug,

	};

	int nextScene = Portal::Title;

	XMFLOAT2 SetPos[3]={ { 640.0f,300.0f } ,{ 640.0f,360.0f } ,{ 640.0f,460.0f } };
	
	float moveText[2]{};
	int moveTitleNow = 1;
	int moveDebugNow = 2;

	std::string SceneName = "null";

	enum {
		UnLoad = 0,
		Loading,
		LoadFinish
	};
	static int LoadState;
	//static 画面 = ゲーム画面;
};

