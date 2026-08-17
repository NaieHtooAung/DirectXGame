#include "GameScene.h"
#include <cmath>

GameScene::~GameScene() { delete itemModel_; }

void GameScene::Initialize() {
	// ----- カメラ -----
	camera_.Initialize();
	// 少し上、少し後ろからフィールド全体を見下ろす
	camera_.translation_ = {0.0f, 22.0f, -22.0f};
	camera_.rotation_.x = 0.75f; // ラジアン。見下ろし角度
	camera_.UpdateMatrix();

	// ----- プレイヤー -----
	player_.Initialize();

	// ----- 床 -----
	floor_.Initialize();

	// ----- アイテム -----
	// モデルとテクスチャは1回だけ読み込み、全アイテムで使い回す
	// ▼▼▼ ここにアイテムのテクスチャパスを入れる ▼▼▼
	itemTextureHandle_ = TextureManager::Load("./Resources/ring/ring.png");
	itemModel_ = Model::CreateFromOBJ("ring", true);

	for (int i = 0; i < kItemCount; ++i) {
		items_[i].Initialize(itemModel_, itemTextureHandle_);
	}

	score_ = 0;
	timeLeft_ = 60 * 30;
	isGameOver_ = false;
}

void GameScene::Reset() { Initialize(); }

void GameScene::CheckCollision() {
	if (isGameOver_) {
		return;
	}

	const Vector3& p = player_.GetPosition();

	for (int i = 0; i < kItemCount; ++i) {
		if (!items_[i].IsAlive()) {
			continue;
		}

		const Vector3& t = items_[i].GetPosition();

		float dx = p.x - t.x;
		float dz = p.z - t.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance < kCollisionDistance) {
			// 取得!スコア加算して、別の場所に再配置する
			score_ += 1;
			items_[i].Respawn();
		}
	}
}

void GameScene::Update() {
	// タイムアップしたら何もしない(シーン切り替えはmain.cppが担当する)
	if (isGameOver_) {
		return;
	}

	// タイマー更新
	if (timeLeft_ > 0) {
		timeLeft_--;
	} else {
		isGameOver_ = true;
	}

	player_.Update();
	for (int i = 0; i < kItemCount; ++i) {
		items_[i].Update();
	}

	CheckCollision();

	camera_.UpdateMatrix();
}

void GameScene::Draw() {
	floor_.Draw(camera_);
	player_.Draw(camera_);

	for (int i = 0; i < kItemCount; ++i) {
		items_[i].Draw(camera_);
	}

	// スコアと残り時間はデバッグ出力で確認できます(下部の解説を参照)
}