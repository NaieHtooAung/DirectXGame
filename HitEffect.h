#pragma once
#include "KamataEngine.h"
#include <array>
#include <random>
using namespace KamataEngine;

class HitEffect {
public:
	static void SetModel(Model* model) { model_ = model; }
	static void SetCamera(Camera* camera) { camera_ = camera; }

	// インスタンス生成と初期化
	static HitEffect* Create(uint32_t textureHandle, const Vector3& position);

	void Update();
	void Draw();

	bool IsFinished() const { return isFinished_; }

private:
	// 発生位置を指定してエフェクトを再生開始する
	void Initialize(uint32_t textureHandle, const Vector3& position);

	static const int32_t kNumEllipse = 3; // 楕円の個数

	static Model* model_;
	static Camera* camera_;

	// 乱数生成エンジン(共通の初期化)
	static std::mt19937_64 randomEngine_;

	uint32_t textureHandle_ = 0u;

	// 楕円のワールドトランスフォーム
	std::array<WorldTransform, kNumEllipse> ellipseWorldTransforms_;

	float timer_ = 0.0f;
	float duration_ = 0.2f; // 表示時間(秒)

	static constexpr float kEllipseWidth = 0.3f;  // 楕円の幅
	static constexpr float kEllipseLength = 6.0f; // 楕円の長さ

	bool isFinished_ = true;
};