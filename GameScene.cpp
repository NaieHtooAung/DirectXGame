#include "GameScene.h"
#include "skydome.h"
#include "Player.h"
#include "mathUti.h"
#include <cmath>

using namespace KamataEngine;

// =========================
// Initialize
// =========================
void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("./Resources/cube/cube.jpg");

	blockModel_ = Model::Create();

	camera_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	isDebugCameraActive_ = true;

	player_ = new Player();

	textureHandlePlayer_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	player_->Initialize(model_, textureHandlePlayer_, &camera_);

	skydome_ = new skydome();

	skydome_->initialize();

	camera_.farZ = 1000.0f;
	camera_.UpdateMatrix();

	mapchipField_ = new MapChipField;
	mapchipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();
	
}

// =========================
// Update
// =========================
void GameScene::Update() {
	player_->Update();
	skydome_->update();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
				
			}
			// Scale
			worldTransformBlock->scale_ = {1.0f, 1.0f, 1.0f};

			// Rotation
			worldTransformBlock->rotation_.y = 0.00f;

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
	} else {

		camera_.UpdateMatrix();
	}
}


// =========================
// Draw
// =========================
void GameScene::Draw() {

	Model::PreDraw();

	player_->Draw();
	// skydome
	skydome_->Draw(camera_);

	// blocks
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

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapchipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapchipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(numBlockHorizontal);

	}
	for (uint32_t i = 0; i < numBlockVirtical; i++) {

		for (uint32_t j = 0; j < numBlockHorizontal; j++) {

			if (mapchipField_->GetmapChipTypeByIndex(j, i) != MapChipType::kBlock) {
				continue;
			}

			worldTransformBlocks_[i][j] = new WorldTransform();

			worldTransformBlocks_[i][j]->Initialize();

			Vector3 blockPosition = mapchipField_->GetmapChipPositionByIndex(j, i);

			worldTransformBlocks_[i][j]->translation_ = blockPosition;
		}
	}

}

// =========================
// Destructor
// =========================
GameScene::~GameScene() {

	delete blockModel_;
	delete model_;
	delete debugCamera_;
	delete player_;
	delete skydome_;
	delete mapchipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}
	
	worldTransformBlocks_.clear();
}