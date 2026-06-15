#pragma once
#include <vector>
#include <KamataEngine.h>
#include "skydome.h"
#include "Player.h"
#include "Enemy.h"
#include "CameraController.h"
#include "MapChipField.h"
#include "DeathParticles.h"
class GameScene {
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::list<Enemy*> enemies_;
	bool isDebugCameraActive_ = false;

public:
	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();

	void CheckAllCollisions();
	~GameScene();

private:
	uint32_t textureHandle_ = 0;
	uint32_t textureHandlePlayer_ = 0;
	uint32_t textureHandleEnemy_ = 0;
	MapChipField* mapchipField_;
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	Model* enemyModel_ = nullptr;
	CameraController* cameraController_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* deathParticlesModel_ = nullptr;

	skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
};