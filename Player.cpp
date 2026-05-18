#define NOMINMAX
#include <Windows.h>
#include "Player.h"
#include "mathUti.h"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <numbers>

using namespace KamataEngine;
void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera, Vector3& position) {

	assert(model);

	model_ = model;

	textureHandlePlayer_ = textureHandlePlayer;

	camera_ = camera;

	worldTransform_.Initialize();

	// player start position
	worldTransform_.translation_ = position;

	// player size
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// player rotation
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}
void Player::Update() {

	if (onground_) {
		
	if (Input::GetInstance()->PushKey(DIK_D)||Input::GetInstance()->PushKey(DIK_A)) {

		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_D)) {

			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
			}
			// record current facing
			turnFirstRotationY_ = worldTransform_.rotation_.y;

			turnTimer_ = 1.0f / 60.0f;

			acceleration.x += kAcceleration;
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			

		} else if (Input::GetInstance()->PushKey(DIK_A)) {
		
				if (lrDirection_ != LRDirection::kLeft) {

				lrDirection_ = LRDirection::kLeft;
			}
			    // record current facing
			    turnFirstRotationY_ = worldTransform_.rotation_.y;
				
		turnTimer_ = 1.0f / 60.0f;

			acceleration.x -= kAcceleration;
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
		
		}
		velocity_.x += acceleration.x;
		
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		if (turnTimer_ < KTimeTurn) {
			
			// update timer
			turnTimer_ += 1.0f / 60.0f;
			// rotation animation

			float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		

			float t = turnTimer_ / KTimeTurn;

			t = std::clamp(t, 0.0f, 1.0f);

			// ease in out
			t = t * t * (3.0f - 2.0f * t);

			// lerp rotation
			worldTransform_.rotation_.y = (1.0f - t) * turnFirstRotationY_ + t * destinationRotationY;
		}
		

	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}
	} else {
		velocity_.y -= kGravityAcceleration;
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
	if (Input::GetInstance()->PushKey(DIK_SPACE)){
		
		velocity_ = Vector3(0, +kJumpAcceleration, 0);

	}
	worldTransform_.translation_.x = worldTransform_.translation_.x + velocity_.x;
	worldTransform_.translation_.y = worldTransform_.translation_.y + velocity_.y;

	
		if (worldTransform_.translation_.y <= 1.0f) {

		worldTransform_.translation_.y = 1.0f;

		velocity_.y = 0.0f;

		onground_ = true;
	} else {

		onground_ = false;
	}
	// affine matrix
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// send to GPU
	worldTransform_.TransferMatrix();
}
void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandlePlayer_); }
Player::~Player() {}