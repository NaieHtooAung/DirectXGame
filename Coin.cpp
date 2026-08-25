#include "Coin.h"
#include"UpdateMatrix.h"
void Coin::Initialize(const Vector3& position) {
	// 後でコインモデルに差し替える場所
	// model_ = Model::CreateFromOBJ("coin", true);
	model_ = Model::CreateFromOBJ("cube", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.5f, 0.5f, 0.5f };
}

void Coin::Update() {
	if (isGot_) {
		return;
	}

	// くるくる回して目立たせる演出
	spinTimer_ += 0.05f;
	worldTransform_.rotation_.y = spinTimer_;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Coin::Draw(const Camera& camera) {
	if (!isGot_) {
		model_->Draw(worldTransform_, camera);
	}
}

void Coin::Reset() {
	isGot_ = false;
	spinTimer_ = 0.0f;
}