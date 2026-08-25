#pragma once
#include <KamataEngine.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Particle.h"

using namespace KamataEngine;

// ---------------------------------------------
// シーン一覧（タイトル→スタート演出→プレイ→クリア/ゲームオーバー）
// ---------------------------------------------
enum class Scene {
	kTitle,
	kStart,
	kPlay,
	kClear,
	kGameOver,
};

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	void UpdateTitle();
	void UpdateStart();
	void UpdatePlay();

	Scene scene_ = Scene::kTitle;
	bool isPaused_ = false;

	// ---- 画面遷移（フェード）----
	float fade_ = 1.0f;
	int32_t startTimer_ = 0;

	// ---- 3Dカメラ（カメラワーク） ----
	Camera camera_;

	// ---- サウンド（後で実際のファイルに差し替える場所）----
	// uint32_t bgmHandle_ = Audio::GetInstance()->LoadWave("bgm.wav");
	// uint32_t seHitHandle_ = Audio::GetInstance()->LoadWave("hit.wav");
	// uint32_t seClearHandle_ = Audio::GetInstance()->LoadWave("clear.wav");
	uint32_t bgmVoiceHandle_ = 0u;

	Player* player_ = nullptr;
	std::vector<Enemy*> enemies_;
	std::vector<Coin*> coins_;
	ParticleManager* particleManager_ = nullptr;

	// フェード用の黒背景スプライト（画面遷移のみ2D）
	uint32_t blackTextureHandle_ = 0;
	Sprite* fadeSprite_ = nullptr;

	// タイトル画面用スプライト（後で画像に差し替える）
	uint32_t titleTextureHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
};