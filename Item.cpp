#include "Item.h"
#include "MyMath.h"
#include <cstdlib>

void Item::Initialize(Model* model, uint32_t textureHandle) {
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	isAlive_ = true;
	Respawn();
}

void Item::Respawn() {
	// フィールドの内側(端から少し余裕を持たせた範囲)にランダム配置
	float range = kFieldHalfSize - 2.0f;
	float x = (static_cast<float>(rand()) / RAND_MAX) * range * 2.0f - range;
	float z = (static_cast<float>(rand()) / RAND_MAX) * range * 2.0f - range;

	worldTransform_.translation_ = {x, 1.0f, z};
}

void Item::Update() {
	if (!isAlive_) {
		return;
	}

	worldTransform_.matWorld_ = MakeAffineMatrixMy(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Item::Draw(const Camera& camera) {
	if (!isAlive_) {
		return;
	}

	// エンジンがテクスチャハンドル引数に対応していない場合はこちら(2引数)
	model_->Draw(worldTransform_, camera);

	// もしModel::Drawがテクスチャハンドルを受け取れるバージョンなら、
	// 上の行をコメントアウトして下の行を使ってください
	// model_->Draw(worldTransform_, camera, textureHandle_);
}