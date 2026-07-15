#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

// HitEffectを複製してリネームしたクラス。
// ShieldEnemyがガードに成功した時に再生する演出。
// NOTE: 専用のリング(輪)形状モデルを1つ生成し、それを中心から
// 拡大させることで「白い輪が広がって消える」見た目を作る。
// 以前は破片を24個並べて疑似的に輪を表現していたが、
// リング形状のモデルが用意されたため、その手法は不要になった。
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
		kSpread, // 輪が広がっていくアニメーション中
		kFade,   // フェードアウト中
		kDead,   // 消滅(削除待ち)
	};

	void UpdateSpread();
	void UpdateFade();

	// 状態切り替え機構(カウンタもリセットする)
	void ChangeState(State newState);

	static Model* model_;
	static Camera* camera_;

	uint32_t textureHandle_ = 0u;

	// 輪モデルのワールドトランスフォーム(1個のみ)
	WorldTransform worldTransform_;

	// 発生位置(輪の中心)
	Vector3 originPosition_{};

	// 現在の状態
	State state_ = State::kDead;

	// 状態内カウンタ(加算していき、状態ごとの継続時間に達したら遷移する)
	float counter_ = 0.0f;

	// アルファ値(フェード中に変化させる)
	float alpha_ = 1.0f;

	static constexpr float kDeltaTime = 1.0f / 60.0f;

	static constexpr float kSpreadDuration = 0.10f; // 輪が広がる継続時間(秒)
	static constexpr float kFadeDuration = 0.15f;   // フェードの継続時間(秒)

	// スプレッド開始時点のスケール倍率(中心に集まった縮小状態から始める)
	static constexpr float kSpreadStartScaleRate = 0.2f;

	// 輪モデルの最終的なスケール倍率(モデル自体の半径に対する倍率)
	static constexpr float kRingTargetScale = 1.0f;
};