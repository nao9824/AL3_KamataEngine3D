#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "mathFunction.h"
#include "Sprite.h"
#include "ViewProjection.h"

///<summary>
///自キャラ
///<summary>
class Player {
public:
	///< summary>
	/// 初期化
	///< summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	///< summary>
	/// デストラクタ
	///< summary>
	~Player();

	///< summary>
	/// 更新
	///< summary>
	void Update(const ViewProjection& viewProjection);
	void Rotate();

	///< summary>
	/// 攻撃
	///< summary>
	void Attack();

	void SpecialAttack();

	///< summary>
	/// 描画
	///< summary>
	void Draw(ViewProjection& viewProjection);

	void DrawUI();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	bool IsDead() const { return isDead_; }

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	Vector3 GetWorldPosition3D();

	//親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

private:
	//ワールドデータ変換
	WorldTransform worldTransform_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	uint32_t textureReticle = 0u;
	Sprite* sprite_ = nullptr;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	//キーボード入力
	Input* input_ = nullptr;
	int32_t hp_;
	// デスフラグ
	bool isDead_ = false;
	//弾
	PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	//Vector3 playerPosition_ = {0.0f, 0.0f, 50.0f};
	int32_t bulletNum_;
};