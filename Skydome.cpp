#include "SkyDome.h"
#include "UpdateMatrix.h"

void SkyDome::Initialize() {
	textureHandleSky_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");
	model_ = Model::CreateFromOBJ("cube", true);

	worldTransform_.Initialize();
	// 内側から見ても違和感がないよう大きくしておく（遠くのクリップ面を超えない程度に）
	worldTransform_.scale_ = { 50.0f, 50.0f, 50.0f };

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void SkyDome::Update(const Camera& camera) {
	// ---- カメラワーク：スカイドームはカメラの位置に常について来る ----
	worldTransform_.translation_ = camera.translation_;

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void SkyDome::Draw(const Camera& camera) {
	model_->Draw(worldTransform_, camera, textureHandleSky_);
}