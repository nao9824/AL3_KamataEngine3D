#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "mathFunction.h"

class Boss {
public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 bossPosition);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};