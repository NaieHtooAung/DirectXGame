#pragma once
#include "KamataEngine.h"
#include <array>
#include <cmath>
using namespace KamataEngine;

class DeathParticles {
public:
	static inline const int kNumParticles = 8;
	static inline const float kDuration = 1.5f;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	DeathParticles();
	void Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera, Vector3& position);
	void Update();
	void Draw();
	bool IsFinished() const { return isFinished_; }
	bool isInitialized_ = false;

private:
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	KamataEngine::Camera* camera_ = nullptr;

	Vector3 velocities_[kNumParticles];
	ObjectColor objectColor_;
	Vector4 color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	float timer_ = 0.0f;
	bool isFinished_ = false;
};