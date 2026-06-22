#pragma once
#include <KamataEngine.h>

class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();

	~TitleScene();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;

	// カメラ
	KamataEngine::Camera camera_;

	// "Sugoi Man" 3Dロゴ
	KamataEngine::Model* titleModel_ = nullptr;
	uint32_t titleTextureHandle_ = 0;
	KamataEngine::WorldTransform titleWorldTransform_;

	// プレイヤーアイコン
	KamataEngine::Model* playerModel_ = nullptr;
	uint32_t playerTextureHandle_ = 0;
	KamataEngine::WorldTransform playerWorldTransform_;

	// アニメーション用タイマー
	float parameterTimer_ = 0.0f;
};