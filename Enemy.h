#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
using namespace KamataEngine;
class Player;
class Enemy {
public:
	float walkTimer_ = 0.0f;
	static inline const float kWalkAnimationPeriod = 1.0f; // seconds per cycle
	static inline const float kMaxRockAngle = 15.0f;       // degrees of rock

	Enemy();
	struct AABB {
		Vector3 min;
		Vector3 max;
	};

	AABB GetAABB();
	Vector3 GetWorldPosition();

	void Initialize(KamataEngine::Model* model, uint32_t textureHandleEnemy, KamataEngine::Camera* camera, Vector3& position);
	void update();
	void draw();
	void setMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void onCollision(const Player* player);

	// Call when the player's attack defeats this enemy. Starts the
	// spin-and-shrink death animation; safe to call more than once.
	void OnDefeated();
	// True the instant OnDefeated() is called (enemy should stop acting as a threat).
	bool IsDefeated() const { return isDefeated_; }
	// True once the death animation has fully played out — GameScene should
	// delete and remove this enemy when this returns true.
	bool IsDefeatAnimationFinished() const { return isDefeated_ && defeatTimer_ >= kDefeatTime; }

private:
	// Collision corners
	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	struct CollisionMapInfo {
		bool isHitDown = false;
		bool isHitUp = false;
		bool isHitLeft = false;
		bool isHitRight = false;
		Vector3 velocityAfterCollision = {};
	};

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void CollisionMap(CollisionMapInfo& info);
	void CollisionMapTop(CollisionMapInfo& info);
	void CollisionMapBottom(CollisionMapInfo& info);
	void CollisionMapRight(CollisionMapInfo& info);
	void CollisionMapLeft(CollisionMapInfo& info);
	void CollisionDetected(const CollisionMapInfo& info);
	void ApplyCollisionResult(const CollisionMapInfo& info);

	MapChipField* mapChipField_ = nullptr;
	WorldTransform worldTransform_;
	Model* enemyModel_ = nullptr;
	uint32_t textureHandleEnemy_ = 0;
	Camera* camera_ = nullptr;

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	bool onGround_ = false;

	// Defeat / death animation state
	bool isDefeated_ = false;
	float defeatTimer_ = 0.0f;
	static inline const float kDefeatTime = 0.5f;       // seconds spent spinning before it vanishes
	static inline const float kDefeatSpinSpeed = 25.0f; // radians/sec while dying (same axis as the walk rock)

	static inline const float kGravityAcceleration = 0.05f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;
	static inline const float kWalkSpeed = 0.02f;
	static inline const float kWalkMotionAngleStart = 1.0f;
	static inline const float kWalkMotionAngleEnd = 2.0f;
	static inline const float kWalkMotionTime = 0.5f;
};