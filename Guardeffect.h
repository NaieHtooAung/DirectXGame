#pragma once
#include "KamataEngine.h"
#include <array>
#include <random>
using namespace KamataEngine;

// HitEffectを複製してリネームしたクラス。
// ShieldEnemyがガードに成功した時に再生する演出。
class GuardEffect {
public:
	static void SetModel(Model* model) { model_ = model; }
	static void SetCamera(Camera* camera) { camera_ = camera; }

	// インスタンス生成と初期化
	static GuardEffect* Create(uint32_t textureHandle, const Vector3& position);

	void Update();
	void Draw();

	// デスフラグの取得
	bool IsDead() const { return state_ == State::kDead; }

private:
	// 発生位置を指定してエフェクトを再生開始する
	void Initialize(uint32_t textureHandle, const Vector3& position);

	// 状態遷移
	enum class State {
		kSpread, // 拡大アニメーション中
		kFade,   // フェードアウト中
		kDead,   // 消滅(削除待ち)
	};

	void UpdateSpread();
	void UpdateFade();

	// 状態切り替え機構(カウンタもリセットする)
	void ChangeState(State newState);

	static const int32_t kNumEllipse = 3; // 楕円の個数

	static Model* model_;
	static Camera* camera_;

	// 乱数生成エンジン(共通の初期化)
	static std::mt19937_64 randomEngine_;

	uint32_t textureHandle_ = 0u;

	// 楕円のワールドトランスフォーム
	std::array<WorldTransform, kNumEllipse> ellipseWorldTransforms_;

	// 現在の状態
	State state_ = State::kDead;

	// 状態内カウンタ(加算していき、状態ごとの継続時間に達したら遷移する)
	float counter_ = 0.0f;

	// アルファ値(フェード中に変化させる)
	float alpha_ = 1.0f;

	static constexpr float kDeltaTime = 1.0f / 60.0f;

	static constexpr float kSpreadDuration = 0.10f; // スプレッドの継続時間(秒)
	static constexpr float kFadeDuration = 0.15f;   // フェードの継続時間(秒)

	static constexpr float kSpreadStartScaleRate = 0.2f; // スプレッド開始時点のスケール倍率

	static constexpr float kEllipseWidth = 0.5f;  // 楕円の幅
	static constexpr float kEllipseLength = 6.0f; // 楕円の長さ
};