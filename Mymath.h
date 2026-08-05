#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// 4x4行列同士の掛け算 (a * b)
Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrixMy(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRotateXMatrixMy(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrixMy(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrixMy(float radian);

// 平行移動行列
Matrix4x4 MakeTranslateMatrixMy(const Vector3& translate);

// スケール・回転(XYZオイラー角)・平行移動からアフィン変換行列を作る
// エンジンにMakeAffineMatrixが無い場合はこちらを使う
Matrix4x4 MakeAffineMatrixMy(const Vector3& scale, const Vector3& rotation, const Vector3& translation);