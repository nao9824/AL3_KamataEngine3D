#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

// ベクトルとベクトルの足し算
Vector3 Add(Vector3 v1, Vector3 v2);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
// 積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

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