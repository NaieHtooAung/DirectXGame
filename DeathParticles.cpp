#include "DeathParticles.h"
#include "mathUti.h"

DeathParticles::DeathParticles() {}

void DeathParticles::Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera, Vector3& position) {
	model_ = model;
	textureHandle_ = textureHandlePlayer;
	camera_ = camera;

	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
	objectColor_.SetColor(color_);

	const float speed = 0.1f;
	const float angles[] = {0, 45, 90, 135, 180, 225, 270, 315};

	for (int i = 0; i < kNumParticles; i++) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = position;
		worldTransforms_[i].scale_ = {1.0f, 1.0f, 1.0f};
		float rad = angles[i] * (3.14159f / 180.0f);
		velocities_[i] = {std::cos(rad) * speed, std::sin(rad) * speed, 0.0f};
	}

	isFinished_ = false;
	timer_ = 0.0f;
	isInitialized_ = true;
}

void DeathParticles::Update() {
	if (!isInitialized_)
		return;
	if (isFinished_)
		return;

	timer_ += 1.0f / 60.0f;
	float progress = timer_ / kDuration;

	// fade alpha over time
	color_.w = 1.0f - progress;
	objectColor_.SetColor(color_);

	for (int i = 0; i < kNumParticles; i++) {
		worldTransforms_[i].translation_.x += velocities_[i].x;
		worldTransforms_[i].translation_.y += velocities_[i].y;
		worldTransforms_[i].matWorld_ = MakeAffineMatrix(worldTransforms_[i].scale_, worldTransforms_[i].rotation_, worldTransforms_[i].translation_);
		worldTransforms_[i].TransferMatrix();
	}

	if (timer_ >= kDuration) {
		isFinished_ = true;
	}
}

void DeathParticles::Draw() {
	if (!isInitialized_)
		return;
	if (isFinished_)
		return;

	for (int i = 0; i < kNumParticles; i++) {
		model_->Draw(worldTransforms_[i], *camera_, textureHandle_, &objectColor_);
	}
}