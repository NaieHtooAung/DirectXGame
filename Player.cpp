#include "Player.h"
#include "mathUti.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;
void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera) {

	assert(model);

	model_ = model;

	textureHandlePlayer_ = textureHandlePlayer;

	camera_ = camera;

	worldTransform_.Initialize();

	// player start position
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	// player size
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// player rotation
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
}
void Player::Update() {

	// rotation animation
	worldTransform_.rotation_.y += 0.03f;

	// affine matrix
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// send to GPU
	worldTransform_.TransferMatrix();
}
void Player::Draw() { model_->Draw(worldTransform_, *camera_, textureHandlePlayer_); }
Player::~Player() {}