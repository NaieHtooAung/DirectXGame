#pragma once

#include "KamataEngine.h"
#include "MapChipField.h"
using namespace KamataEngine;

class MapChipField;
class Enemy;
class Player {
public:
	enum class LRDirection {
		kRight,
		kLeft,
	};
	enum Corner {

		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner

	};
	enum class Behavior { kRoot, kAttack, kUnknow };

	// Sub-phases of the dash (kAttack) behavior.
	enum class AttackPhase {
		kSqueeze,  // anticipation squash just before the dash fires
		kDash,     // the actual dash movement (stretched, invincible)
		kRecovery, // easing scale/speed back to normal
	};

	Behavior behavior_ = Behavior::kRoot;
	Behavior behaviorRequest_ = Behavior::kUnknow;

	AttackPhase attackPhase_ = AttackPhase::kSqueeze;

	float dashTimer_ = 0.0f;
	float squeezeTimer_ = 0.0f;
	float recoveryTimer_ = 0.0f;
	static inline const float kDashTime = 0.25f;
	static inline const float kDashSpeed = 0.4f;
	static inline const float kSqueezeTime = 0.1f;
	static inline const float kRecoveryTime = 0.1f;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	bool onground_ = true;
	bool isDead_ = false;
	bool hasAirAttacked_ = false; 
	bool isDead() const { return isDead_; }
	LRDirection lrDirection_ = LRDirection::kRight;
	Vector3 velocity_ = {0, 0, 0};
	Vector3 GetWorldPosition();

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;
	static inline const float KTimeTurn = 0.3f;
	static inline const float kGravityAcceleration = 0.01f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.3f;
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.01f;
	bool wasRunningBeforeAttack_ = false;

	struct AABB {

		Vector3 min;
		Vector3 max;
	};

	AABB GetAABB();

	struct CollisionMapInfo {
		bool isHitLeft = false;
		bool isHitRight = false;
		bool isHitUp = false;
		bool isHitDown = false;

		Vector3 velocityAfterCollision = {0, 0, 0};
	};

	const WorldTransform& GetWorldTransform() const;
	const Vector3& GetVelocity() const { return velocity_; }
	void CollisionDeceted(const CollisionMapInfo& info);
	void TopCollision(const CollisionMapInfo& info);
	void Initialize(Model* model, Model* modelAttack, uint32_t textureHandlePlayer, uint32_t textureHandleAttack, Camera* camera, Vector3& position);

	void BehaviorRootInitialize();

	void BehaviorAttackInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

	// AttackPhase sub-updates, dispatched from BehaviorAttackUpdate via switch.
	void AttackSqueezeUpdate();
	void AttackDashUpdate();
	void AttackRecoveryUpdate();

	void Update();

	void Draw();

	void setMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void CollisionMap(CollisionMapInfo& info);

	void onCollision(const Enemy* enemy);

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	~Player();

private:
	void CollisionMapTop(CollisionMapInfo& info);
	void CollisionMapBottom(CollisionMapInfo& info);
	void CollisionMapRight(CollisionMapInfo& info);
	void CollisionMapleft(CollisionMapInfo& info);
	MapChipField* mapChipField_ = nullptr;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Model* modelAttack_ = nullptr;

	WorldTransform worldTransformAttack_;

	uint32_t textureHandlePlayer_ = 0;

	uint32_t textureHandleAttack_ = 0;

	Camera* camera_ = nullptr;
};