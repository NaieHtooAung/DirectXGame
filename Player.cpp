#include "Player.h"
#include "UpdateMatrix.h"
#include <cmath>

void Player::Initialize() {
	// 後で専用モデルに差し替える場所
	// model_ = Model::CreateFromOBJ("player", true);
	model_ = Model::CreateFromOBJ("p1", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	// 後で専用の攻撃モデルに差し替える場所（例：Resources/attack/attack.obj）
	// attackModel_ = Model::CreateFromOBJ("attack", true);
	attackModel_ = Model::CreateFromOBJ("litblade", true);

	attackWorldTransform_.Initialize();
	attackWorldTransform_.scale_ = { 0.6f, 0.6f, 0.6f };
}

void Player::Update() {
	Input* input = Input::GetInstance();

	// ---- 移動（プレイヤーアクション：XZ平面上を移動） ----
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	if (input->PushKey(DIK_LEFT)) {
		move.x -= 1.0f;
	}
	if (input->PushKey(DIK_RIGHT)) {
		move.x += 1.0f;
	}
	if (input->PushKey(DIK_UP)) {
		move.z += 1.0f;
	}
	if (input->PushKey(DIK_DOWN)) {
		move.z -= 1.0f;
	}

	if (move.x != 0.0f || move.z != 0.0f) {
		facingDir_ = move; // 攻撃モデルを出す向きとして覚えておく

		// ---- 移動方向にプレイヤーを向かせる ----
		float targetAngle = std::atan2(move.x, move.z) + kModelForwardOffset_;
		worldTransform_.rotation_.y = targetAngle;
	}

	worldTransform_.translation_.x += move.x * speed_;
	worldTransform_.translation_.z += move.z * speed_;

	// 移動範囲を制限（clampを使わず手動で）
	if (worldTransform_.translation_.x < -10.0f) {
		worldTransform_.translation_.x = -10.0f;
	}
	if (worldTransform_.translation_.x > 10.0f) {
		worldTransform_.translation_.x = 10.0f;
	}

	// ---- 攻撃（プレイヤーアクション） ----
	if (input->TriggerKey(DIK_Z) && attackTimer_ <= 0) {
		attackTimer_ = kAttackDuration_;
	}
	if (attackTimer_ > 0) {
		attackTimer_--;
	}

	// ---- 攻撃モデルの位置・向き・見た目を更新（攻撃中だけ前に出す）----
	Vector3 attackPos = worldTransform_.translation_;
	attackPos.x += facingDir_.x * 1.5f;
	attackPos.z += facingDir_.z * 1.5f;
	attackWorldTransform_.translation_ = attackPos;

	// ---- 攻撃モデルもプレイヤーと同じ向きに合わせて回転させる ----
	float attackAngle = std::atan2(facingDir_.x, facingDir_.z) + kAttackModelForwardOffset_;
	attackWorldTransform_.rotation_.y = attackAngle;

	// 攻撃の残り時間に合わせて大きさを変化させ、振っているように見せる
	float t = float(attackTimer_) / float(kAttackDuration_);
	float scale = 0.4f + t * 0.6f;
	attackWorldTransform_.scale_ = { scale, scale, scale };
	attackWorldTransform_.matWorld_ =
		MakeAffineMatrix(attackWorldTransform_.scale_, attackWorldTransform_.rotation_, attackWorldTransform_.translation_);
	attackWorldTransform_.TransferMatrix();

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw(const Camera& camera) {
	model_->Draw(worldTransform_, camera);

	// ---- 攻撃中だけ攻撃モデルを表示 ----
	if (IsAttacking()) {
		attackModel_->Draw(attackWorldTransform_, camera);
	}
}

void Player::Reset() {
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.rotation_ = { 0.0f, 0.0f, 0.0f };
	hp_ = 3;
	attackTimer_ = 0;
	facingDir_ = { 0.0f, 0.0f, 1.0f };
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}