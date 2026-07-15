#define NOMINMAX
#include "ShieldEnemy.h"
#include "MapChipField.h"
#include "Player.h"
#include "mathUti.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <numbers>

ShieldEnemy::ShieldEnemy() {}

void ShieldEnemy::Initialize(KamataEngine::Model* model, uint32_t textureHandleEnemy, KamataEngine::Camera* camera, Vector3& position) {
	walkTimer_ = 0.0f;

	assert(camera);
	camera_ = camera;
	assert(model);
	enemyModel_ = model;
	textureHandleEnemy_ = textureHandleEnemy;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f}; // 実際の値はUpdateWalkが毎フレーム計算する
	facingBaseAngleY_ = -1.5f;
	velocity_ = {-kWalkSpeed, 0, 0};
	facingRight_ = false; // 初期速度が負(=左向き)なのでrotation_.yの-1.5fと対応させる
	behavior_ = Behavior::kWalk;
}

void ShieldEnemy::update() {
	switch (behavior_) {
	case Behavior::kWalk:
		UpdateWalk();
		break;
	case Behavior::kDefeat:
		UpdateDefeat();
		break;
	}

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void ShieldEnemy::UpdateWalk() {
	walkTimer_ += 1.0f / 60.0f;
	worldTransform_.translation_.x += velocity_.x;

	
	float angularSpeed = 2.0f * std::numbers::pi_v<float> / kWalkAnimationPeriod;
	float param = std::sin(walkTimer_ * angularSpeed);
	float degree = (param + 1.0f) / 2.0f;
	float startAngle = +kMaxRockAngle;
	float endAngle = -kMaxRockAngle;
	float lerpedDegree = startAngle + (endAngle - startAngle) * degree;
	float wagOffset = lerpedDegree * (std::numbers::pi_v<float> / 180.0f);

	worldTransform_.rotation_.x = 0.0f;
	worldTransform_.rotation_.z = 0.0f;
	worldTransform_.rotation_.y = facingBaseAngleY_ + wagOffset;

	onGround_ = false;

	velocity_.y -= kGravityAcceleration;
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	CollisionMapInfo info;
	info.velocityAfterCollision = velocity_;

	CollisionMap(info);
	CollisionDetected(info);
	ApplyCollisionResult(info);
}

void ShieldEnemy::UpdateDefeat() {
	defeatTimer_ += 1.0f / 60.0f;

	// デス演出: その場でくるくる回転するだけ(移動・重力・当たり判定はしない)
	worldTransform_.rotation_.x += kDefeatSpinSpeed * (1.0f / 60.0f);
}

void ShieldEnemy::draw() { enemyModel_->Draw(worldTransform_, *camera_, textureHandleEnemy_); }

void ShieldEnemy::onCollision(Player* player) {
	// 既にデス演出中なら何もしない(連続ヒットで再トリガーさせない)
	if (behavior_ == Behavior::kDefeat) {
		return;
	}

	// プレイヤーが攻撃中でなければ何も起きない
	if (!player->IsAttacking()) {
		return;
	}

	// 前方であればガード成功
	// 自キャラが右向き かつ 敵が左向き、または 自キャラが左向き かつ 敵が右向き
	bool isFacingEachOther = (player->GetLRDirection() == Player::LRDirection::kRight && !facingRight_) || (player->GetLRDirection() == Player::LRDirection::kLeft && facingRight_);

	if (isFacingEachOther) {
		// ガード成功: デスを回避する。専用のアニメーションは付けず、
		// 見た目はそのまま歩行状態を継続させる(GuardEffectとノックバックだけで表現する)。
		guardEffectRequested_ = true;

		// プレイヤーのノックバックを要求する
		player->RequestKnockback();

		// 早期returnで敵のデスを回避する
		return;
	}

	// 背面・側面からの攻撃はガードできないので、敵の振るまいをデス演出に変更
	OnDefeated();
}

bool ShieldEnemy::ConsumeGuardEffectRequest() {
	bool requested = guardEffectRequested_;
	guardEffectRequested_ = false;
	return requested;
}

bool ShieldEnemy::ConsumeHitEffectRequest() {
	bool requested = hitEffectRequested_;
	hitEffectRequested_ = false;
	return requested;
}

void ShieldEnemy::OnDefeated() {
	if (behavior_ == Behavior::kDefeat) {
		return; // already dying, ignore repeated hits
	}
	behavior_ = Behavior::kDefeat;
	defeatTimer_ = 0.0f;
	velocity_ = {0.0f, 0.0f, 0.0f};

	// 実際に撃破された瞬間なので、通常の撃破エフェクト(HitEffect)を要求する。
	hitEffectRequested_ = true;
}

// ---- 以下、当たり判定まわりはEnemyクラスから最低限の書き換え(クラス名のみ変更) ----

void ShieldEnemy::CollisionMap(CollisionMapInfo& info) {
	CollisionMapTop(info);
	CollisionMapBottom(info);
	CollisionMapRight(info);
	CollisionMapLeft(info);
}

void ShieldEnemy::CollisionMapBottom(CollisionMapInfo& info) {
	if (info.velocityAfterCollision.y >= 0.0f)
		return;

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

void ShieldEnemy::CollisionMapTop(CollisionMapInfo& info) {
	if (info.velocityAfterCollision.y <= 0)
		return;

	Vector3 moved = {worldTransform_.translation_.x, worldTransform_.translation_.y + info.velocityAfterCollision.y, worldTransform_.translation_.z};

	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++)
		positionNew[i] = CornerPosition(moved, static_cast<Corner>(i));

	bool hit = false;
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.y = rect.bottom - kHeight / 2.0f;
		info.velocityAfterCollision.y = 0.0f;
		info.isHitUp = true;
	}
}

