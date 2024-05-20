#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include <list>
#include "mathFunction.h"

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

/// <summary>
/// 弾発射
/// </summary>
	void Fire();

	//発射間隔
	static const int kFireIntervel = 60;

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
};
