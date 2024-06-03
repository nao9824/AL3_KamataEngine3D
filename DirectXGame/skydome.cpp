#include "skydome.h"
#include "assert.h"
#include "mathFunction.h"

void Skydome::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection)
{
	//NULLポインタチェック
	assert(model);
model_ = model;
textureHandle_ = textureHandle;

worldTransform_.Initialize();
//引数の内容をメンバ変数に記録
viewProjection_ = viewProjection;
}

void Skydome::Update() {
	//行列の定数バッファに転送
	worldTransform_.TransferMatrix();

	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
}

void Skydome::Draw(const ViewProjection& viewProjection) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
