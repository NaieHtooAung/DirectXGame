#include "DeathParticles.h"
#include "mathUti.h"
DeathParticles::DeathParticles() {}
void DeathParticles::Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera, Vector3& position) {
	model_ = model;
	textureHandle_ = textureHandlePlayer;
	camera_ = camera;

	// 8 particles spread in different directions
	const float speed = 0.1f;
	const float angles[] = {0, 45, 90, 135, 180, 225, 270, 315};

	for (int i = 0; i < kNumParticles; i++) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].translation_ = position;
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

	// 0.0 to 1.0 progress
	float progress = timer_ / kDuration;

	for (int i = 0; i < kNumParticles; i++) {
		worldTransforms_[i].translation_.x += velocities_[i].x;
		worldTransforms_[i].translation_.y += velocities_[i].y;

		// shrink scale over time
		float scale = 1.0f - progress;
		worldTransforms_[i].scale_ = {scale, scale, scale};

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
		model_->Draw(worldTransforms_[i], *camera_, textureHandle_);
	}
}