#include "boss.h"
#include "assert.h"
#include "TextureManager.h"

void Boss::Initialize(Model* model, uint32_t textureHandle, Vector3 bossPosition) {
	//NULLチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	//textureHandle_ = TextureManager::Load("")
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = bossPosition;
	// 引数の内容をメンバ変数に記録
	//viewProjection_ = viewProjection;

}

void Boss::Update() {
	// 行列の定数バッファに転送
	worldTransform_.TransferMatrix();

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Boss::Draw(const ViewProjection& viewProjection) {
//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
