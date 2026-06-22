#include "TitleScene.h"
#include "mathUti.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete titleModel_;
	delete playerModel_;
}

void TitleScene::Initialize() {

	// カメラ初期化
	camera_.Initialize();

	// "Sugoi Man" 3Dロゴモデルの読み込み
	titleTextureHandle_ = TextureManager::Load("./Resources/titleFont/white1x1.png");
	titleModel_ = Model::CreateFromOBJ("titleFont", true);

	titleWorldTransform_.Initialize();
	titleWorldTransform_.translation_ = {0.0f, 2.0f, 0.0f};
	titleWorldTransform_.scale_ = {2.0f, 2.0f, 2.0f};

	// プレイヤーアイコンモデルの読み込み
	playerTextureHandle_ = TextureManager::Load("./Resources/player/player.png");
	playerModel_ = Model::CreateFromOBJ("player", true);

	playerWorldTransform_.Initialize();
	playerWorldTransform_.translation_ = {0.0f, -5.0f, 0.0f};
	playerWorldTransform_.scale_ = {8.0f, 8.0f, 8.0f};
	playerWorldTransform_.rotation_.y = 3.14159265f;
}

void TitleScene::Update() {

	// タイマーを進める(浮遊アニメーション用)
	parameterTimer_ += 0.05f;

	// タイトルロゴをゆっくり上下に揺らす（浮遊エフェクト）
	titleWorldTransform_.translation_.y = 3.0f + std::sin(parameterTimer_) * 0.2f;

	// ワールド行列の更新
	titleWorldTransform_.matWorld_ = MakeAffineMatrix(titleWorldTransform_.scale_, titleWorldTransform_.rotation_, titleWorldTransform_.translation_);
	titleWorldTransform_.TransferMatrix();

	playerWorldTransform_.matWorld_ = MakeAffineMatrix(playerWorldTransform_.scale_, playerWorldTransform_.rotation_, playerWorldTransform_.translation_);
	playerWorldTransform_.TransferMatrix();

	camera_.UpdateMatrix();

	// スペースキーでゲームシーンへ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {
	Model::PreDraw();

	titleModel_->Draw(titleWorldTransform_, camera_, titleTextureHandle_);
	playerModel_->Draw(playerWorldTransform_, camera_, playerTextureHandle_);

	Model::PostDraw();
}