#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

class RaillCamera {
public:
	void Initialize(const Vector3& position, const Vector3& rotate);

	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	// ワールドデータ変換
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// 速度
	Vector3 velocity_;
	//回転
	Vector3 rotate_;
};