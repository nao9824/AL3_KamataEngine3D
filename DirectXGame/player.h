#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "mathFunction.h"

///<summary>
///自キャラ
///<summary>
class Player {
public:
	///< summary>
	/// 初期化
	///< summary>
	void Initialize(Model* model,uint32_t textureHandle);

	///< summary>
	/// デストラクタ
	///< summary>
	~Player();

	///< summary>
	/// 更新
	///< summary>
	void Update();
	void Rotate();

	///< summary>
	/// 攻撃
	///< summary>
	void Attack();

	///< summary>
	/// 描画
	///< summary>
	void Draw(ViewProjection& viewProjection);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

private:
	//ワールドデータ変換
	WorldTransform worldTranshorm_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//弾
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	
};