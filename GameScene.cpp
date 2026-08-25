#include "GameScene.h"

GameScene::~GameScene() {
	delete player_;
	for (auto* e : enemies_) {
		delete e;
	}
	for (auto* c : coins_) {
		delete c;
	}
	delete particleManager_;
	delete fadeSprite_;
	delete titleSprite_;
}

void GameScene::Initialize() {
	// ---- 3Dカメラの初期化（プレイヤーの少し後ろ上から見下ろす）----
	camera_.Initialize();
	camera_.translation_ = { 0.0f, 5.0f, -20.0f };
	camera_.rotation_.x = 0.3f;

	player_ = new Player();
	player_->Initialize();

	// ---- 敵やギミックの配置 ----
	enemies_.push_back(new Enemy());
	enemies_[0]->Initialize({ 3.0f, 0.0f, 5.0f }, 3.0f, 1.0f);
	enemies_.push_back(new Enemy());
	enemies_[1]->Initialize({ -4.0f, 0.0f, 10.0f }, 2.0f, -1.0f);

	// ---- ゲームシステムの目標（コイン）----
	coins_.push_back(new Coin());
	coins_[0]->Initialize({ -2.0f, 1.0f, 4.0f });
	coins_.push_back(new Coin());
	coins_[1]->Initialize({ 2.0f, 1.0f, 8.0f });
	coins_.push_back(new Coin());
	coins_[2]->Initialize({ 0.0f, 1.0f, 12.0f });

	particleManager_ = new ParticleManager();
	particleManager_->Initialize();

	// フェード用オーバーレイ（2Dスプライト）
	blackTextureHandle_ = TextureManager::GetInstance()->Load("white1x1.png");
	fadeSprite_ = Sprite::Create(blackTextureHandle_, { 0.0f, 0.0f });
	fadeSprite_->SetSize({ 1280.0f, 720.0f });

	// タイトル画像（2Dスプライト）
	// 用意した画像に差し替える場所
	titleTextureHandle_ = TextureManager::GetInstance()->Load("title.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, { 0.0f, 0.0f });
	titleSprite_->SetSize({ 1280.0f, 720.0f });
}

void GameScene::Update() {
	Input* input = Input::GetInstance();

	// ---- ポーズメニュー ----
	if (scene_ == Scene::kPlay && input->TriggerKey(DIK_ESCAPE)) {
		isPaused_ = !isPaused_;
	}

	switch (scene_) {
	case Scene::kTitle:
		UpdateTitle();
		break;
	case Scene::kStart:
		UpdateStart();
		break;
	case Scene::kPlay:
		if (!isPaused_) {
			UpdatePlay();
		}
		break;
	case Scene::kClear:
	case Scene::kGameOver:
		if (input->TriggerKey(DIK_R)) {
			// 簡易リトライ（本来はシーンの再初期化を行う）
			scene_ = Scene::kTitle;
			fade_ = 1.0f;
		}
		break;
	}
}

void GameScene::UpdateTitle() {
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_RETURN)) {
		// ---- タイトル→スタート演出への画面遷移 ----
		scene_ = Scene::kStart;
		startTimer_ = 60;
		fade_ = 1.0f;
		// Audio::GetInstance()->PlayWave(bgmHandle_, true, &bgmVoiceHandle_);
	}
}

void GameScene::UpdateStart() {
	// ---- スタート演出：フェードインしながらカウントダウン ----
	startTimer_--;
	if (fade_ > 0.0f) {
		fade_ -= 0.02f;
	}
	if (startTimer_ <= 0) {
		scene_ = Scene::kPlay;
		fade_ = 0.0f;
	}
}

void GameScene::UpdatePlay() {
	player_->Update();

	// ---- カメラワーク：プレイヤーに追従（Z方向のみ）----
	camera_.translation_.x = player_->GetPosition().x * 0.3f;
	camera_.translation_.z = player_->GetPosition().z - 20.0f;
	camera_.UpdateMatrix();

	for (auto* enemy : enemies_) {
		enemy->Update();

		if (!enemy->IsAlive()) {
			continue;
		}

		Vector3 pp = player_->GetPosition();
		Vector3 ep = enemy->GetPosition();
		float dx = pp.x - ep.x;
		float dz = pp.z - ep.z;
		float distSq = dx * dx + dz * dz;

		// ---- プレイヤーの攻撃が当たったら敵を倒す（フィードバック）----
		if (player_->IsAttacking() && distSq < 4.0f) {
			enemy->Kill();
			particleManager_->Spawn(ep);
			// Audio::GetInstance()->PlayWave(seHitHandle_);
		}
		// ---- 敵に接触したらダメージ ----
		else if (distSq < 1.5f) {
			player_->TakeDamage();
			particleManager_->Spawn(pp);
			enemy->PushBack();
			if (player_->IsDead()) {
				scene_ = Scene::kGameOver;
				fade_ = 0.0f;
			}
		}
	}

	// ---- コイン取得判定（ゲームシステムの目標）----
	bool allGot = true;
	for (auto* coin : coins_) {
		coin->Update();

		if (!coin->IsGot()) {
			Vector3 pp = player_->GetPosition();
			Vector3 cp = coin->GetPosition();
			float dx = pp.x - cp.x;
			float dz = pp.z - cp.z;
			if (dx * dx + dz * dz < 1.5f) {
				coin->Get();
				particleManager_->Spawn(cp);
			}
		}
		if (!coin->IsGot()) {
			allGot = false;
		}
	}

	// ---- クリア演出への画面遷移 ----
	if (allGot) {
		scene_ = Scene::kClear;
		// Audio::GetInstance()->PlayWave(seClearHandle_);
	}

	particleManager_->Update();
}

void GameScene::Draw() {
	if (scene_ == Scene::kTitle) {
		// ---- タイトルシーン ----
		titleSprite_->Draw();
	}
	else {
		player_->Draw(camera_);
		for (auto* enemy : enemies_) {
			enemy->Draw(camera_);
		}
		for (auto* coin : coins_) {
			coin->Draw(camera_);
		}
		particleManager_->Draw(camera_);
	}

	// ---- 画面遷移フェード ----
	if (fade_ > 0.0f) {
		fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, fade_ });
		fadeSprite_->Draw();
	}
}