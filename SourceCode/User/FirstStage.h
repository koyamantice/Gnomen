#pragma once
#include "SceneManager.h"


/// <summary>
/// ボス：キノコ　第一ステージ
/// </summary>
class FirstStage : public BattleScene {
private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update(DirectXCommon* dxCommon) override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;
	/// <summary>
	/// /// 終了
	/// </summary>
	void Finalize() override;
private:
	/// <summary>
	/// 導入カメラ
	/// </summary>
	/// <param name="Timer"></param>
	void IntroCamera(const float& Timer);
	
	/// <summary>
	/// クリア後カメラ
	/// </summary>
	/// <param name="Timer"></param>
	void ResultCamera(const float& Timer);

	/// <summary>
	///	導入部分の更新処理
	/// </summary>
	/// <returns></returns>
	bool IntroUpdate()override;

	/// <summary>
	/// 説明表示用の更新
	/// </summary>
	void DescriptionUpdate() override;

	/// <summary>
	/// クリア更新処理
	/// </summary>
	bool ClearUpdate() override;

	/// <summary>
	/// ゲームオーバーの更新処理
	/// </summary>
	void GameOverUpdate()override;

	float intro_count = 0;
	const float intro_count_max = 800;

	float intro_speed = 1;
	float intro_speed_max = 2;

	const float first_hight = 50.0f;
	//導入部分変数
	static const int intro_word_max = 6;
	int nowWord = 0;
	std::unique_ptr<Sprite> IntroWord[intro_word_max];
private:
	std::unique_ptr<Sprite> Clear{};
	std::unique_ptr<Sprite> Over{};

	//コントローラーの説明
	std::unique_ptr<Sprite> Camecon[6]{};
	std::unique_ptr<Sprite> con_vis[2]{};

	XMFLOAT2 base = { 640,530 };
	float camera_frame = 0.0f;
	int animation = 0;
	int tapanima = 3;
	int animafrate = 0;
	int vec = 1;


	PostEffect* postEffect = nullptr;
};

