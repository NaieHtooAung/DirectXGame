#pragma once
#include <vector>
#include <KamataEngine.h>
class GameScene {
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	bool isDebugCameraActive_ = false;

public:
	void Initialize();

	void Update();

	void Draw();
	~GameScene();

private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
};