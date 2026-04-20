#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
class  Player {
public:
	void Initialize(KamataEngine::Model* model,uint32_t textrueHandle, KamataEngine::Camera* camera);
	void Update();
	void Draw();
	~ Player();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Camera* camera_ = nullptr;
};

 