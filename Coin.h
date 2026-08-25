#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// ゲームシステムの目標（全部集めたらクリア）を担当するクラス（3D版）
// ---------------------------------------------
class Coin {
public:
	void Initialize(const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

	Vector3 GetPosition() const { return worldTransform_.translation_; }
	bool IsGot() const { return isGot_; }
	void Get() { isGot_ = true; }

private:
	// 後でコインモデルに差し替える場所（Resources/coin/coin.obj）
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	bool isGot_ = false;
	float spinTimer_ = 0.0f;
};