#include "Enemy.h"

void Enemy::Initialize(const Vector3& basePosition, float range, float dir) {
	basePosition_ = basePosition;
	range_ = range;
	dir_ = dir;

	// 後で敵モデルに差し替える場所
	// model_ = Model::CreateFromOBJ("enemy", true);
	model_ = Model::CreateFromOBJ("cube", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = basePosition_;
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void Enemy::Update() {
	if (!isAlive_) {
		return;
	}

	// ---- 敵の挙動：範囲内をXZ平面上で往復 ----
	worldTransform_.translation_.x += dir_ * 0.05f;
	if (worldTransform_.translation_.x > basePosition_.x + range_ || worldTransform_.translation_.x < basePosition_.x - range_) {
		dir_ *= -1.0f;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const Camera& camera) {
	if (isAlive_) {
		model_->Draw(worldTransform_, camera);
	}
}