#include "Enemy.h"
#include "assert.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model,Model* bulletModel, const Vector3& position, const Vector3& ApproachVelocity, const Vector3& LeaveVelocity) {
	// NULLポインタチェック
	assert(model);

	model_ = model;
	bulletModel_ = bulletModel;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("gensokuhiyaemon.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期化座標をセット
	worldTransform_.translation_ = position;
	// 引数で受け取った速度をメンバ変数に代入
	ApproachVelocity_ = ApproachVelocity;
	LeaveVelocity_ = LeaveVelocity;

	approachInitialize();
	
	
}

Enemy::~Enemy() {
	
}

void Enemy::Update() { 
	//if (isStop == false) {

		approachUpdate();

		switch (phase_) {
		case Phase::Approach:
		default:
			// 座標を移動させる
			worldTransform_.translation_.x += ApproachVelocity_.x;
			worldTransform_.translation_.y += ApproachVelocity_.y;
			worldTransform_.translation_.z += ApproachVelocity_.z;
			// 既定の位置に到達したら離脱
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
	//}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection);

	
}

void Enemy::approachInitialize() {
	// 発射タイマーを初期化
	bulletTimer_ = kFireIntervel;
}

void Enemy::approachUpdate() {
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

//void Enemy::Stop(float time) { 
//	time--;
//	if (time >= 0) {
//		isStop = true;
//	}
//	if (time < 0) {
//		isStop = false;
//	}
//
//}

void Enemy::Fire() {
	assert(player_);
	// 弾の速度
	const float kBulletSpeed = 0.1f;
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 p = player_->GetWorldPosition();
	Vector3 e = GetWorldPosition();

	subtractVector_ = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	
	normalVector_ = Normalize(subtractVector_);
	velocity = vectorMultiply(kBulletSpeed, normalVector_);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(bulletModel_, worldTransform_.translation_, velocity);

	// 弾を登録する
	//enemyBullets_.push_back(newBullet);
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::OnCollision() { isDead_ = true; }

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
