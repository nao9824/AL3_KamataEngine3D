#include "player.h"
#include "cassert"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "WinApp.h"

void Player::Initialize(Model* model, Model* bulletModel, uint32_t textureHandle, Vector3 playerPosition) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	bulletModel_ = bulletModel;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	hp_ = 5;

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//レティクル用テクスチャ取得
	textureReticle = TextureManager::Load("reticle.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1,1,1,1},{0.5,0.5});

	bulletNum_ = 10;
	bulletTimer_ = 0;
}

Player::~Player() { 
	
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
	}




void Player::Update(const ViewProjection& viewProjection) {
	
	

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

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	//スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	
	//ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);
	}

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
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	
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
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = vectorMultiply(kDistancePlayerTo3DReticle,Normalize(offset));
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(offset,GetWorldPosition());

	Vector3 positionReticle = GetWorldPosition3D();
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = 
		matrixMultiply(matrixMultiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	//POINT mousePosition;
	//マウス座標を取得
	//GetCursorPos(&mousePosition);
	//クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);//ここでマウスポジをクライアントエリアに変換してる
	//sprite2DReticle_->SetPosition(Vector2((float)mousePosition.x,(float)mousePosition.y));
	//ビュープロジェクションビューポート行列
	Matrix4x4 matVPV = matrixMultiply(matrixMultiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3((float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar = Vector3((float)sprite2DReticle_->GetPosition().x, (float)sprite2DReticle_->GetPosition().y, 1);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	//マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	//カメラから標準オブジェクトの距離
	const float kDistanceTestObject = 130;
	worldTransform3DReticle_.translation_ = Add(posNear, vectorMultiply(kDistanceTestObject, mouseDirection));


	//行列更新
	//worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	
	worldTransform_.UpdateMatrix();
	worldTransform3DReticle_.UpdateMatrix();

	//転送
	worldTransform_.TransferMatrix();
	worldTransform3DReticle_.TransferMatrix();

	//キャラクターの座標を画面表示する処理
	/*ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", positionReticle.x, positionReticle.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::Text("hp:%d", hp_);

	ImGui::End();*/

	
}

void Player::Rotate() {
	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}



void Player::Attack() {
	bulletTimer_++;
	XINPUT_STATE joyState;
	//ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		//return;
	}

	if ((hp_>0 && (input_->TriggerKey(DIK_SPACE) || 
		joyState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER)) && bulletTimer_ % 15 == 0) {
		
		if (bulletNum_ > 3) {
			bulletNum_--;
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			Vector3 velocity(0, 0, kBulletSpeed);

			// 速度ベクトルを自機の向きに合わせて回転させる
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

			// 自機から照準オブジェクトへのベクトル
			velocity = Subtract(GetWorldPosition3D(), GetWorldPosition());
			velocity = vectorMultiply(kBulletSpeed, Normalize(velocity));

			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

			// 弾を登録する
			bullets_.push_back(newBullet);
		} else if (bulletNum_ <= 3 && bulletNum_ >0) {
			bulletNum_--;
			SpecialAttack();
		}

		if (bulletNum_ <= 0) {
			bulletNum_ = 10;
		}
		
	}
}

void Player::SpecialAttack() {
	// 弾の速度
	const float kBulletSpeed1 = 2.0f;
	const float kBulletSpeed2 = 2.0f;
	const float kBulletSpeed3 = 2.0f;
	Vector3 velocity1(0, 0, kBulletSpeed1);
	Vector3 velocity2(0, 0, kBulletSpeed2);
	Vector3 velocity3(0, 0, kBulletSpeed2);

	// 速度ベクトルを自機の向きに合わせて回転させる
	// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 自機から照準オブジェクトへのベクトル
	velocity1 = Subtract(GetWorldPosition3D(), GetWorldPosition());
	velocity1 = vectorMultiply(kBulletSpeed1, Normalize(velocity1));
	velocity2 = Subtract(GetWorldPosition3D(), GetWorldPosition());
	velocity2 = vectorMultiply(kBulletSpeed2, Normalize(velocity2));
	velocity3 = Subtract(GetWorldPosition3D(), GetWorldPosition());
	velocity3 = vectorMultiply(kBulletSpeed3, Normalize(velocity3));

	// 弾を生成し、初期化
	PlayerBullet* newBullet1 = new PlayerBullet();
	PlayerBullet* newBullet2 = new PlayerBullet();
	PlayerBullet* newBullet3 = new PlayerBullet();
	newBullet1->Initialize(bulletModel_, GetWorldPosition(), velocity1);
	newBullet2->Initialize(bulletModel_, Add(GetWorldPosition(), {-3, 1, 0}), velocity2);
	newBullet3->Initialize(bulletModel_, Add(GetWorldPosition(), {3, 1, 0}), velocity3);

	// 弾を登録する
	bullets_.push_back(newBullet1);
	bullets_.push_back(newBullet2);
	bullets_.push_back(newBullet3);
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}

void Player::bulletPuls() {
	if (hp_ < 5) {
		hp_++;
	}
}

Vector3 Player::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetWorldPosition3D() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}


void Player::SetParent(const WorldTransform* parent) {
//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::bossDead() {
	if (worldTransform_.rotation_.y <= 360) {
		worldTransform_.rotation_.y++;
	}
}

