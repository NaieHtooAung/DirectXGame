#pragma once
#include <vector>
#include <KamataEngine.h>
#include "skydome.h"
#include "Player.h"
#include "MapChipField.h"
class GameScene {
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	bool isDebugCameraActive_ = false;

public:
	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();
	~GameScene();

private:
	uint32_t textureHandle_ = 0;
	uint32_t textureHandlePlayer_ = 0;
	MapChipField* mapchipField_;
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
	
};