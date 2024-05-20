#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "EnemyBullet.h"

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

	// 弾の更新処理
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	

	approachInitialize();
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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

	// 弾の更新処理
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	approachInitialize();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::approachInitialize() {
	//発射タイマーカウントダウン
	bulletTimer_--;
	//指定時間に達した
	if (bulletTimer_ <= 0) {
	//弾を発射
		Fire();
	// 発射タイマーを初期化
	bulletTimer_ = kFireIntervel;
	}


}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}
