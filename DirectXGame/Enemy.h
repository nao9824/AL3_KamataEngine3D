#pragma once
#include "Model.h"
#include "WorldTransform.h"

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
};
