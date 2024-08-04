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
#include "boss.h"

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


	bool IsFinished() const {return finished_;}

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandleBoss_ = 0;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//3Dモデルデータ
	Model* model_ = nullptr;
	//自キャラ
	Player* player_ = nullptr;
	// 3Dモデル
	Model* modelPlayer_ = nullptr;
	float playerRadius_ = 1.2f;
	Model* modelBullet_ = nullptr;

	//敵キャラ
	
	std::list<Enemy*> enemys_;
	Model* modelEnemy_ = nullptr;
	Model* modelEnemyBullet_ = nullptr;
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
	Vector3 railPosition_ = {0.0f, 20.0f, -50.0f};
	Vector3 railRotate_ = {0.08f, 0.0f, 0.0f};
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//ボス
	Boss* boss_ = nullptr;
	//ボス3Dモデル
	Model* modelBoss_ = nullptr;
	float bossKaoRadius_ = 15.0f;
	float bossKataRadius_ = 10.0f;
	float bossTeRadius_ = 10.0f;
	float bossJakutenRadius_ = 10.0f;


	bool finished_ = false;

	uint32_t bossHp_ = 0;
	Sprite* bossHpSprite_ = nullptr;
	uint32_t bossHp2_ = 0;
	Sprite* bossHpSprite2_ = nullptr;

	uint32_t playerHp_ = 0;
	Sprite* playerHpSprite_ = nullptr;
	uint32_t playerHp2_ = 0;
	Sprite* playerHpSprite2_ = nullptr;
	uint32_t playerBullet_ = 0;
	Sprite* playerBulletSprite_ = nullptr;
	uint32_t playerBullet2_ = 0;
	Sprite* playerBulletSprite2_ = nullptr;
	uint32_t playerBulletSpecial_ = 0;
	Sprite* playerBulletSpriteSpecial_ = nullptr;
	uint32_t Ui_ = 0;
	Sprite* UiSprite_ = nullptr;
	uint32_t sousa_ = 0;
	Sprite* sousaSprite_ = nullptr;

	uint32_t clear_ = 0;
	Sprite* clearSprite_ = nullptr;
	uint32_t gameover_ = 0;
	Sprite* gameoverSprite_ = nullptr;
	int bulletTimer_;
	int bulletNum_;

	int isClear;
};
