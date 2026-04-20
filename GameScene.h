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
	
	uint32_t textureHandle1_ = 0;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::Model* model_ = nullptr;
};