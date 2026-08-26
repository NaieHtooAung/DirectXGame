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
	delete explanationSprite_;
	delete gameClearSprite_;
	delete gameOverSprite_;
	delete endSprite_;
	delete ground_;
	delete skyDome_;
	delete pressEnterText_;
	delete pressRText_;
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

	// ---- 浮遊する3D文字 ----
// Resources/pressEnter/pressEnter.obj, Resources/pressR/pressR.obj を用意する場所
	pressEnterText_ = new PressEnterText();
	pressEnterText_->Initialize({ 0.0f, -7.5f, 0.0f });

	pressRText_ = new PressRText();
	pressRText_->Initialize({ 0.0f, -5.5f, 0.0f });

	// ---- タイトル画像（2Dスプライト）：不透明で表示し、Press Enterの3D文字を上に重ねる ----
	// 用意した画像に差し替える場所
	titleTextureHandle_ = TextureManager::GetInstance()->Load("./Resources/images/CoinCollector.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, { 0.0f, 0.0f });
	titleSprite_->SetSize({ 1280.0f, 720.0f });
	titleSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 不透明（ぼかし無し）

	// ---- 説明画面（タイトルとゲームの間）用スプライト ----
	// 用意した説明用画像に差し替える場所。今は仮でsample.pngを使い回している
	explanationTextureHandle_ = TextureManager::GetInstance()->Load("./Resources/images/Explanation.png");
	explanationSprite_ = Sprite::Create(explanationTextureHandle_, { 0.0f, 0.0f });
	explanationSprite_->SetSize({ 1280.0f, 720.0f });
	explanationSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	// ---- フェード用オーバーレイ（2Dスプライト）----
	blackTextureHandle_ = TextureManager::GetInstance()->Load("white1x1.png");
	fadeSprite_ = Sprite::Create(blackTextureHandle_, { 0.0f, 0.0f });
	fadeSprite_->SetSize({ 1280.0f, 720.0f });

	// ---- 結果画面画像（クリア／ゲームオーバー）：不透明で表示し、Press Rの3D文字を上に重ねる ----
	// 用意した画像に差し替える場所。今は仮でsample.pngを使い回している
	gameClearTextureHandle_ = TextureManager::GetInstance()->Load("./Resources/images/Win.png");
	gameClearSprite_ = Sprite::Create(gameClearTextureHandle_, { 0.0f, 0.0f });
	gameClearSprite_->SetSize({ 1280.0f, 720.0f });
	gameClearSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	gameOverTextureHandle_ = TextureManager::GetInstance()->Load("./Resources/images/Lose.png");
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, { 0.0f, 0.0f });
	gameOverSprite_->SetSize({ 1280.0f, 720.0f });
	gameOverSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	// エンド画面用オーバーレイ（クリア/ゲームオーバー/ステージクリア/ポーズの色分けに流用）
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

// ---- フェード遷移を開始する：黒くなった瞬間にシーンを切り替え、onCompleteを1回だけ実行する ----
void GameScene::StartTransition(Scene target, std::function<void()> onComplete) {
	isFading_ = true;
	fadingOut_ = true;
	fadeTargetScene_ = target;
	onFadeComplete_ = onComplete;
}

void GameScene::UpdateTransition() {
	if (fadingOut_) {
		// ---- 黒くしていく途中 ----
		fade_ += kFadeSpeed_;
		if (fade_ >= 1.0f) {
			fade_ = 1.0f;
			scene_ = fadeTargetScene_;
			if (onFadeComplete_) {
				onFadeComplete_();
				onFadeComplete_ = nullptr;
			}
			fadingOut_ = false; // 次のUpdateから黒→見える側にフェードしていく
		}
	}
	else {
		// ---- 黒から戻していく途中 ----
		fade_ -= kFadeSpeed_;
		if (fade_ <= 0.0f) {
			fade_ = 0.0f;
			isFading_ = false;
		}
	}
}

