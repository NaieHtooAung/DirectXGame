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
	//uint32_t textureHandle_ = 0;
	uint32_t textureHandle1_ = 0;
	//uint32_t soundDataHandle_ = 0;
	//uint32_t voiceHandle_ = 0;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	//KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	//float inputFloat3[3] = { 0, 0, 0 };
};