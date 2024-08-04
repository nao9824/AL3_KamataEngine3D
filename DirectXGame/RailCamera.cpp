#include "RailCamera.h"
#include "mathFunction.h"
//#include "imgui.h"

void RaillCamera::Initialize(const Vector3& position, const Vector3& rotate) {
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	velocity_ = {0.0f, 0.0f, 0.05f};
	viewProjection_.Initialize();

}

void RaillCamera::Update() {
	// 座標を移動させる
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	//座標を回転させる
	worldTransform_.rotation_.x += rotate_.x;
	worldTransform_.rotation_.y += rotate_.y;
	worldTransform_.rotation_.z += rotate_.z;

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	// 行列更新
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラの座標を画面表示する処理
	/*ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTranslate", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();*/

}
