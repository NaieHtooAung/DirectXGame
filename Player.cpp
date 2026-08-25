#include "Player.h"

void Player::Initialize() {
	// 後で専用モデルに差し替える場所
	// model_ = Model::CreateFromOBJ("player", true);
	model_ = Model::CreateFromOBJ("cube", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void Player::Update() {
	Input* input = Input::GetInstance();

	// ---- 移動（プレイヤーアクション：XZ平面上を移動） ----
	if (input->PushKey(DIK_LEFT)) {
		worldTransform_.translation_.x -= speed_;
	}
	if (input->PushKey(DIK_RIGHT)) {
		worldTransform_.translation_.x += speed_;
	}
	if (input->PushKey(DIK_UP)) {
		worldTransform_.translation_.z += speed_;
	}
	if (input->PushKey(DIK_DOWN)) {
		worldTransform_.translation_.z -= speed_;
	}

	// 移動範囲を制限（clampを使わず手動で）
	if (worldTransform_.translation_.x < -10.0f) {
		worldTransform_.translation_.x = -10.0f;
	}
	if (worldTransform_.translation_.x > 10.0f) {
		worldTransform_.translation_.x = 10.0f;
	}

	// ---- 攻撃（プレイヤーアクション） ----
	if (input->TriggerKey(DIK_Z) && attackTimer_ <= 0) {
		attackTimer_ = 15;
	}
	if (attackTimer_ > 0) {
		attackTimer_--;
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera); }