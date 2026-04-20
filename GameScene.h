#pragma once
#include <KamataEngine.h>

class GameScene
{
public:
	void Initialize();

	void Update();

	void Draw();
	~GameScene();
	private:
	uint32_t textureHandle_ = 0;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
};