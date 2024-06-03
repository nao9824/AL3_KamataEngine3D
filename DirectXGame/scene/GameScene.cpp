#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete model_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player.png");
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//3Dモデルの生成
	model_ = Model::Create();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	//敵キャラの発生
	enemy_ = new Enemy();
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	if (enemy_ != nullptr) {
		// 敵キャラの初期化
		enemy_->Initialize(model_, position_, ApproachVelocity_,LeaveVelocity_);
	}
	//スカイドーム
	skydome_ = new Skydome();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("uvCheck", true);
	// スカイドームの初期化 
	skydome_->Initialize(modelSkydome_, textureHandle_, &viewProjection_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の意思を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	CheckAllCollisions();
	//自キャラの更新
	player_->Update();
	//スカイドームの更新
	skydome_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_E)) {
		isDebugCameraActive_ = true;
	}
	#endif

	//カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
	//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
	if (enemy_ != nullptr) {
		enemy_->Update();
	}
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}
	//スカイドームの描画
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region //自キャラと敵弾の当たり判定
	#pragma endregion
	
	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 自キャラの座標
		posA = player_->GetWorldPosition();
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		if ((posB.x - posA.x) * (posB.x - posA.x) + 
			(posB.y - posA.y) * (posB.y - posA.y) + 
			(posB.z - posA.z) * (posB.z - posA.z) <= (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	#pragma region // 自弾と敵キャラの当たり判定
    #pragma endregion
	
	// 自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posA = bullet->GetWorldPosition();
		// 敵キャラの座標
		posB = enemy_->GetWorldPosition();

		if ((posB.x - posA.x) * (posB.x - posA.x) + 
			(posB.y - posA.y) * (posB.y - posA.y) + 
			(posB.z - posA.z) * (posB.z - posA.z) <= (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
		}
	}
	#pragma region // 自弾と敵弾の当たり判定
    #pragma endregion
	// 自弾と敵弾全ての当たり判定
	for (PlayerBullet* Pbullet : playerBullets) {
		for (EnemyBullet* Ebullet : enemyBullets) {

			// 自弾の座標
			posA = Pbullet->GetWorldPosition();
			// 敵弾の座標
			posB = Ebullet->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
			    (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

				// 自弾の衝突時コールバックを呼び出す
				Pbullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				Ebullet->OnCollision();
			}
		}
	}
}
