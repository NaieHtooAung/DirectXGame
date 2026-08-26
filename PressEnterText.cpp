#include "PressEnterText.h"
#include "UpdateMatrix.h"
#include <cmath>

using namespace KamataEngine;

PressEnterText::~PressEnterText() {
	delete model_;
}

void PressEnterText::Initialize(const Vector3& basePosition) {
	// ---- Resources/pressEnter/pressEnter.png と pressEnter.obj を用意する場所 ----
	//textureHandle_ = TextureManager::Load("./Resources/pressEnter/pressEnter.png");
	model_ = Model::CreateFromOBJ("pressEnter", true);

	basePosition_ = basePosition;
	worldTransform_.Initialize();
	worldTransform_.translation_ = basePosition_;

	// ---- 90度回転（上から見えるように寝かせる）----
	
	worldTransform_.rotation_.y = 3.14159f; // spin it 180° around to face the other way, without flipping upside down

	// ---- サイズを大きくする ----
	worldTransform_.scale_ = { 3.0f, 3.0f, 3.0f };
}

void PressEnterText::Update() {
	// ---- サイン波でY座標を上下させて浮遊感を出す ----
	floatTimer_ += kFloatSpeed_;
	worldTransform_.translation_ = basePosition_;
	worldTransform_.translation_.y += std::sin(floatTimer_) * kFloatAmplitude_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void PressEnterText::Draw(const Camera& camera) {
	if (model_) {
		model_->Draw(worldTransform_, camera, textureHandle_);
	}
}