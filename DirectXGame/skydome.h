#pragma 
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model, uint32_t textureHandle_, ViewProjection* viewProjection);

	void Update();

	void Draw(const ViewProjection& viewProjection);


private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// モデル
	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

};