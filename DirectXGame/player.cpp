#include "player.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle) {//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTranshorm_.Initialize();
}

void Player::Update() { worldTranshorm_.TransferMatrix(); }

void Player::Draw(ViewProjection& viewProjection) { model_->Draw(worldTranshorm_, viewProjection, textureHandle_); }

