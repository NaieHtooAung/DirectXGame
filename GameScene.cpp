#include "GameScene.h"

GameScene::~GameScene() {
	delete player_;
	for (auto* e : enemies_) {
		delete e;
	}
	for (auto* c : coins_) {
		delete c;
	}
	delete coinParticle_;
	delete enemyParticle_;
	delete fadeSprite_;
	delete titleSprite_;
	delete endSprite_;
	delete ground_;
	delete skyDome_;
}

void GameScene::Initialize() {
	// ---- 3Dカメラの初期化（プレイヤーの少し後ろ上から見下ろす）----

	Audio* audio = Audio::GetInstance();
	bgmSoundHandle_ = audio->LoadWave("./Resources/sound/game_bgm.mp3");
	bgmVoiceHandle_ = audio->PlayWave(bgmSoundHandle_, true); // 第2引数true = ループ再生

	camera_.Initialize();
	camera_.translation_ = { 0.0f, 5.0f, -20.0f };
	camera_.rotation_.x = 0.3f;

	player_ = new Player();
	player_->Initialize();

	// ---- パーティクル：コイン用と敵用でモデルを分けて初期化 ----
	coinParticle_ = new ParticleManager();
	coinParticle_->Initialize("star"); // 後でコイン用パーティクルモデルに差し替える場所

	enemyParticle_ = new ParticleManager();
	enemyParticle_->Initialize("particle"); // 後で敵用パーティクルモデルに差し替える場所

	ground_ = new Ground();
	ground_->Initialize();

	skyDome_ = new SkyDome();
	skyDome_->Initialize();

	// フェード用オーバーレイ（2Dスプライト）
	blackTextureHandle_ = TextureManager::GetInstance()->Load("white1x1.png");
	fadeSprite_ = Sprite::Create(blackTextureHandle_, { 0.0f, 0.0f });
	fadeSprite_->SetSize({ 1280.0f, 720.0f });

	// タイトル画像（2Dスプライト）
	// 用意した画像に差し替える場所
	titleTextureHandle_ = TextureManager::GetInstance()->Load("sample.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, { 0.0f, 0.0f });
	titleSprite_->SetSize({ 1280.0f, 720.0f });

	// エンド画面用オーバーレイ（クリア/ゲームオーバー/ステージクリアの色分けに流用）
	endSprite_ = Sprite::Create(blackTextureHandle_, { 0.0f, 0.0f });
	endSprite_->SetSize({ 1280.0f, 720.0f });

	// ---- 1ステージ目を読み込む ----
	currentStage_ = 1;
	LoadStage(currentStage_);
}

void GameScene::ClearStageObjects() {
	for (auto* e : enemies_) {
		delete e;
	}
	enemies_.clear();
	for (auto* c : coins_) {
		delete c;
	}
	coins_.clear();
}

void GameScene::LoadStage(int32_t stageNumber) {
	ClearStageObjects();
	currentStage_ = stageNumber;

	switch (stageNumber) {
	case 1:
		// ---- ステージ1：まずは基本を覚える難易度 ----
		enemies_.push_back(new Enemy());
		enemies_[0]->Initialize({ 3.0f, 0.0f, 5.0f }, 3.0f, 1.0f, 0.05f);
		enemies_.push_back(new Enemy());
		enemies_[1]->Initialize({ -4.0f, 0.0f, 10.0f }, 2.0f, -1.0f, 0.05f);

		coins_.push_back(new Coin());
		coins_[0]->Initialize({ -2.0f, 1.0f, 4.0f });
		coins_.push_back(new Coin());
		coins_[1]->Initialize({ 2.0f, 1.0f, 8.0f });
		coins_.push_back(new Coin());
		coins_[2]->Initialize({ 0.0f, 1.0f, 12.0f });
		break;

	case 2:
		// ---- ステージ2：敵の数と速度アップ、コインまでの距離も伸ばす ----
		enemies_.push_back(new Enemy());
		enemies_[0]->Initialize({ 4.0f, 0.0f, 6.0f }, 4.0f, 1.0f, 0.08f);
		enemies_.push_back(new Enemy());
		enemies_[1]->Initialize({ -4.0f, 0.0f, 10.0f }, 3.0f, -1.0f, 0.08f);
		enemies_.push_back(new Enemy());
		enemies_[2]->Initialize({ 2.0f, 0.0f, 14.0f }, 3.0f, 1.0f, 0.09f);
		enemies_.push_back(new Enemy());
		enemies_[3]->Initialize({ -3.0f, 0.0f, 18.0f }, 2.0f, -1.0f, 0.09f);

		coins_.push_back(new Coin());
		coins_[0]->Initialize({ -3.0f, 1.0f, 5.0f });
		coins_.push_back(new Coin());
		coins_[1]->Initialize({ 3.0f, 1.0f, 9.0f });
		coins_.push_back(new Coin());
		coins_[2]->Initialize({ -2.0f, 1.0f, 14.0f });
		coins_.push_back(new Coin());
		coins_[3]->Initialize({ 2.0f, 1.0f, 18.0f });
		break;

	case 3:
	default:
		// ---- ステージ3：敵が多く速い。広い範囲を往復する敵もいて、
		//      コインを取るには敵の隙間を縫って進むタイミングが必要 ----
		enemies_.push_back(new Enemy());
		enemies_[0]->Initialize({ 0.0f, 0.0f, 6.0f }, 5.0f, 1.0f, 0.12f);
		enemies_.push_back(new Enemy());
		enemies_[1]->Initialize({ -5.0f, 0.0f, 9.0f }, 3.0f, -1.0f, 0.11f);
		enemies_.push_back(new Enemy());
		enemies_[2]->Initialize({ 5.0f, 0.0f, 12.0f }, 3.0f, 1.0f, 0.11f);
		enemies_.push_back(new Enemy());
		enemies_[3]->Initialize({ -4.0f, 0.0f, 16.0f }, 4.0f, -1.0f, 0.13f);
		enemies_.push_back(new Enemy());
		enemies_[4]->Initialize({ 4.0f, 0.0f, 20.0f }, 4.0f, 1.0f, 0.13f);
		enemies_.push_back(new Enemy());
		enemies_[5]->Initialize({ 0.0f, 0.0f, 24.0f }, 6.0f, -1.0f, 0.15f);

		coins_.push_back(new Coin());
		coins_[0]->Initialize({ -4.0f, 1.0f, 5.0f });
		coins_.push_back(new Coin());
		coins_[1]->Initialize({ 4.0f, 1.0f, 9.0f });
		coins_.push_back(new Coin());
		coins_[2]->Initialize({ -3.0f, 1.0f, 14.0f });
		coins_.push_back(new Coin());
		coins_[3]->Initialize({ 3.0f, 1.0f, 19.0f });
		coins_.push_back(new Coin());
		coins_[4]->Initialize({ 0.0f, 1.0f, 25.0f });
		break;
	}
}

void GameScene::Update() {
	Input* input = Input::GetInstance();

	// ---- ポーズメニュー ----
	if (scene_ == Scene::kGame && input->TriggerKey(DIK_ESCAPE)) {
		isPaused_ = !isPaused_;
	}

	switch (scene_) {
	case Scene::kStart:
		UpdateStart();
		break;
	case Scene::kGame:
		if (!isPaused_) {
			UpdateGame();
		}
		break;
	case Scene::kStageClear:
		UpdateStageClear();
		break;
	case Scene::kEnd:
		UpdateEnd();
		break;
	}
}

void GameScene::UpdateStart() {
	Input* input = Input::GetInstance();

	if (!isCountingDown_) {
		// ---- タイトル表示中：Enterでスタート演出へ ----
		if (input->TriggerKey(DIK_RETURN)) {
			isCountingDown_ = true;
			startTimer_ = 60;
			fade_ = 1.0f;
			// Audio::GetInstance()->PlayWave(bgmHandle_, true, &bgmVoiceHandle_);
		}
		return;
	}

	// ---- スタート演出：フェードインしながらカウントダウン ----
	startTimer_--;
	if (fade_ > 0.0f) {
		fade_ -= 0.02f;
	}
	if (startTimer_ <= 0) {
		scene_ = Scene::kGame;
		isCountingDown_ = false;
		fade_ = 0.0f;
	}
}

void GameScene::UpdateGame() {
	player_->Update();

	// ---- カメラワーク：プレイヤーに追従（Z方向のみ）----
	camera_.translation_.x = player_->GetPosition().x * 0.3f;
	camera_.translation_.z = player_->GetPosition().z - 20.0f;
	camera_.UpdateMatrix();

	skyDome_->Update(camera_);

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
			enemyParticle_->Spawn(ep);
			// Audio::GetInstance()->PlayWave(seHitHandle_);
		}
		// ---- 敵に接触したらダメージ ----
		else if (distSq < 1.5f) {
			player_->TakeDamage();
			enemyParticle_->Spawn(pp);
			enemy->PushBack();
			if (player_->IsDead()) {
				// ---- ゲーム→エンド（負け）----
				won_ = false;
				scene_ = Scene::kEnd;
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
				coinParticle_->Spawn(cp);
			}
		}
		if (!coin->IsGot()) {
			allGot = false;
		}
	}

	// ---- 全コイン取得：次のステージへ（最終ステージなら本当のクリア）----
	if (allGot) {
		if (currentStage_ < kMaxStage_) {
			scene_ = Scene::kStageClear;
			stageClearTimer_ = kStageClearDuration_;
		}
		else {
			won_ = true;
			scene_ = Scene::kEnd;
			// Audio::GetInstance()->PlayWave(seClearHandle_);
		}
	}

	coinParticle_->Update();
	enemyParticle_->Update();
}

