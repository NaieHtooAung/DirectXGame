#pragma once
#include <KamataEngine.h>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Particle.h"
#include "Ground.h"
#include "SkyDome.h"

using namespace KamataEngine;

// ---------------------------------------------
// シーン一覧：スタート→ゲーム→エンド→（リトライで）ゲーム、のループ
// ---------------------------------------------
enum class Scene {
	kStart,
	kGame,
	kEnd,
};

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	void UpdateStart();
	void UpdateGame();
	void UpdateEnd();

	// ゲームの状態を初期値に戻す（エンド→ゲームのループで使う）
	void Reset();

	Scene scene_ = Scene::kStart;
	bool isPaused_ = false;
	bool isCountingDown_ = false; // スタート演出中かどうか
	bool won_ = false;            // エンドの結果（true=クリア, false=ゲームオーバー）

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
	Ground* ground_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	// フェード用の黒背景スプライト（画面遷移のみ2D）
	uint32_t blackTextureHandle_ = 0;
	Sprite* fadeSprite_ = nullptr;

	// タイトル画面用スプライト（後で画像に差し替える）
	uint32_t titleTextureHandle_ = 0;
	int32_t textureHandleSky_ = 0;
	Sprite* titleSprite_ = nullptr;

	// エンド画面用の色付きオーバーレイ（クリア=緑、ゲームオーバー=赤）
	Sprite* endSprite_ = nullptr;
};