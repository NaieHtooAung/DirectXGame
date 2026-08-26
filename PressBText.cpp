#include "PressBText.h"
#include "UpdateMatrix.h"
#include <cmath>

using namespace KamataEngine;

PressBText::~PressBText() {
	delete model_;
}

void PressBText::Initialize(const Vector3& basePosition) {
	// ---- Resources/pressB/pressB.obj を用意する場所 ----
	model_ = Model::CreateFromOBJ("pressB", true);

	basePosition_ = basePosition;
	worldTransform_.Initialize();
	worldTransform_.translation_ = basePosition_;

	// ---- 90度回転（寝ている状態から立たせる）----
	worldTransform_.rotation_.x = 1.5708f; // 追加：X軸-90度で立たせる
	worldTransform_.rotation_.y = 3.14159f; // 180度回転してカメラ側を向かせる

	

	// ---- サイズを大きくする ----
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void PressBText::Update() {
	// ---- サイン波でY座標を上下させて浮遊感を出す ----
	floatTimer_ += kFloatSpeed_;
	worldTransform_.translation_ = basePosition_;
	worldTransform_.translation_.y += std::sin(floatTimer_) * kFloatAmplitude_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PressBText::Draw(const Camera& camera) {
	if (model_) {
		model_->Draw(worldTransform_, camera, textureHandle_);
	}
}