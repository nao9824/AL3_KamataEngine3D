#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

/// <summary>
/// 衝突判定と応答
/// </summary>
	void CheckAllCollisions();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//3Dモデルデータ
	Model* model_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;
	float playerRadius_ = 1.0f;
	//敵キャラ
	Enemy* enemy_ = nullptr;
	Vector3 position_ = {20.0f, 0, 22.0f};
	Vector3 ApproachVelocity_ = {0, 0, 0.0f};
	Vector3 LeaveVelocity_ = {0.0f, 0.0f, 0.0f};
	float enemyRadius_ = 1.0f;
	//スカイドーム
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
};
