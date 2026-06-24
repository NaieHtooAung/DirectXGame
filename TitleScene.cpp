#include "TitleScene.h"
#include "mathUti.h"
#include <cmath>

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete titleModel_;
	delete playerModel_;
	delete fade_;
}

void TitleScene::Initialize() {

	// カメラ初期化
	camera_.Initialize();

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 3.0f);
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

	fade_->Update();

	switch (phase_) {

	case Phase::kFadeIn:

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kMain:

		// スペースキーでフェードアウト開始
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {

			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;

	case Phase::kFadeOut:

		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	// タイマーを進める
	parameterTimer_ += 0.05f;

	// タイトル浮遊
	titleWorldTransform_.translation_.y = 3.0f + std::sin(parameterTimer_) * 0.2f;

	titleWorldTransform_.matWorld_ = MakeAffineMatrix(titleWorldTransform_.scale_, titleWorldTransform_.rotation_, titleWorldTransform_.translation_);

	titleWorldTransform_.TransferMatrix();

	playerWorldTransform_.matWorld_ = MakeAffineMatrix(playerWorldTransform_.scale_, playerWorldTransform_.rotation_, playerWorldTransform_.translation_);

	playerWorldTransform_.TransferMatrix();

	camera_.UpdateMatrix();
}
void TitleScene::Draw() {

	Model::PreDraw();

	titleModel_->Draw(titleWorldTransform_, camera_, titleTextureHandle_);

	playerModel_->Draw(playerWorldTransform_, camera_, playerTextureHandle_);

	Model::PostDraw();

	fade_->Draw();
}