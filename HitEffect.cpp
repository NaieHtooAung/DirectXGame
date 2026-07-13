#include "HitEffect.h"
#include "mathUti.h"
#include <cassert>
#include <cmath>

using namespace KamataEngine;

Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

// メルセンヌ・ツイスターエンジン(64bit版)の共通初期化
std::mt19937_64 HitEffect::randomEngine_ = std::mt19937_64(std::random_device{}());

namespace {

// 0.0f~1.0fにクランプする(windows.hのmin/maxマクロと衝突するのでstd::minは使わない)
float Clamp01(float t) {
	if (t < 0.0f) {
		return 0.0f;
	}
	if (t > 1.0f) {
		return 1.0f;
	}
	return t;
}

// 線形補間
float Lerp(float a, float b, float t) { return a + (b - a) * t; }

// イーズアウト(急激に始まり緩やかに終わる) : スプレッドの拡大に使用
float EaseOutQuad(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }

// イーズイン(緩やかに始まり急激に終わる) : フェードアウトに使用
float EaseInQuad(float t) { return t * t; }

} // namespace

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

	const float kPi = 3.14159265359f;

	// 指定範囲の乱数生成器(浮動小数点数用)
	std::uniform_real_distribution<float> rotationDistribution(-kPi, kPi);

	// 楕円エフェクト
	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		// スプレッド開始時点は縮小した状態から始める
		worldTransform.scale_ = {kEllipseWidth * kSpreadStartScaleRate, kEllipseLength * kSpreadStartScaleRate, 1.0f};
		worldTransform.rotation_ = {0.0f, 0.0f, rotationDistribution(randomEngine_)};
		worldTransform.translation_ = position; // 楕円エフェクトのトランスレーションを発生座標で初期化

		worldTransform.Initialize();
	}

	alpha_ = 1.0f;

	// 状態遷移: まずはスプレッドから開始する
	ChangeState(State::kSpread);
}

void HitEffect::ChangeState(State newState) {
	state_ = newState;
	counter_ = 0.0f;
}

void HitEffect::Update() {
	// デス状態は何もしない(削除待ちのため)
	if (state_ == State::kDead) {
		return;
	}

	// カウンタの加算
	counter_ += kDeltaTime;

	// 状態切り替え機構
	switch (state_) {
	case State::kSpread:
		UpdateSpread();
		break;
	case State::kFade:
		UpdateFade();
		break;
	case State::kDead:
		break;
	}

	// ワールド行列の更新
	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}
}

void HitEffect::UpdateSpread() {
	// スプレッド中はスケーリングを変化(イージング)
	float t = Clamp01(counter_ / kSpreadDuration);
	float eased = EaseOutQuad(t);
	float scaleRate = Lerp(kSpreadStartScaleRate, 1.0f, eased);

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.scale_ = {kEllipseWidth * scaleRate, kEllipseLength * scaleRate, 1.0f};
	}

	// 継続時間に達したらフェードへ遷移
	if (counter_ >= kSpreadDuration) {
		ChangeState(State::kFade);
	}
}

void HitEffect::UpdateFade() {
	// フェード中はアルファ値を変化(イージング)
	float t = Clamp01(counter_ / kFadeDuration);
	float eased = EaseInQuad(t);
	alpha_ = Lerp(1.0f, 0.0f, eased);

	// 継続時間に達したらインスタンスごと消滅(デス状態)へ遷移
	if (counter_ >= kFadeDuration) {
		alpha_ = 0.0f;
		ChangeState(State::kDead);
	}
}

void HitEffect::Draw() {
	if (state_ == State::kDead || !model_ || !camera_) {
		return;
	}

	// フェード中のアルファ値をモデルに反映
	// TODO: お使いのModelクラスにあるアルファ/カラー設定用のAPIに置き換えてください。
	// 例: model_->SetColor({1.0f, 1.0f, 1.0f, alpha_});

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		model_->Draw(worldTransform, *camera_, textureHandle_);
	}
}