Vector3 ShieldEnemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}

ShieldEnemy::AABB ShieldEnemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z};
	return aabb;
}

void ShieldEnemy::CollisionMapRight(CollisionMapInfo& info) {
	if (info.velocityAfterCollision.x <= 0)
		return;

	Vector3 moved = {worldTransform_.translation_.x + info.velocityAfterCollision.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++)
		positionNew[i] = CornerPosition(moved, static_cast<Corner>(i));

	bool hit = false;
	IndexSet indexSet;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.x = rect.left - kWidth / 2.0f;
		info.velocityAfterCollision.x = 0.0f;
		info.isHitRight = true;
	}
}

void ShieldEnemy::CollisionMapLeft(CollisionMapInfo& info) {
	if (info.velocityAfterCollision.x >= 0)
		return;

	Vector3 moved = {worldTransform_.translation_.x + info.velocityAfterCollision.x, worldTransform_.translation_.y, worldTransform_.translation_.z};

	std::array<Vector3, kNumCorner> positionNew;
	for (uint32_t i = 0; i < positionNew.size(); i++)
		positionNew[i] = CornerPosition(moved, static_cast<Corner>(i));

	bool hit = false;
	IndexSet indexSetTop, indexSetBottom;

	indexSetTop = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSetTop.xIndex, indexSetTop.yIndex) == MapChipType::kBlock)
		hit = true;

	indexSetBottom = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	if (mapChipField_->GetmapChipTypeByIndex(indexSetBottom.xIndex, indexSetBottom.yIndex) == MapChipType::kBlock)
		hit = true;

	if (hit) {
		IndexSet indexSet = indexSetTop;
		if (mapChipField_->GetmapChipTypeByIndex(indexSetBottom.xIndex, indexSetBottom.yIndex) == MapChipType::kBlock)
			indexSet = indexSetBottom;

		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		worldTransform_.translation_.x = rect.right + kWidth / 2.0f - 0.001f;
		info.velocityAfterCollision.x = 0.0f;
		info.isHitLeft = true;
	}
}

Vector3 ShieldEnemy::CornerPosition(const Vector3& center, Corner corner) {
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

void ShieldEnemy::CollisionDetected(const CollisionMapInfo& info) {
	if (!info.isHitDown && !info.isHitUp)
		worldTransform_.translation_.y += info.velocityAfterCollision.y;
	if (!info.isHitLeft && !info.isHitRight)
		worldTransform_.translation_.x += info.velocityAfterCollision.x;
	worldTransform_.translation_.z += info.velocityAfterCollision.z;
}

void ShieldEnemy::ApplyCollisionResult(const CollisionMapInfo& info) {
	if (info.isHitUp)
		velocity_.y = 0.0f;
	if (info.isHitDown) {
		onGround_ = true;
		velocity_.y = 0.0f;
	}
	// 壁にぶつかったら反転する
	if (info.isHitLeft) {
		velocity_.x = +kWalkSpeed;
		worldTransform_.rotation_.y = +1.5f;
		facingRight_ = true;
	}
	if (info.isHitRight) {
		velocity_.x = -kWalkSpeed;
		worldTransform_.rotation_.y = -1.5f;
		facingRight_ = false;
	}
}