void GameScene::UpdateStageClear() {
	// ---- ステージクリア演出：少し待ってから次のステージを読み込む ----
	stageClearTimer_--;
	if (stageClearTimer_ <= 0) {
		LoadStage(currentStage_ + 1);
		player_->Reset();
		camera_.translation_ = { 0.0f, 5.0f, -20.0f };
		scene_ = Scene::kGame;
	}
}

void GameScene::UpdateEnd() {
	Input* input = Input::GetInstance();

	// ---- エンド→ゲームのループ：Rキーでリセットして再開 ----
	if (input->TriggerKey(DIK_R)) {
		Reset();
		scene_ = Scene::kGame;
	}
}

void GameScene::Reset() {
	player_->Reset();
	LoadStage(1);
	camera_.translation_ = { 0.0f, 5.0f, -20.0f };
	fade_ = 0.0f;
	stageClearTimer_ = 0;
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ---- 3Dモデルの描画（先にPreDraw/PostDrawで挟む）----
	if (scene_ != Scene::kStart) {
		// スカイドームはカメラの内側から見るので、カリングを無効にして描画する
		Model::PreDraw(Model::CullingMode::kNone);
		skyDome_->Draw(camera_);
		Model::PostDraw();

		Model::PreDraw();
		ground_->Draw(camera_);
		player_->Draw(camera_);
		for (auto* enemy : enemies_) {
			enemy->Draw(camera_);
		}
		for (auto* coin : coins_) {
			coin->Draw(camera_);
		}
		coinParticle_->Draw(camera_);
		enemyParticle_->Draw(camera_);
		Model::PostDraw();
	}

	// ---- 2Dスプライトの描画（別でPreDraw/PostDrawが必要）----
	Sprite::PreDraw(dxCommon->GetCommandList());
	if (scene_ == Scene::kStart) {
		// ---- タイトルシーン ----
		titleSprite_->Draw();
	}
	if (scene_ == Scene::kEnd) {
		// ---- エンドシーン：クリア=緑、ゲームオーバー=赤の半透明オーバーレイ ----
		if (won_) {
			endSprite_->SetColor({ 0.0f, 0.8f, 0.0f, 0.5f });
		}
		else {
			endSprite_->SetColor({ 0.8f, 0.0f, 0.0f, 0.5f });
		}
		endSprite_->Draw();
	}
	if (scene_ == Scene::kStageClear) {
		// ---- ステージクリア演出：金色の半透明オーバーレイを一瞬挟む ----
		endSprite_->SetColor({ 1.0f, 0.85f, 0.0f, 0.35f });
		endSprite_->Draw();
	}
	// ---- 画面遷移フェード ----
	if (fade_ > 0.0f) {
		fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, fade_ });
		fadeSprite_->Draw();
	}
	Sprite::PostDraw();
}