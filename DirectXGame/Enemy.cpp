#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& ApproachVelocity, const Vector3& LeaveVelocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("gensokuhiyaemon.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	ApproachVelocity_ = ApproachVelocity;
	LeaveVelocity_ = LeaveVelocity;
}

Enemy::~Enemy() {}

void Enemy::Update() { 

	switch (phase_) { 
	case Phase::Approach:
	default:
		// 座標を移動させる
		worldTransform_.translation_.x += ApproachVelocity_.x;
		worldTransform_.translation_.y += ApproachVelocity_.y;
		worldTransform_.translation_.z += ApproachVelocity_.z;
		//既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		// 座標を移動させる
		worldTransform_.translation_.x += LeaveVelocity_.x;
		worldTransform_.translation_.y += LeaveVelocity_.y;
		worldTransform_.translation_.z += LeaveVelocity_.z;
		break;
	}

	
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
