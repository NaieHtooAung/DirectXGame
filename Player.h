#pragma once
#include <KamataEngine.h>
class  Player {
public:
	void Initialize(KamataEngine::Model* model,uint32_t textrueHandle_);
	void Update();
	void Draw();
	~ Player();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
};

 