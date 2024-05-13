#pragma once
#include "WorldTransform.h"

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
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
