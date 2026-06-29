#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "mathUti.h"
#include <Windows.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, Model* modelAttack, uint32_t textureHandlePlayer, uint32_t textureHandleAttack, KamataEngine::Camera* camera, Vector3& position) {

	assert(model);

	model_ = model;

	modelAttack_ = modelAttack;

	textureHandlePlayer_ = textureHandlePlayer;

	textureHandleAttack_ = textureHandleAttack;
	camera_ = camera;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	// Effect model shares the player's position/facing but keeps its own scale.
	worldTransformAttack_.Initialize();
	worldTransformAttack_.translation_ = worldTransform_.translation_;

	if (lrDirection_ == LRDirection::kRight) {
		worldTransformAttack_.translation_.x += 0.8f;
	} else {
		worldTransformAttack_.translation_.x -= 0.8f;
	}
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;
}

void Player::BehaviorRootInitialize() {}

void Player::BehaviorAttackInitialize() {

	attackPhase_ = AttackPhase::kSqueeze;
	squeezeTimer_ = 0.0f;
	dashTimer_ = 0.0f;
	recoveryTimer_ = 0.0f;

	wasRunningBeforeAttack_ = std::abs(velocity_.x) > 0.01f;
}

void Player::BehaviorRootUpdate() {
	if (isDead_)
		return;

	if (onground_) {
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {

			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
				}
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
				turnTimer_ += 1.0f / 60.0f;
				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
				float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				float t = turnTimer_ / KTimeTurn;
				t = std::clamp(t, 0.0f, 1.0f);
				t = t * t * (3.0f - 2.0f * t);
				worldTransform_.rotation_.y = (1.0f - t) * turnFirstRotationY_ + t * destinationRotationY;
			}

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
	}

	// Trigger dash with LSHIFT — allowed in air too
	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackUpdate() {

	// Movement is applied only during the dash phase.
	float velocity = 0.0f;

	switch (attackPhase_) {

	case AttackPhase::kSqueeze:
		AttackSqueezeUpdate();
		break;

	case AttackPhase::kDash:
		AttackDashUpdate();

		if (lrDirection_ == LRDirection::kRight) {
			velocity = kDashSpeed;
		} else {
			velocity = -kDashSpeed;
		}
		break;

	case AttackPhase::kRecovery:
		AttackRecoveryUpdate();
		break;
	}

	velocity_.x = velocity;
}

void Player::AttackSqueezeUpdate() {
	squeezeTimer_ += 1.0f / 60.0f;

	float t = squeezeTimer_ / kSqueezeTime;

	// 予備動作（アンティシペーション）：ダッシュ前に少しタメる
	// Compress forward (z), bulge vertically (y) — winding up before the dash.
	worldTransform_.scale_.z = EaseOut(1.0f, 0.3f, t);
	worldTransform_.scale_.y = EaseOut(1.0f, 1.6f, t);

	if (squeezeTimer_ >= kSqueezeTime) {
		attackPhase_ = AttackPhase::kDash;
		dashTimer_ = 0.0f;
	}
}

