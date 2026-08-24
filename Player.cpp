#include "Player.h"
#include "MyMath.h"

Player::~Player() { delete model_; }

void Player::Initialize() {
	// ▼▼▼ ここにプレイヤーのテクスチャパスを入れる ▼▼▼
	textureHandle_ = TextureManager::Load("./Resources/player/rabbit.png");

	// "player.obj" は Resources/player フォルダに用意してください
	model_ = Model::CreateFromOBJ("player", true);

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 1.0f, 0.0f};
}

void Player::Update() {
	Input* input = Input::GetInstance();

	Vector3 move = {0.0f, 0.0f, 0.0f};

	// 矢印キーで移動(前後左右)
	if (input->PushKey(DIK_LEFT)) {
		move.x -= kSpeed;
	}
	if (input->PushKey(DIK_RIGHT)) {
		move.x += kSpeed;
	}
	if (input->PushKey(DIK_UP)) {
		move.z += kSpeed;
	}
	if (input->PushKey(DIK_DOWN)) {
		move.z -= kSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.z += move.z;

	// フィールドの外に出ないようにする
	if (worldTransform_.translation_.x > kFieldHalfSize) {
		worldTransform_.translation_.x = kFieldHalfSize;
	}
	if (worldTransform_.translation_.x < -kFieldHalfSize) {
		worldTransform_.translation_.x = -kFieldHalfSize;
	}
	if (worldTransform_.translation_.z > kFieldHalfSize) {
		worldTransform_.translation_.z = kFieldHalfSize;
	}
	if (worldTransform_.translation_.z < -kFieldHalfSize) {
		worldTransform_.translation_.z = -kFieldHalfSize;
	}

	// ワールド行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrixMy(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw(const Camera& camera) {
	// エンジンがテクスチャハンドル引数に対応していない場合はこちら(2引数)
	model_->Draw(worldTransform_, camera);

	// もしModel::Drawがテクスチャハンドルを受け取れるバージョンなら、
	// 上の行をコメントアウトして下の行を使ってください
	// model_->Draw(worldTransform_, camera, textureHandle_);
}