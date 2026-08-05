#include "GameScene.h"
#include "MyMath.h"
#include <cmath>
#include <cstdlib>

GameScene::~GameScene() {
	delete playerModel_;
	delete floorModel_;
	delete itemModel_;
}

void GameScene::Initialize() {
	// ----- カメラ -----
	camera_.Initialize();
	// 少し上、少し後ろからフィールド全体を見下ろす
	camera_.translation_ = {0.0f, 22.0f, -22.0f};
	camera_.rotation_.x = 0.75f; // ラジアン。見下ろし角度
	camera_.UpdateMatrix();

	// ----- プレイヤー -----
	// "player.obj" は Resources/player フォルダに用意してください(立方体や球でOK)
	
	// ▼▼▼ ここにプレイヤーのテクスチャパスを入れる ▼▼▼
	playerTextureHandle_ = TextureManager::Load("./Resources/player/player.png");
	playerModel_ = Model::CreateFromOBJ("player", true);
	playerTransform_.Initialize();
	playerTransform_.translation_ = {0.0f, 1.0f, 0.0f};

	// ----- 床 -----
	// "floor.obj" は 1x1x1 の平たい板を想定。scale_で引き伸ばして使う
	
	// ▼▼▼ ここに床のテクスチャパスを入れる ▼▼▼
	floorTextureHandle_ = TextureManager::Load("./Resources/SkyDome/sky_sphere.png");
	floorModel_ = Model::CreateFromOBJ("SkyDome", true);
	floorTransform_.Initialize();
	floorTransform_.scale_ = {kFieldHalfSize, 0.1f, kFieldHalfSize};
	floorTransform_.translation_ = {0.0f, -0.5f, 0.0f};

	// ----- アイテム -----
	
	// ▼▼▼ ここにアイテムのテクスチャパスを入れる ▼▼▼
	itemTextureHandle_ = TextureManager::Load("./Resources/ring/ring.png");
	itemModel_ = Model::CreateFromOBJ("ring", true);
	for (int i = 0; i < kItemCount; ++i) {
		itemTransforms_[i].Initialize();
		SpawnItem(i);
		itemAlive_[i] = true;
	}

	score_ = 0;
	timeLeft_ = 60 * 30;
	isGameOver_ = false;
}

void GameScene::Reset() { Initialize(); }

void GameScene::SpawnItem(int index) {
	// フィールドの内側(端から少し余裕を持たせた範囲)にランダム配置
	float range = kFieldHalfSize - 2.0f;
	float x = (static_cast<float>(rand()) / RAND_MAX) * range * 2.0f - range;
	float z = (static_cast<float>(rand()) / RAND_MAX) * range * 2.0f - range;

	itemTransforms_[index].translation_ = {x, 1.0f, z};
}

void GameScene::MovePlayer() {
	if (isGameOver_) {
		return;
	}

	Input* input = Input::GetInstance();

	Vector3 move = {0.0f, 0.0f, 0.0f};

	// 矢印キーで移動(前後左右)
	if (input->PushKey(DIK_LEFT)) {
		move.x -= kPlayerSpeed;
	}
	if (input->PushKey(DIK_RIGHT)) {
		move.x += kPlayerSpeed;
	}
	if (input->PushKey(DIK_UP)) {
		move.z += kPlayerSpeed;
	}
	if (input->PushKey(DIK_DOWN)) {
		move.z -= kPlayerSpeed;
	}

	playerTransform_.translation_.x += move.x;
	playerTransform_.translation_.z += move.z;

	// フィールドの外に出ないようにする
	if (playerTransform_.translation_.x > kFieldHalfSize) {
		playerTransform_.translation_.x = kFieldHalfSize;
	}
	if (playerTransform_.translation_.x < -kFieldHalfSize) {
		playerTransform_.translation_.x = -kFieldHalfSize;
	}
	if (playerTransform_.translation_.z > kFieldHalfSize) {
		playerTransform_.translation_.z = kFieldHalfSize;
	}
	if (playerTransform_.translation_.z < -kFieldHalfSize) {
		playerTransform_.translation_.z = -kFieldHalfSize;
	}
}

void GameScene::CheckCollision() {
	if (isGameOver_) {
		return;
	}

	for (int i = 0; i < kItemCount; ++i) {
		if (!itemAlive_[i]) {
			continue;
		}

		Vector3& p = playerTransform_.translation_;
		Vector3& t = itemTransforms_[i].translation_;

		float dx = p.x - t.x;
		float dz = p.z - t.z;
		float distance = std::sqrt(dx * dx + dz * dz);

		if (distance < kCollisionDistance) {
			// 取得!スコア加算して、別の場所に再配置する
			score_ += 1;
			SpawnItem(i);
		}
	}
}

void GameScene::Update() {
	// リトライ(ゲームオーバー時にRキーでリセット)
	if (isGameOver_) {
		if (Input::GetInstance()->PushKey(DIK_R)) {
			Reset();
		}
		return;
	}

	// タイマー更新
	if (timeLeft_ > 0) {
		timeLeft_--;
	} else {
		isGameOver_ = true;
	}

	MovePlayer();
	CheckCollision();

	// ワールド行列の更新(UpdateMatrix()もMakeAffineMatrix()も無いバージョン用)
	// 自作のMakeAffineMatrixMy()でscale_/rotation_/translation_から行列を作り、GPUに転送する
	playerTransform_.matWorld_ = MakeAffineMatrixMy(playerTransform_.scale_, playerTransform_.rotation_, playerTransform_.translation_);
	playerTransform_.TransferMatrix();

	floorTransform_.matWorld_ = MakeAffineMatrixMy(floorTransform_.scale_, floorTransform_.rotation_, floorTransform_.translation_);
	floorTransform_.TransferMatrix();

	for (int i = 0; i < kItemCount; ++i) {
		itemTransforms_[i].matWorld_ = MakeAffineMatrixMy(itemTransforms_[i].scale_, itemTransforms_[i].rotation_, itemTransforms_[i].translation_);
		itemTransforms_[i].TransferMatrix();
	}

	camera_.UpdateMatrix();
}
void GameScene::Draw() {
	// 床
	floorModel_->Draw(floorTransform_, camera_, floorTextureHandle_);

	// プレイヤー
	playerModel_->Draw(playerTransform_, camera_, playerTextureHandle_);

	// 生存しているアイテムだけ描画
	for (int i = 0; i < kItemCount; ++i) {
		if (itemAlive_[i]) {
			itemModel_->Draw(itemTransforms_[i], camera_, itemTextureHandle_);
		}
	}

	// スコアと残り時間はデバッグ出力で確認できます(下部の解説を参照)
}