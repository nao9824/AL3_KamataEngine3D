#include "player.h"
#include "cassert"
#include "ImGuiManager.h"


void Player::Initialize(Model* model, uint32_t textureHandle) {//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTranshorm_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

Player::~Player() { 
	
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	}




void Player::Update() {
	worldTranshorm_.TransferMatrix(); 

	//デスフラグの立った球を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	//左右
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;	
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	//上下
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動
	worldTranshorm_.translation_ = Add(worldTranshorm_.translation_, move);
	
	Rotate();
	Attack();

	//弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//移動限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 20;

	//範囲を超えない処理
	worldTranshorm_.translation_.x = max(worldTranshorm_.translation_.x, -kMoveLimitX);
	worldTranshorm_.translation_.x = min(worldTranshorm_.translation_.x, +kMoveLimitX);
	worldTranshorm_.translation_.y = max(worldTranshorm_.translation_.y, -kMoveLimitY);
	worldTranshorm_.translation_.y = min(worldTranshorm_.translation_.y, +kMoveLimitY);

	
	//行列更新
	worldTranshorm_.matWorld_ = MakeAffineMatrix(worldTranshorm_.scale_, worldTranshorm_.rotation_, worldTranshorm_.translation_);
	//WorldTransform::UpdateMatrix();

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Debug1");
	ImGui::Text("Kamata Tarou %.02f,%.02f,%.02f", worldTranshorm_.translation_.x, worldTranshorm_.translation_.y, worldTranshorm_.translation_.z);
	ImGui::End();

	
}

void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTranshorm_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTranshorm_.rotation_.y += kRotSpeed;
	}
}



void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTranshorm_.matWorld_);

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTranshorm_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTranshorm_, viewProjection, textureHandle_); 

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

