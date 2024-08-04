#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "mathFunction.h"

class Boss {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 bossPosition);

	void Update();

	~Boss();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();
	void OnCollisionJakuten();
	bool IsDead() const { return isDead_; }
	// ワールド座標を取得
	Vector3 GetWorldPositionKao();
	Vector3 GetWorldPositionMigikata();
	Vector3 GetWorldPositionHidarikata();
	Vector3 GetWorldPositionMigite();
	Vector3 GetWorldPositionHidarite();
	Vector3 GetWorldPositionMigiJakuten();
	Vector3 GetWorldPositionHidariJakuten();

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	WorldTransform worldTransformJ_;
	WorldTransform worldTransformJL_;
	// モデル
	Model* model_ = nullptr;
	Model* modelJ_ = nullptr;
	Model* modelJL_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 速度
	Vector3 velocity_;

	int32_t hp_;
	// デスフラグ
	bool isDead_ = false;

};