void Player::AttackDashUpdate() {

	dashTimer_ += 1.0f / 60.0f;

	float t = dashTimer_ / kDashTime;

	// Stretch during the dash.
	worldTransform_.scale_.z = EaseOut(0.3f, 1.3f, t);
	worldTransform_.scale_.y = EaseOut(1.6f, 0.7f, t);

	if (dashTimer_ >= kDashTime && !Input::GetInstance()->PushKey(DIK_LSHIFT)) {

		attackPhase_ = AttackPhase::kRecovery;
		recoveryTimer_ = 0.0f;
	}
}
void Player::AttackRecoveryUpdate() {

	recoveryTimer_ += 1.0f / 60.0f;

	float t = recoveryTimer_ / kRecoveryTime;

	worldTransform_.scale_.z = EaseOut(1.3f, 1.0f, t);
	worldTransform_.scale_.y = EaseOut(0.7f, 1.0f, t);

	if (recoveryTimer_ >= kRecoveryTime) {

		worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

		if (wasRunningBeforeAttack_) {

			if (lrDirection_ == LRDirection::kRight) {
				velocity_.x = kLimitRunSpeed;
			} else {
				velocity_.x = -kLimitRunSpeed;
			}

		} else {

			velocity_.x = 0.0f;
		}

		behaviorRequest_ = Behavior::kRoot;
	}
}
void Player::Update() {

	// Run the current behavior update first
	switch (behavior_) {
	case Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	default:
		break;
	}

	// Then handle any transition request set during this frame's update
	if (behaviorRequest_ != Behavior::kUnknow) {
		behavior_ = behaviorRequest_;
		behaviorRequest_ = Behavior::kUnknow;

		switch (behavior_) {
		case Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		default:
			break;
		}
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && onground_) {
		velocity_.y = kJumpAcceleration;
	}

	onground_ = false;

	velocity_.y -= kGravityAcceleration;
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	CollisionMapInfo collisionmapInfo;
	collisionmapInfo.velocityAfterCollision.x = velocity_.x;
	collisionmapInfo.velocityAfterCollision.y = velocity_.y;
	collisionmapInfo.velocityAfterCollision.z = velocity_.z;

	CollisionMap(collisionmapInfo);

	CollisionDeceted(collisionmapInfo);

	TopCollision(collisionmapInfo);

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// トランスフォームの値をコピー
	// Keep the effect model following the player's position/facing
	// (its own scale_ is left alone, independent of the squash-stretch above).
	worldTransformAttack_.translation_ = worldTransform_.translation_;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;

	worldTransformAttack_.matWorld_ = MakeAffineMatrix(worldTransformAttack_.scale_, worldTransformAttack_.rotation_, worldTransformAttack_.translation_);

	worldTransformAttack_.TransferMatrix();
}

Player::AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f + kBlank, worldPos.y - kHeight / 2.0f + kBlank, worldPos.z};
	aabb.max = {worldPos.x + kWidth / 2.0f - kBlank, worldPos.y + kHeight / 2.0f - kBlank, worldPos.z};
	return aabb;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

const WorldTransform& Player::GetWorldTransform() const { return worldTransform_; }

void Player::Draw() {

	if (isDead_) {
		return;
	}

	model_->Draw(worldTransform_, *camera_, textureHandlePlayer_);

	if (behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kDash && modelAttack_) {

		modelAttack_->Draw(worldTransformAttack_, *camera_, textureHandleAttack_);
	}
}

void Player::onCollision(const Enemy* enemy) {
	(void)enemy;

	// While dashing, the player is invincible and simply passes through enemies.
	// NOTE: the enemy itself is not defeated/killed here yet — that's a later step.
	if (behavior_ == Behavior::kAttack) {
		return;
	}

	isDead_ = true;
}

void Player::CollisionMap(CollisionMapInfo& info) {
	CollisionMapTop(info);
	CollisionMapBottom(info);
	CollisionMapRight(info);
	CollisionMapleft(info);
}

void Player::CollisionMapTop(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionNew;
	Vector3 movedTranslation = {worldTransform_.translation_.x, worldTransform_.translation_.y + info.velocityAfterCollision.y, worldTransform_.translation_.z};

	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(movedTranslation, static_cast<Corner>(i));
	}

	if (info.velocityAfterCollision.y <= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.y = rect.bottom - kHeight / 2.0f;
		info.velocityAfterCollision.y = 0.0f;
		info.isHitUp = true;
	}
}

