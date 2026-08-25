#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// 敵やギミックの挙動を担当するクラス（3D版：左右に往復するだけの簡易版）
// ---------------------------------------------
class Enemy {
public:
	void Initialize(const Vector3& basePosition, float range, float dir);
	void Update();
	void Draw(const Camera& camera);

	Vector3 GetPosition() const { return worldTransform_.translation_; }
	bool IsAlive() const { return isAlive_; }
	void Kill() { isAlive_ = false; }
	void PushBack() { worldTransform_.translation_ = basePosition_; }

private:
	// 後で敵モデルに差し替える場所（Resources/enemy/enemy.obj）
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Vector3 basePosition_{};
	float range_ = 3.0f;
	float dir_ = 1.0f;
	bool isAlive_ = true;
};