#include "boss.h"
#include "assert.h"
#include "TextureManager.h"
//#include "imgui.h"

void Boss::Initialize(Model* model, uint32_t textureHandle, Vector3 bossPosition) {
	//NULLチェック
	assert(model);

	model_ = model;

	modelJ_ = Model::Create();
	modelJL_ = Model::Create();

	//テクスチャ読み込み
	//textureHandle_ = TextureManager::Load("")
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransformJ_.Initialize();
	worldTransformJL_.Initialize();

	worldTransform_.translation_ = bossPosition;
	worldTransformJ_.translation_ = GetWorldPositionMigiJakuten();
	worldTransformJL_.translation_ = GetWorldPositionHidariJakuten();

	worldTransform_.scale_ = {8, 8, 8};
	worldTransformJ_.scale_ = {7, 7, 7};
	worldTransformJ_.rotation_ = {0, 0.1f, 0};
	worldTransformJL_.scale_ = {7, 7, 7};
	worldTransformJL_.rotation_ = {0, -0.1f, 0};

	
	velocity_ = {0.0f, 0.0f, 0.05f};
	// 引数の内容をメンバ変数に記録
	//viewProjection_ = viewProjection;

	hp_ = 100;
}

void Boss::Update() {

	
	// 座標を移動させる
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	// 行列更新
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransformJ_.translation_ = GetWorldPositionMigiJakuten();
	worldTransformJL_.translation_ = GetWorldPositionHidariJakuten();

	// 行列の定数バッファに転送
	worldTransform_.UpdateMatrix();
	worldTransformJ_.UpdateMatrix();
	worldTransformJL_.UpdateMatrix();

	
	

	/*ImGui::Begin("Boss");
	ImGui::Text("bossHp:%d", hp_);

	ImGui::End();*/
}

Boss::~Boss() {
	delete modelJ_;
	delete modelJL_;
}

void Boss::Draw(const ViewProjection& viewProjection) {
//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
modelJ_->Draw(worldTransformJ_, viewProjection,textureHandle_);
modelJL_->Draw(worldTransformJL_, viewProjection,textureHandle_);
}

void Boss::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

void Boss::OnCollisionJakuten() {
	hp_ -= 3;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

Vector3 Boss::GetWorldPositionKao() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {0.0f, 2.5f, 0.0f};
	Vector3 result{};

	
	result = Transform(worldPos, worldTransform_.matWorld_);
	

	// ワールド行列の平行移動成分を取得(ワールド座標)
	//worldPos.x = worldTransform_.matWorld_.m[3][0];
	//worldPos.y = worldTransform_.matWorld_.m[3][1];
	//worldPos.z = worldTransform_.matWorld_.m[3][2];

	return result;
}

Vector3 Boss::GetWorldPositionMigikata() { 
// ワールド座標を入れる変数
	Vector3 worldPos = {2.8f, 2.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}

Vector3 Boss::GetWorldPositionHidarikata() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {-2.9f, 2.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}

Vector3 Boss::GetWorldPositionMigite() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {4.7f, 0.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}

Vector3 Boss::GetWorldPositionHidarite() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {-4.5f, 0.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}

Vector3 Boss::GetWorldPositionMigiJakuten() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {1.6f, 2.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}

Vector3 Boss::GetWorldPositionHidariJakuten() {
	// ワールド座標を入れる変数
	Vector3 worldPos = {-1.6f, 2.0f, 0.0f};
	Vector3 result{};

	result = Transform(worldPos, worldTransform_.matWorld_);

	return result;
}
