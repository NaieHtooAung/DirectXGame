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
	static const int32_t kAttackDuration_ = 15;

	// 攻撃モデルを出す向き（最後に動いた方向）
	Vector3 facingDir_ = { 0.0f, 0.0f, 1.0f };

	// モデルの正面とテクスチャの「顔」が描かれた面がズレている場合の補正角（ラジアン）
	// 動かしても顔が変な方向を向くなら、この値を 0 → 1.5708f(90°) → 3.14159f(180°) → -1.5708f(-90°) の順で試す
	static constexpr float kModelForwardOffset_ = 0.0f;

	// litblade（攻撃モデル）の正面ズレ補正角（プレイヤーとは別に調整する）
	static constexpr float kAttackModelForwardOffset_ = 0.0f;
};