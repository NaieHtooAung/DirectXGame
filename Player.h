#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// プレイヤーアクション（移動・攻撃）を担当するクラス（3D版）
// ---------------------------------------------
class Player {
public:
	void Initialize();
	void Update();
	void Draw(const Camera& camera);
	void Reset();

	Vector3 GetPosition() const { return worldTransform_.translation_; }
	bool IsAttacking() const { return attackTimer_ > 0; }
	int32_t GetHP() const { return hp_; }
	void TakeDamage() { hp_--; }
	bool IsDead() const { return hp_ <= 0; }

private:
	// 後で専用モデルに差し替える場所（Resources/player/player.obj）
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	// 攻撃時に見える攻撃モデル（後でResources/attack/attack.objなどに差し替え）
	Model* attackModel_ = nullptr;
	WorldTransform attackWorldTransform_;

	float speed_ = 0.2f;
	int32_t hp_ = 3;
	int32_t attackTimer_ = 0;
	int32_t textureHandlePlayer_ = 0;
	static const int32_t kAttackDuration_ = 15;

	// 攻撃モデルを出す向き（最後に動いた方向）
	Vector3 facingDir_ = { 0.0f, 0.0f, 1.0f };
};