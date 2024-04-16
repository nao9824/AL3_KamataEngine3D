#pragma once
#include "Model.h"
#include "WorldTransform.h"

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
	/// 更新
	///< summary>
	void Update();

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
};