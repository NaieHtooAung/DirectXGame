#include "HitEffect.h"
#include "mathUti.h"
#include <cassert>

using namespace KamataEngine;

Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

// メルセンヌ・ツイスターエンジン(64bit版)の共通初期化
std::mt19937_64 HitEffect::randomEngine_ = std::mt19937_64(std::random_device{}());

// インスタンス生成と初期化
HitEffect* HitEffect::Create(uint32_t textureHandle, const Vector3& position) {
	// インスタンス生成
	HitEffect* instance = new HitEffect();
	// newの失敗を検出
	assert(instance);
	// インスタンスの初期化
	instance->Initialize(textureHandle, position);
	// 初期化したインスタンスを返す
	return instance;
}

void HitEffect::Initialize(uint32_t textureHandle, const Vector3& position) {
	textureHandle_ = textureHandle;
	timer_ = 0.0f;
	isFinished_ = false;

	const float kPi = 3.14159265359f;

	// 指定範囲の乱数生成器(浮動小数点数用)
	std::uniform_real_distribution<float> rotationDistribution(-kPi, kPi);

	// 楕円エフェクト
	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.scale_ = {kEllipseWidth, kEllipseLength, 1.0f};
		worldTransform.rotation_ = {0.0f, 0.0f, rotationDistribution(randomEngine_)};
		worldTransform.translation_ = position; // 楕円エフェクトのトランスレーションを発生座標で初期化

		worldTransform.Initialize();
	}
}

void HitEffect::Update() {
	if (isFinished_) {
		return;
	}

	timer_ += 1.0f / 60.0f;
	if (timer_ >= duration_) {
		isFinished_ = true;
	}

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
}

void HitEffect::Draw() {
	if (isFinished_ || !model_ || !camera_) {
		return;
	}

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		model_->Draw(worldTransform, *camera_, textureHandle_);
	}
}