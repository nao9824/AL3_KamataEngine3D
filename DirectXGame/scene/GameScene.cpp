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
	delete boss_;
	delete modelBoss_;

	delete bossHpSprite_;
	delete bossHpSprite2_;
	delete playerHpSprite_;
	delete playerHpSprite2_;
	delete playerBulletSprite_;
	delete playerBulletSprite2_;
	delete playerBulletSpriteSpecial_;
	delete UiSprite_;
	delete sousaSprite_;
	delete clearSprite_;
	delete gameoverSprite_;
}

void GameScene::Initialize() {

	finished_ = false;
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("player.png");
	textureHandleBoss_ = TextureManager::Load("mato.png");
	viewProjection_.farZ = 10000.0f;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//3Dモデルの生成
model_ = Model::Create();
//自キャラの生成
player_ = new Player();
// 3Dモデルの生成
modelPlayer_ = Model::CreateFromOBJ("player", true);
//自キャラの初期化
Vector3 playerPosition(0, -10, 50);
// 弾の3Dモデルの生成
modelBullet_ = Model::CreateFromOBJ("bullet", true);
player_->Initialize(modelPlayer_, modelBullet_, textureHandle_, playerPosition);
//敵キャラの発生
modelEnemy_ = Model::CreateFromOBJ("zakoteki", true);
// 
// 弾を生成し、初期化
modelEnemyBullet_ = Model::CreateFromOBJ("gareki", true);
//スカイドーム
skydome_ = new Skydome();
//3Dモデルの生成
modelSkydome_ = Model::CreateFromOBJ("AL3_skydome", true);
// スカイドームの初期化 
skydome_->Initialize(modelSkydome_, textureHandle_, &viewProjection_);
//デバッグカメラの生成
debugCamera_ = new DebugCamera(1280, 720);
//レールカメラの生成
railcamera_ = new RaillCamera();
railcamera_->Initialize(railPosition_, railRotate_);
// ボス
boss_ = new Boss();
// 3Dモデルの生成
modelBoss_ = Model::CreateFromOBJ("clock", true);
Vector3 bossPosition(0, 0, 80);
boss_->Initialize(modelBoss_, textureHandleBoss_, bossPosition);
//軸方向表示の意思を有効にする
//AxisIndicator::GetInstance()->SetVisible(true);
//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

//自キャラとレールカメラの親子関係を結ぶ
player_->SetParent(&railcamera_->GetWorldTransform());

// 弾の更新処理
for (EnemyBullet* bullet : enemyBullets_) {
	bullet->Update();
}

bossHp_ = TextureManager::Load("bossHp.png");
bossHpSprite_ = Sprite::Create(bossHp_, { 390, -40 });
bossHp2_ = TextureManager::Load("bossHp2.png");
bossHpSprite2_ = Sprite::Create(bossHp2_, { 390, -40 });
playerHp_ = TextureManager::Load("playerHp.png");
playerHpSprite_ = Sprite::Create(playerHp_, { 30, 470 });
playerHp2_ = TextureManager::Load("playerHp2.png");
playerHpSprite2_ = Sprite::Create(playerHp2_, { 30, 480 });
playerBullet_ = TextureManager::Load("playerBullet.png");
playerBulletSprite_ = Sprite::Create(playerBullet_, { 30, 550 });
playerBullet2_ = TextureManager::Load("playerBullet2.png");
playerBulletSprite2_ = Sprite::Create(playerBullet2_, { 30, 560 });
playerBulletSpecial_ = TextureManager::Load("playerBulletSpecial.png");
playerBulletSpriteSpecial_ = Sprite::Create(playerBulletSpecial_, { 30, 550 });
Ui_ = TextureManager::Load("AL3_zako.png");
UiSprite_ = Sprite::Create(Ui_, {950, 510});
UiSprite_->SetSize(Vector2{384.0f,192.0f});
sousa_ = TextureManager::Load("sousa.png");
sousaSprite_ = Sprite::Create(sousa_, {0, 0});
sousaSprite_->SetSize(Vector2{256.0f, 128.0f});


clear_ = TextureManager::Load("clear.png");
clearSprite_ = Sprite::Create(clear_, {340, 104});
gameover_ = TextureManager::Load("gameover.png");
gameoverSprite_ = Sprite::Create(gameover_, {340, 104});

bulletNum_ = 10;
isClear = -1;
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
	if (player_ != nullptr) {

		player_->Update(viewProjection_);
		if (player_->IsDead()) {
			delete player_;
			
			isClear = 0;
			player_ = nullptr;
			//XINPUT_STATE joyState;
			//// ゲームパッド未接続なら何もせず抜ける
			//if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			//	return;
			//}
			//if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE))&& isClear == 0) {
			//	finished_ = true;
			//}
			for (Enemy* enemy : enemys_) {
				enemy->SetPlayer(player_);
			}
			
		}
		bulletTimer_++;
		XINPUT_STATE joyState;
		// ゲームパッド未接続なら何もせず抜ける
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			// return;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && bulletTimer_%15==0 && bulletNum_>3){
			bulletNum_--;
			playerBulletSprite_->SetSize(vector2Subtract(playerBulletSprite_->GetSize(), {73.14f, 0}));
		} else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && bulletTimer_ % 15 == 0 && bulletNum_ <= 3 && bulletNum_ > 0) {
			bulletNum_--;
			playerBulletSpriteSpecial_->SetSize(vector2Subtract(playerBulletSpriteSpecial_->GetSize(), {170.66f, 0}));
		}

		if (bulletNum_ <= 0) {
			bulletNum_ = 10;
			playerBulletSprite_->SetSize(Vector2( 512.0f, 256.0f));
			playerBulletSpriteSpecial_->SetSize(Vector2(512.0f, 256.0f));
		}
	}
	//スカイドームの更新
	skydome_->Update();
	
	if (boss_ != nullptr) {

		// ボスの更新
		boss_->Update();
		if (boss_->IsDead()) {
			delete boss_;
			if (isClear == -1) {

				isClear = 1;
			}
			boss_ = nullptr;
			
			//player_->bossDead();
		}
	}

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
		if (player_ != nullptr && boss_ != nullptr) {

			railcamera_->Update();
		}
		viewProjection_.matView = railcamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railcamera_->GetViewProjection().matProjection;

	//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	
	if (player_ != nullptr && boss_ != nullptr) {

		for (Enemy* enemy : enemys_) {

			enemy->Update();
		}
	}
	// 弾の更新処理
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || input_->TriggerKey(DIK_SPACE)) && isClear != -1) {
		finished_ = true;
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
	if (boss_ != nullptr) {
		// ボスの描画
		boss_->Draw(viewProjection_);
	}
	//自キャラの描画
	if (player_ != nullptr) {

		if (player_->IsDead() == false) {

			player_->Draw(viewProjection_);
		}
	}
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

	if (player_ != nullptr) {

		player_->DrawUI();
	}

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	bossHpSprite_->Draw();
	bossHpSprite2_->Draw();
	playerHpSprite_->Draw();
	playerHpSprite2_->Draw();
	playerBulletSprite_->Draw();
	playerBulletSprite2_->Draw();
	playerBulletSpriteSpecial_->Draw();
	UiSprite_->Draw();
	sousaSprite_->Draw();
	if (isClear == 1) {

		clearSprite_->Draw();
	}
	if (isClear == 0) {

		gameoverSprite_->Draw();
	}
	


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	if (boss_ != nullptr) {
		if (player_ != nullptr) {

			// 判定対象AとBの座標
			Vector3 posA, posB;

			// 自弾リストの取得
			const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
			// 敵弾リストの取得
			const std::list<EnemyBullet*>& enemyBullets = GetBullets();

#pragma region // 自キャラと敵弾の当たり判定
#pragma endregion

			// 自キャラと敵弾全ての当たり判定
			for (EnemyBullet* bullet : enemyBullets) {
				// 自キャラの座標
				posA = player_->GetWorldPosition();
				// 敵弾の座標
				posB = bullet->GetWorldPosition();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

					// 自キャラの衝突時コールバックを呼び出す
					player_->OnCollision();
					playerHpSprite_->SetSize(vector2Subtract(playerHpSprite_->GetSize(), {102.4f, 0}));
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
#pragma region // 自キャラと敵キャラの当たり判定
#pragma endregion

			// 自キャラと敵キャラ全ての当たり判定

			// 自キャラの座標
			posA = player_->GetWorldPosition();
			for (Enemy* enemy : enemys_) {

				// 敵キャラの座標
				posB = enemy->GetWorldPosition();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + enemyRadius_) * (playerRadius_ + enemyRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					//player_->OnCollision();
					player_->bulletPuls();
					playerHpSprite_->SetSize(vector2Subtract(playerHpSprite_->GetSize(), {-102.4f, 0}));
					// 敵キャラの衝突時コールバックを呼び出す
					enemy->OnCollision();
				}
			}

#pragma region // 自弾とボスの当たり判定
#pragma endregion

			// 自弾とボス顔の当たり判定
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionKao();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossKaoRadius_) * (playerRadius_ + bossKaoRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollision();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(),{5.12f,0}));
				}
			}
			// 自弾とボス顔の当たり判定
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionMigikata();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossKataRadius_) * (playerRadius_ + bossKataRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollision();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {5.12f, 0}));
				}
			}
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionHidarikata();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossKataRadius_) * (playerRadius_ + bossKataRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollision();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {5.12f, 0}));
				}
			}
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionMigite();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossTeRadius_) * (playerRadius_ + bossTeRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollision();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {5.12f, 0}));
				}
			}
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionHidarite();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossTeRadius_) * (playerRadius_ + bossTeRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollision();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {5.12f, 0}));
				}
			}
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionMigiJakuten();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossJakutenRadius_) * (playerRadius_ + bossJakutenRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
    					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollisionJakuten();
					    bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {15.36f, 0}));
				}
			}
			for (PlayerBullet* bullet : playerBullets) {
				// 自弾の座標
				posA = bullet->GetWorldPosition();

				// 敵ボスの座標
				posB = boss_->GetWorldPositionHidariJakuten();

				if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) <=
				    (playerRadius_ + bossJakutenRadius_) * (playerRadius_ + bossJakutenRadius_)) {

					// 自弾の衝突時コールバックを呼び出す
					bullet->OnCollision();
					// ボスの衝突時コールバックを呼び出す
					boss_->OnCollisionJakuten();
					bossHpSprite_->SetSize(vector2Subtract(bossHpSprite_->GetSize(), {15.36f, 0}));
				}
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
	if (player_ != nullptr) {

		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(modelEnemy_,modelEnemyBullet_, position, ApproachVelocity_, LeaveVelocity_);
		// 敵キャラに自キャラのアドレスを渡す
		newEnemy->SetPlayer(player_);

		// 敵キャラにゲームシーンを渡す
		newEnemy->SetGameScene(this);

		enemys_.push_back(newEnemy);
	}

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


