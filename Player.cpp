#include "Player.h"
#include <cassert>
void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) {

	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	textureHandle_ = textureHandle;
	camera_ = camera;

}

void Player::Update() {

	
	worldTransform_.TransferMatrix();

}

void Player::Draw() {

		model_->Draw(worldTransform_, *camera_, textureHandle_);

}

Player::~Player() {}