void GameScene::Update() {
	Input* input = Input::GetInstance();

	// ---- ポーズメニュー（フェード中は無効）：ESCまたはPキーでトグル ----
	if (scene_ == Scene::kGame && !isFading_ &&
		(input->TriggerKey(DIK_ESCAPE) || input->TriggerKey(DIK_P))) {
		isPaused_ = !isPaused_;
	}

	if (isFading_) {
		UpdateTransition();
	}
	else {
		switch (scene_) {
		case Scene::kStart:
			UpdateStart();
			break;
		case Scene::kExplanation:
			UpdateExplanation();
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

	// ---- 浮遊する3D文字はフェード中も含めて常にアニメーションさせる ----
	// ---- "Press Enter"はタイトルと説明画面の両方で表示するので両方で更新する ----
	if (scene_ == Scene::kStart || scene_ == Scene::kExplanation) {
		pressEnterText_->Update();
	}
	if (scene_ == Scene::kEnd) {
		pressRText_->Update();
	}
}

void GameScene::UpdateStart() {
	Input* input = Input::GetInstance();

	// ---- タイトル表示中：Enterで説明画面へフェード ----
	if (input->TriggerKey(DIK_RETURN)) {
		StartTransition(Scene::kExplanation);
	}
}

void GameScene::UpdateExplanation() {
	Input* input = Input::GetInstance();

	// ---- 説明画面：Enterでゲーム本編へフェード ----
	if (input->TriggerKey(DIK_RETURN)) {
		StartTransition(Scene::kGame);
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
				// ---- 負け：カメラを固定位置に戻してからフェードアウトし、黒くなった瞬間にエンド画面へ切り替える ----
				won_ = false;
				isPaused_ = false;
				camera_.translation_ = { 0.0f, 5.0f, -20.0f };
				camera_.UpdateMatrix();
				StartTransition(Scene::kEnd);
				return; // このフレームはこれ以上ゲーム更新をしない
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
			// ---- 勝ち：カメラを固定位置に戻してからフェードアウトし、黒くなった瞬間にエンド画面へ切り替える ----
			won_ = true;
			isPaused_ = false;
			camera_.translation_ = { 0.0f, 5.0f, -20.0f };
			camera_.UpdateMatrix();
			StartTransition(Scene::kEnd);
			// Audio::GetInstance()->PlayWave(seClearHandle_);
			return;
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

	// ---- リトライ待ち→ゲームのループ：Rキーでフェードしてリセット再開 ----
	if (input->TriggerKey(DIK_R)) {
		StartTransition(Scene::kGame, [this]() { Reset(); });
	}
}

void GameScene::Reset() {
	player_->Reset();
	LoadStage(1);
	camera_.translation_ = { 0.0f, 5.0f, -20.0f };
	isPaused_ = false;
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ---- スカイドームは常に一番奥に描画（カリング無効）----
	Model::PreDraw(Model::CullingMode::kNone);
	skyDome_->Draw(camera_);
	Model::PostDraw();

	if (scene_ == Scene::kStart) {
		// ---- タイトル画像を先に不透明で描画（ぼかし無し）----
		Sprite::PreDraw(dxCommon->GetCommandList());
		titleSprite_->Draw();
		Sprite::PostDraw();

		// ---- 深度バッファをクリアしてから"Press Enter"の3D文字を描画：画像に隠れず必ず上に見える ----
		dxCommon->ClearDepthBuffer();
		Model::PreDraw();
		pressEnterText_->Draw(camera_);
		Model::PostDraw();
	}
	else if (scene_ == Scene::kExplanation) {
		// ---- 説明画面：画像を不透明で表示 ----
		Sprite::PreDraw(dxCommon->GetCommandList());
		explanationSprite_->Draw();
		Sprite::PostDraw();

		// ---- 深度バッファをクリアしてから"Press Enter"の3D文字を描画：画像に隠れず必ず上に見える ----
		dxCommon->ClearDepthBuffer();
		Model::PreDraw();
		pressEnterText_->Draw(camera_);
		Model::PostDraw();
	}
	else {
		// ---- ゲーム世界の描画（ステージクリア／エンドの背景としても表示）----
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

		// ---- ポーズ中：半透明の暗いオーバーレイを表示 ----
		if (scene_ == Scene::kGame && isPaused_) {
			Sprite::PreDraw(dxCommon->GetCommandList());
			endSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.5f });
			endSprite_->Draw();
			Sprite::PostDraw();
		}

		if (scene_ == Scene::kStageClear) {
			// ---- ステージクリア演出：金色の半透明オーバーレイ ----
			Sprite::PreDraw(dxCommon->GetCommandList());
			endSprite_->SetColor({ 1.0f, 0.85f, 0.0f, 0.35f });
			endSprite_->Draw();
			Sprite::PostDraw();
		}
		else if (scene_ == Scene::kEnd) {
			// ---- 先に色つきオーバーレイと結果画像を不透明で描画 ----
			Sprite::PreDraw(dxCommon->GetCommandList());
			if (won_) {
				endSprite_->SetColor({ 0.0f, 0.8f, 0.0f, 0.35f });
				endSprite_->Draw();
				gameClearSprite_->Draw();
			}
			else {
				endSprite_->SetColor({ 0.8f, 0.0f, 0.0f, 0.35f });
				endSprite_->Draw();
				gameOverSprite_->Draw();
			}
			Sprite::PostDraw();

			// ---- 深度バッファをクリアしてから"Press R"の3D文字を描画：画像に隠れず必ず上に見える ----
			dxCommon->ClearDepthBuffer();
			Model::PreDraw();
			pressRText_->Draw(camera_);
			Model::PostDraw();
		}
	}

	// ---- 画面遷移フェード：常に最前面 ----
	Sprite::PreDraw(dxCommon->GetCommandList());
	if (fade_ > 0.0f) {
		fadeSprite_->SetColor({ 0.0f, 0.0f, 0.0f, fade_ });
		fadeSprite_->Draw();
	}
	Sprite::PostDraw();
}