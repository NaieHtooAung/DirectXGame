#include "PressRText.h"
#include "UpdateMatrix.h"
#include <cmath>

using namespace KamataEngine;

PressRText::~PressRText() {
	delete model_;
}

void PressRText::Initialize(const Vector3& basePosition) {
	// ---- Resources/pressR/pressR.png と pressR.obj を用意する場所 ----
	//textureHandle_ = TextureManager::Load("./Resources/pressR/pressR.png");
	model_ = Model::CreateFromOBJ("pressR", true);

	basePosition_ = basePosition;
	worldTransform_.Initialize();
	worldTransform_.translation_ = basePosition_;
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };

	
	// ---- 180度回転（向きを反転、上下逆さにはならない）----
	worldTransform_.rotation_.y = 3.14159f; // πラジアン = 180度
}

void PressRText::Update() {
	// ---- サイン波でY座標を上下させて浮遊感を出す ----
	floatTimer_ += kFloatSpeed_;
	worldTransform_.translation_ = basePosition_;
	worldTransform_.translation_.y += std::sin(floatTimer_) * kFloatAmplitude_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PressRText::Draw(const Camera& camera) {
	if (model_) {
		model_->Draw(worldTransform_, camera, textureHandle_);
	}
}