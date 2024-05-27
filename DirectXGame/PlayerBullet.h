#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラの弾
/// </summary>

class PlayerBullet {
	public:
	/// <summary>
	/// 初期化
	/// </summary>
	    void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	    void Update();

	/// <summary>
	/// 更新
	/// </summary>
	    void Draw(const ViewProjection& viewProjection);

		/// <summary>
		/// getter
		/// </summary>
	    bool IsDead() const { return isDead_; }

		// 衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		// ワールド座標を取得
	    Vector3 GetWorldPosition();
		private:
	    // ワールドデータ変換
	    WorldTransform worldTransform_;
	    // モデル
	    Model* model_ = nullptr;
	    // テクスチャハンドル
	    uint32_t textureHandle_ = 0u;
		//速度
	    Vector3 velocity_;
		//寿命
	    static const int32_t kLifeTime = 60 * 5;
		//デスタイマー
	    int32_t deathTimer_ = kLifeTime;
		//デスフラグ
	    bool isDead_ = false;
};
