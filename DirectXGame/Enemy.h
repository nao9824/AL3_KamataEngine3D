#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "mathFunction.h"

class Player; 

class Enemy {
public:
	///< summary>
	/// 初期化
	///< summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& ApproachVelocity, const Vector3& LeaveVelocity);

	///< summary>
	/// デストラクタ
	///< summary>
	~Enemy();

	///< summary>
	/// 更新
	///< summary>
	void Update();

	///< summary>
	/// 描画
	///< summary>
	void Draw(const ViewProjection& viewProjection);

   //行動フェーズ
	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};

	//接近フェーズ初期化
	void approachInitialize();
	// 接近フェーズ更新
	void approachUpdate();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

/// <summary>
/// 弾発射
/// </summary>
	void Fire();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//発射間隔
	static const int kFireIntervel = 60;

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 ApproachVelocity_;
	Vector3 LeaveVelocity_;
	//フェーズ
	Phase phase_ = Phase::Approach;
	// 弾
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t bulletTimer_ = 0;

	// 自キャラ
	Player* player_ = nullptr;
	//差分ベクトル
	Vector3 subtractVector_;
	//正規化したベクトル
	Vector3 normalVector_;
	//長さと速さ合わせたやつ
	Vector3 nsVector_;
};
