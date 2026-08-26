#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// リトライ待ち画面の"Press R"3D文字を上下にふわふわ浮かせて表示するクラス
// ---------------------------------------------
class PressRText {
public:
	~PressRText();

	void Initialize(const Vector3& basePosition);
	void Update();
	void Draw(const Camera& camera);

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandle_ = 0;

	Vector3 basePosition_{};
	float floatTimer_ = 0.0f;

	static inline const float kFloatSpeed_ = 0.05f;      // 浮遊の速さ
	static inline const float kFloatAmplitude_ = 0.3f;   // 浮遊の振れ幅
};