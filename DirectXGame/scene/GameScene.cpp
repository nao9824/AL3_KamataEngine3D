#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete model_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	
	delete skydome_;
	delete modelSkydome_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player.png");
	viewProjection_.farZ = 10000.0f;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//3Dモデルの生成
	model_ = Model::Create();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition(0, 0, 50);
	player_->Initialize(model_, textureHandle_,playerPosition);
	//敵キャラの発生
	// 弾を生成し、初期化
	
	//スカイドーム
	skydome_ = new Skydome();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("uvCheck", true);
	// スカイドームの初期化 
	skydome_->Initialize(modelSkydome_, textureHandle_, &viewProjection_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//レールカメラの生成
	railcamera_ = new RaillCamera();
	railcamera_->Initialize(railPosition_,railRotate_);
	//軸方向表示の意思を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railcamera_->GetWorldTransform());

	// 弾の更新処理
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

}

void GameScene::Update() {
	LoadEnemyPopData();
	UpdateEnemyPopCommands();

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	CheckAllCollisions();
	//自キャラの更新
	player_->Update(viewProjection_);
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
		// レールカメラの処理
		railcamera_->Update();
		viewProjection_.matView = railcamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railcamera_->GetViewProjection().matProjection;

	//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	for (Enemy* enemy : enemys_) {
		
		enemy->Update();
	}
	// 弾の更新処理
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
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

	// スカイドームの描画
	skydome_->Draw(viewProjection_);
	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	
		for (Enemy* enemy : enemys_) {

			enemy->Draw(viewProjection_);
		}
	
	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	
	player_->DrawUI();

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
	const std::list<EnemyBullet*>& enemyBullets = GetBullets();

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
		for (Enemy* enemy : enemys_) {

			// 敵キャラの座標
			posB = enemy->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
			    (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

				// 自弾の衝突時コールバックを呼び出す
 				bullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
			}
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

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::enemyHassei(Vector3 position) { 
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model_, position, ApproachVelocity_, LeaveVelocity_);
	// 敵キャラに自キャラのアドレスを渡す
	newEnemy->SetPlayer(player_);

	// 敵キャラにゲームシーンを渡す
	newEnemy->SetGameScene(this);

	enemys_.push_back(newEnemy);

}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
		//待機完了
			isWait = false;
		}
		return;
	}

//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	//1行分の文字列をストリームに変換して更新しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
		//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
		    //x座標
		    	getline(line_stream, word, ',');
		    float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			enemyHassei(Vector3(x,y,z));

		}//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			isWait = true;
			waitTimer = (float)waitTime;
			//コマンドループを抜ける
			break;
		}
	}

}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録
	enemyBullets_.push_back(enemyBullet);
}
