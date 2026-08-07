#include "Floor.h"
#include "MyMath.h"

Floor::~Floor() { delete model_; }

void Floor::Initialize() {
	// ▼▼▼ ここに床のテクスチャパスを入れる ▼▼▼
	textureHandle_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");

	// "floor.obj" は 1x1x1 の平たい板を想定。scale_で引き伸ばして使う
	model_ = Model::CreateFromOBJ("SkyDome", true);

	worldTransform_.Initialize();
	worldTransform_.scale_ = {kFieldHalfSize, 0.1f, kFieldHalfSize};
	worldTransform_.translation_ = {0.0f, -0.5f, 0.0f};

	worldTransform_.matWorld_ = MakeAffineMatrixMy(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Floor::Draw(const Camera& camera) {
	// エンジンがテクスチャハンドル引数に対応していない場合はこちら(2引数)
	model_->Draw(worldTransform_, camera);

	// もしModel::Drawがテクスチャハンドルを受け取れるバージョンなら、
	// 上の行をコメントアウトして下の行を使ってください
	// model_->Draw(worldTransform_, camera, textureHandle_);
}