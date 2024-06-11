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
#include"EnemyBullet.h"
#include "skydome.h"
#include "RailCamera.h"
#include <list>
#include <sstream>

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

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	//敵発生データの読み込み
	void LoadEnemyPopData();

	//敵を発生させる関数
	void enemyHassei(Vector3 position);

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

/// <summary>
/// 敵弾を追加する
/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

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
	
	std::list<Enemy*> enemys_;
	Vector3 position_ = {20.0f, 0, 22.0f};
	Vector3 ApproachVelocity_ = {0, 0, -0.01f};
	Vector3 LeaveVelocity_ = {0.0f, 0.0f, 0.01f};
	float enemyRadius_ = 1.0f;
	std::stringstream enemyPopCommands;
	bool isWait;
	float waitTimer;
	// 弾
	
	std::list<EnemyBullet*> enemyBullets_;

	
	//スカイドーム
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;
	//レールカメラ
	RaillCamera* railcamera_ = nullptr;
	Vector3 railPosition_ = {0.0f, 0.0f, -50.0f};
	Vector3 railRotate_ = {0.0f, 0.0f, 0.0f};
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
};
