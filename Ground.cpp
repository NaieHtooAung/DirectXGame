#include "Ground.h"
#include "UpdateMatrix.h"
void Ground::Initialize() {
	textureHandleFloor_ = TextureManager::Load("./Resources/Floor/floorBrick.png");
	model_ = Model::CreateFromOBJ("floor", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0.0f, -1.0f, 6.0f };
	// 床っぽく見えるように平べったく、広めに引き伸ばす
	worldTransform_.scale_ = { 15.0f, 0.2f, 20.0f };

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Ground::Draw(const Camera& camera) { model_->Draw(worldTransform_, camera); }