#include "GameScene.h"
#include "Player.h"
#include <cmath>

using namespace KamataEngine;

// =========================
// MakeAffineMatrix
// =========================
Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {

	Matrix4x4 matrix{};

	float cosX = cosf(rotate.x);
	float sinX = sinf(rotate.x);

	float cosY = cosf(rotate.y);
	float sinY = sinf(rotate.y);

	float cosZ = cosf(rotate.z);
	float sinZ = sinf(rotate.z);

	matrix.m[0][0] = scale.x * (cosY * cosZ);
	matrix.m[0][1] = scale.x * (cosY * sinZ);
	matrix.m[0][2] = scale.x * (-sinY);
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = scale.y * (sinX * sinY * cosZ - cosX * sinZ);
	matrix.m[1][1] = scale.y * (sinX * sinY * sinZ + cosX * cosZ);
	matrix.m[1][2] = scale.y * (sinX * cosY);
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = scale.z * (cosX * sinY * cosZ + sinX * sinZ);
	matrix.m[2][1] = scale.z * (cosX * sinY * sinZ - sinX * cosZ);
	matrix.m[2][2] = scale.z * (cosX * cosY);
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;
	matrix.m[3][2] = translate.z;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

// =========================
// Initialize
// =========================
void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");

	blockModel_ = Model::Create();

	camera_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	isDebugCameraActive_ = true;

	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockHeight = 2.0f;
	const float kBlockWidth = 2.0f;
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);

	}
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if ((i + j) % 2 == 0) {

				delete worldTransformBlocks_[i][j];

				worldTransformBlocks_[i][j] = nullptr;

				continue;
			}
			worldTransformBlocks_[i][j] = new WorldTransform();
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}

// =========================
// Update
// =========================
void GameScene::Update() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
				
			}
			// Scale
			worldTransformBlock->scale_ = {1.0f, 1.0f, 1.0f};

			// Rotation
			worldTransformBlock->rotation_.y += 0.01f;

			// World Matrix
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// Transfer
			worldTransformBlock->TransferMatrix();
			
		}
	}
	
#ifdef DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		if (isDebugCameraActive_) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		} else {
			isDebugCameraActive_ = true;
		}
		
	}

#endif
	debugCamera_->Update();
	if (isDebugCameraActive_) {
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
		;

	} else {
		camera_.UpdateMatrix();
	}
}


// =========================
// Draw
// =========================
void GameScene::Draw() {

	Model::PreDraw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}
			blockModel_->Draw(*worldTransformBlock, camera_, textureHandle_);
		}
	}
	Model::PostDraw();
}

// =========================
// Destructor
// =========================
GameScene::~GameScene() {

	delete blockModel_;
	delete debugCamera_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}
	
	worldTransformBlocks_.clear();
}