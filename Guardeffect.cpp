#include "GuardEffect.h"
#include "mathUti.h"
#include <cassert>

using namespace KamataEngine;

Model* GuardEffect::model_ = nullptr;
Camera* GuardEffect::camera_ = nullptr;

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

// イーズアウト(急激に始まり緩やかに終わる) : 輪の拡大に使用
float EaseOutQuad(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }

// イーズイン(緩やかに始まり急激に終わる) : フェードアウトに使用
float EaseInQuad(float t) { return t * t; }

} // namespace

// インスタンス生成と初期化
GuardEffect* GuardEffect::Create(uint32_t textureHandle, const Vector3& position) {
	// インスタンス生成
	GuardEffect* instance = new GuardEffect();
	// newの失敗を検出
	assert(instance);
	// インスタンスの初期化
	instance->Initialize(textureHandle, position);
	// 初期化したインスタンスを返す
	return instance;
}

void GuardEffect::Initialize(uint32_t textureHandle, const Vector3& position) {
	textureHandle_ = textureHandle;
	originPosition_ = position;

	// スプレッド開始時点は中心に集まった縮小状態から始める
	worldTransform_.scale_ = {kRingTargetScale * kSpreadStartScaleRate, kRingTargetScale * kSpreadStartScaleRate, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = originPosition_;

	worldTransform_.Initialize();

	alpha_ = 1.0f;

	// 状態遷移: まずはスプレッドから開始する
	ChangeState(State::kSpread);
}

void GuardEffect::ChangeState(State newState) {
	state_ = newState;
	counter_ = 0.0f;
}

void GuardEffect::Update() {
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
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void GuardEffect::UpdateSpread() {
	// スプレッド中は、輪モデルが中心から広がっていくようにスケールを変化させる(イージング)
	float t = Clamp01(counter_ / kSpreadDuration);
	float eased = EaseOutQuad(t);
	float scaleRate = Lerp(kSpreadStartScaleRate, kRingTargetScale, eased);

	worldTransform_.scale_ = {scaleRate, scaleRate, 1.0f};
	// 位置は中心に固定したまま(モデル自体が輪の形をしているため、移動は不要)
	worldTransform_.translation_ = originPosition_;

	// 継続時間に達したらフェードへ遷移
	if (counter_ >= kSpreadDuration) {
		ChangeState(State::kFade);
	}
}

void GuardEffect::UpdateFade() {
	// フェード中は輪が広がりきった状態のまま、アルファ値だけを変化させる(イージング)
	float t = Clamp01(counter_ / kFadeDuration);
	float eased = EaseInQuad(t);
	alpha_ = Lerp(1.0f, 0.0f, eased);

	// 継続時間に達したらインスタンスごと消滅(デス状態)へ遷移
	if (counter_ >= kFadeDuration) {
		alpha_ = 0.0f;
		ChangeState(State::kDead);
	}
}

void GuardEffect::Draw() {
	if (state_ == State::kDead || !model_ || !camera_) {
		return;
	}

	// フェード中のアルファ値をモデルに反映
	// TODO: お使いのModelクラスにあるアルファ/カラー設定用のAPIに置き換えてください。
	// 例: model_->SetColor({1.0f, 1.0f, 1.0f, alpha_});

	model_->Draw(worldTransform_, *camera_, textureHandle_);
}