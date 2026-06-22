#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "Player.h"
#include "skydome.h"
#include <KamataEngine.h>
#include <list>
#include <vector>

class GameScene {
public:
	void Initialize();

	void Update();

	void Draw();

	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	bool IsFinished() const { return finished_; }

	~GameScene();

private:
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	std::list<Enemy*> enemies_;
	bool isDebugCameraActive_ = false;

	enum class Phase {
		kFadeIn,
		kPlay,
		kDeath,
		kFadeOut,
	};
	Phase phase_;

	bool finished_ = false;

	uint32_t textureHandle_ = 0;
	uint32_t textureHandlePlayer_ = 0;
	uint32_t textureHandleEnemy_ = 0;
	MapChipField* mapchipField_ = nullptr;
	KamataEngine::Model* blockModel_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* enemyModel_ = nullptr;
	CameraController* cameraController_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	KamataEngine::Model* deathParticlesModel_ = nullptr;

	skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
};