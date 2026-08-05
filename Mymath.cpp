#include "MyMath.h"
#include <cmath>

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result{};
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			float sum = 0.0f;
			for (int k = 0; k < 4; ++k) {
				sum += a.m[row][k] * b.m[k][col];
			}
			result.m[row][col] = sum;
		}
	}
	return result;
}

Matrix4x4 MakeScaleMatrixMy(const Vector3& scale) {
	Matrix4x4 m{};
	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;
	m.m[3][3] = 1.0f;
	return m;
}

Matrix4x4 MakeRotateXMatrixMy(float radian) {
	Matrix4x4 m{};
	float s = std::sin(radian);
	float c = std::cos(radian);
	m.m[0][0] = 1.0f;
	m.m[1][1] = c;
	m.m[1][2] = s;
	m.m[2][1] = -s;
	m.m[2][2] = c;
	m.m[3][3] = 1.0f;
	return m;
}

Matrix4x4 MakeRotateYMatrixMy(float radian) {
	Matrix4x4 m{};
	float s = std::sin(radian);
	float c = std::cos(radian);
	m.m[0][0] = c;
	m.m[0][2] = -s;
	m.m[1][1] = 1.0f;
	m.m[2][0] = s;
	m.m[2][2] = c;
	m.m[3][3] = 1.0f;
	return m;
}

Matrix4x4 MakeRotateZMatrixMy(float radian) {
	Matrix4x4 m{};
	float s = std::sin(radian);
	float c = std::cos(radian);
	m.m[0][0] = c;
	m.m[0][1] = s;
	m.m[1][0] = -s;
	m.m[1][1] = c;
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
	return m;
}

Matrix4x4 MakeTranslateMatrixMy(const Vector3& translate) {
	Matrix4x4 m{};
	m.m[0][0] = 1.0f;
	m.m[1][1] = 1.0f;
	m.m[2][2] = 1.0f;
	m.m[3][3] = 1.0f;
	m.m[3][0] = translate.x;
	m.m[3][1] = translate.y;
	m.m[3][2] = translate.z;
	return m;
}

Matrix4x4 MakeAffineMatrixMy(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
	Matrix4x4 scaleMatrix = MakeScaleMatrixMy(scale);

	Matrix4x4 rotateX = MakeRotateXMatrixMy(rotation.x);
	Matrix4x4 rotateY = MakeRotateYMatrixMy(rotation.y);
	Matrix4x4 rotateZ = MakeRotateZMatrixMy(rotation.z);
	Matrix4x4 rotateMatrix = Multiply(rotateX, Multiply(rotateY, rotateZ));

	Matrix4x4 translateMatrix = MakeTranslateMatrixMy(translation);

	// スケール → 回転 → 平行移動 の順で合成
	Matrix4x4 result = Multiply(scaleMatrix, Multiply(rotateMatrix, translateMatrix));
	return result;
}