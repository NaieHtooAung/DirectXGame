#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class Player {
public:
	enum class LRDirection {
		kRight,kLeft,
	};
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	bool onground_ = true;
	LRDirection lrDirection_ = LRDirection::kRight;
	Vector3 velocity_ = {0,0,0};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;
	static inline const float KTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.01f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.2f;
	const WorldTransform& GetWorldTransform() const;
	const Vector3& GetVelocity() const { return velocity_; }
	void Initialize(KamataEngine::Model* model, uint32_t textureHandlePlayer, KamataEngine::Camera* camera, Vector3& position);

	void Update();

	void Draw();

	~Player();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandlePlayer_ = 0;

	Camera* camera_ = nullptr;
};