void Player::CollisionMapBottom(CollisionMapInfo& info) {
	if (info.velocityAfterCollision.y >= 0.0f) {
		return;
	}

	Vector3 moved = {worldTransform_.translation_.x, worldTransform_.translation_.y + info.velocityAfterCollision.y, worldTransform_.translation_.z};

	Vector3 leftBottom = CornerPosition(moved, kLeftBottom);
	Vector3 rightBottom = CornerPosition(moved, kRightBottom);

	bool hit = false;
	IndexSet hitIndex{};

	IndexSet index = mapChipField_->GetMapChipIndexSetByPosition(leftBottom);
	if (mapChipField_->GetmapChipTypeByIndex(index.xIndex, index.yIndex) == MapChipType::kBlock) {
		hit = true;
		hitIndex = index;
	}

	index = mapChipField_->GetMapChipIndexSetByPosition(rightBottom);
	if (mapChipField_->GetmapChipTypeByIndex(index.xIndex, index.yIndex) == MapChipType::kBlock) {
		hit = true;
		hitIndex = index;
	}

	if (hit) {
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(hitIndex.xIndex, hitIndex.yIndex);
		float correctY = rect.top + kHeight / 2.0f;
		info.velocityAfterCollision.y = correctY - worldTransform_.translation_.y;
		info.isHitDown = true;
	}
}

void Player::CollisionMapRight(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionNew;
	Vector3 movedTranslation = {worldTransform_.translation_.x + info.velocityAfterCollision.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(movedTranslation, static_cast<Corner>(i));
	}

	if (info.velocityAfterCollision.x <= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.x = rect.left - kWidth / 2.0f;
		info.velocityAfterCollision.x = 0.0f;
		info.isHitRight = true;
	}
}

void Player::CollisionMapleft(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionNew;
	Vector3 movedTranslation = {worldTransform_.translation_.x + info.velocityAfterCollision.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	for (uint32_t i = 0; i < positionNew.size(); i++) {
		positionNew[i] = CornerPosition(movedTranslation, static_cast<Corner>(i));
	}

	if (info.velocityAfterCollision.x >= 0) {
		return;
	}

	MapChipType mapChipType;
	bool hit = false;
	IndexSet indexSetTop;
	IndexSet indexSetBottom;

	indexSetTop = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSetTop.xIndex, indexSetTop.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	indexSetBottom = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetmapChipTypeByIndex(indexSetBottom.xIndex, indexSetBottom.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		IndexSet indexSet = indexSetTop;
		if (mapChipField_->GetmapChipTypeByIndex(indexSetBottom.xIndex, indexSetBottom.yIndex) == MapChipType::kBlock) {
			indexSet = indexSetBottom;
		}
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.x = rect.right + kWidth / 2.0f - 0.001f;
		info.velocityAfterCollision.x = 0.0f;
		info.isHitLeft = true;
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	const float kAdjustX = 0.03f;
	const float kAdjustY = 0.03f;

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f - kAdjustX, -kHeight / 2.0f + kAdjustY, 0},
	    {-kWidth / 2.0f + kAdjustX, -kHeight / 2.0f + kAdjustY, 0},
	    {+kWidth / 2.0f - kAdjustX, +kHeight / 2.0f - kAdjustY, 0},
	    {-kWidth / 2.0f + kAdjustX, +kHeight / 2.0f - kAdjustY, 0},
	};

	Vector3 offset = offsetTable[static_cast<uint32_t>(corner)];
	return {center.x + offset.x, center.y + offset.y, center.z + offset.z};
}

void Player::CollisionDeceted(const CollisionMapInfo& info) {
	if (!info.isHitDown && !info.isHitUp) {
		worldTransform_.translation_.y += info.velocityAfterCollision.y;
	}
	if (!info.isHitLeft && !info.isHitRight) {
		worldTransform_.translation_.x += info.velocityAfterCollision.x;
	}
	worldTransform_.translation_.z += info.velocityAfterCollision.z;
}

void Player::TopCollision(const CollisionMapInfo& info) {
	if (info.isHitUp) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		velocity_.y = 0.0f;
	}
	if (info.isHitDown) {
		onground_ = true;
		velocity_.y = 0.0f;
	}
	if (info.isHitLeft || info.isHitRight) {
		velocity_.x = 0.0f;
	}
}

Player::~Player() {}