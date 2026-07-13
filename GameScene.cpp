#include "GameScene.h"
#include "CameraController.h"
#include "Enemy.h"
#include "GuardEffect.h"
#include "HitEffect.h"
#include "MapChipField.h"
#include "Player.h"
#include "ShieldEnemy.h"
#include "mathUti.h"
#include "skydome.h"
#include <cmath>

using namespace KamataEngine;

// =========================
// Initialize
// =========================
void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("./Resources/block/block.png");

	blockModel_ = Model::CreateFromOBJ("block", true);

	camera_.Initialize();

	debugCamera_ = new DebugCamera(1280, 720);

	// debug camera activation flag
	isDebugCameraActive_ = false;

	// MAP FIRST
	mapchipField_ = new MapChipField();

	mapchipField_->LoadMapChipCsv("Resources/blocks.csv");

	phase_ = Phase::kFadeIn;

	// PLAYER
	player_ = new Player();

	textureHandlePlayer_ = TextureManager::Load("./Resources/player/player.png");

	model_ = Model::CreateFromOBJ("player", true);

	textureHandleAttack_ = TextureManager::Load("./Resources/hit_effect/hit_effect.png");

	modelAttack_ = Model::CreateFromOBJ("hit_effect", true);

	deathParticlesModel_ = Model::CreateFromOBJ("player", true);

	deathParticles_ = new DeathParticles();

	Vector3 playerPosition = mapchipField_->GetmapChipPositionByIndex(1, 17);

	player_->Initialize(model_, modelAttack_, textureHandlePlayer_, textureHandleAttack_, &camera_, playerPosition);

	player_->setMapChipField(mapchipField_);

	textureHandleEnemy_ = TextureManager::Load("./Resources/enemy/enemy.png");

	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	for (int32_t i = 0; i < 1; i++) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemysPosition = mapchipField_->GetmapChipPositionByIndex(10 + i * 5, 17);
		newEnemy->Initialize(enemyModel_, textureHandleEnemy_, &camera_, enemysPosition);
		newEnemy->setMapChipField(mapchipField_);
		enemies_.push_back(newEnemy);
	}

	// SHIELD ENEMY (正面から倒せない敵)
	// モデルの差し替え: 専用モデルデータが用意できたら、下の2行を
	// 専用OBJ/テクスチャの読み込みに差し替えること。差し替え前でも
	// 一旦enemyモデルを流用してビルド・実行確認できるようにしてある。
	textureHandleShieldEnemy_ = TextureManager::Load("./Resources/shieldEnemy/shieldEnemy.png");
	shieldEnemyModel_ = Model::CreateFromOBJ("shieldEnemy", true);

	for (int32_t i = 0; i < 1; i++) {
		ShieldEnemy* newShieldEnemy = new ShieldEnemy();
		Vector3 shieldEnemyPosition = mapchipField_->GetmapChipPositionByIndex(20 + i * 5, 17);
		newShieldEnemy->Initialize(shieldEnemyModel_, textureHandleShieldEnemy_, &camera_, shieldEnemyPosition);
		newShieldEnemy->setMapChipField(mapchipField_);
		shieldEnemies_.push_back(newShieldEnemy);
	}

	// CAMERA CONTROLLER
	cameraController_ = new CameraController();

	cameraController_->Initialize(&camera_, player_);

	CameraController::Rect movableArea = {
	    0.0f,   // left
	    100.0f, // top
	    100.0f, // right
	    0.0f    // bottom
	};

	cameraController_->SetMovableArea(movableArea);

	// SKYDOME
	skydome_ = new skydome();

	skydome_->initialize();

	camera_.farZ = 1000.0f;

	camera_.UpdateMatrix();

	GenerateBlocks();

	// FADE
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	// HIT EFFECT (white dot burst on enemy defeat)
	// NOTE: hitEffects_ is a std::list<HitEffect*> MEMBER declared in GameScene.h.
	// Do not redeclare it locally here - that would shadow the member and never get used.
	textureHandleHitEffect_ = TextureManager::Load("./Resources/deathParticle/white1x1.png");
	hitEffectmodel_ = Model::CreateFromOBJ("deathParticle", true);
	HitEffect::SetModel(hitEffectmodel_);
	HitEffect::SetCamera(&camera_);

	// GUARD EFFECT (burst played when a ShieldEnemy guards an attack)
	// 手順はHitEffectとほぼ同じ。専用テクスチャが用意できたら差し替えること。
	textureHandleGuardEffect_ = TextureManager::Load("./Resources/deathParticle/white1x1.png");
	guardEffectModel_ = Model::CreateFromOBJ("deathParticle", true);
	GuardEffect::SetModel(guardEffectModel_);
	GuardEffect::SetCamera(&camera_);
}

// =========================
// Update
// =========================
void GameScene::Update() {

	// フェード更新
	fade_->Update();

	switch (phase_) {

	case Phase::kFadeIn:
		// フェードイン完了でゲーム開始
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		break;

	case Phase::kPlay:

		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->update();
		}
		for (ShieldEnemy* shieldEnemy : shieldEnemies_) {
			shieldEnemy->update();
		}

		// if player died
		if (deathParticles_ && deathParticles_->isInitialized_ && !deathParticles_->IsFinished()) {
			phase_ = Phase::kDeath;
		}

		break;

	case Phase::kDeath:
		// 死亡パーティクル終了後にフェードアウト開始
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}

		break;

	case Phase::kFadeOut:
		// フェードアウト完了でシーン終了
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	player_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->update();
	}
	for (ShieldEnemy* shieldEnemy : shieldEnemies_) {
		shieldEnemy->update();
	}
	if (deathParticles_) {
		deathParticles_->Update();
	}
	skydome_->update();
	cameraController_->Update();
	CheckAllCollisions();

	// Remove enemies whose defeat (spin+shrink) animation has finished.
	enemies_.remove_if([this](Enemy* enemy) {
		if (enemy->IsDefeatAnimationFinished()) {
			delete enemy;
			finished_ = true;
			return true;
		}
		return false;
	});

	// Remove shield enemies whose defeat animation has finished (only reached
	// if they were hit from a non-guardable angle).
	shieldEnemies_.remove_if([this](ShieldEnemy* shieldEnemy) {
		if (shieldEnemy->IsDefeatAnimationFinished()) {
			delete shieldEnemy;
			return true;
		}
		return false;
	});

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			// Scale
			worldTransformBlock->scale_ = {1.0f, 1.0f, 1.0f};

			// Rotation
			worldTransformBlock->rotation_.y = 0.00f;

			// World Matrix
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// Transfer
			worldTransformBlock->TransferMatrix();
		}
	}

	// Update all active hit effects, then remove any whose デス状態 (dead state) has been reached.
	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Update();
	}
	hitEffects_.remove_if([](HitEffect* hitEffect) {
		if (hitEffect->IsDead()) {
			delete hitEffect;
			return true;
		}
		return false;
	});

	// Update all active guard effects, then remove any whose デス状態 has been reached.
	for (GuardEffect* guardEffect : guardEffects_) {
		guardEffect->Update();
	}
	guardEffects_.remove_if([](GuardEffect* guardEffect) {
		if (guardEffect->IsDead()) {
			delete guardEffect;
			return true;
		}
		return false;
	});

#ifdef DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		if (isDebugCameraActive_) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		} else {
			isDebugCameraActive_ = true;
		}
	}

#endif
	debugCamera_->Update();
	if (isDebugCameraActive_) {

		camera_.matView = debugCamera_->GetCamera().matView;

		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();
	} else {

		camera_.UpdateMatrix();
	}
}

// =========================
// Draw
// =========================
void GameScene::Draw() {
	Model::PreDraw();

	skydome_->Draw(camera_);
	player_->Draw();
	if (deathParticles_) {
		deathParticles_->Draw();
	}
	for (Enemy* enemy : enemies_) {
		enemy->draw();
	}
	for (ShieldEnemy* shieldEnemy : shieldEnemies_) {
		shieldEnemy->draw();
	}

	// Draw all active hit effects.
	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Draw();
	}

	// Draw all active guard effects.
	for (GuardEffect* guardEffect : guardEffects_) {
		guardEffect->Draw();
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			if (!blockModel_)
				continue;
			blockModel_->Draw(*worldTransformBlock, camera_, textureHandle_);
		}
	}

	Model::PostDraw();

	// フェードを最前面に描画
	fade_->Draw();
}

void GameScene::GenerateBlocks() {

	uint32_t numBlockVirtical = mapchipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapchipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {

		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; i++) {

		for (uint32_t j = 0; j < numBlockHorizontal; j++) {

			if (mapchipField_->GetmapChipTypeByIndex(j, i) != MapChipType::kBlock) {
				continue;
			}

			worldTransformBlocks_[i][j] = new WorldTransform();

			worldTransformBlocks_[i][j]->Initialize();

			Vector3 blockPosition = mapchipField_->GetmapChipPositionByIndex(j, i);

			worldTransformBlocks_[i][j]->translation_ = blockPosition;
		}
	}
}

void GameScene::CheckAllCollisions() {
	Player::AABB aabb1;
	Enemy::AABB aabb2;
	ShieldEnemy::AABB aabb3;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_) {
		// Already dying — skip so it can't be re-triggered or block the player.
		if (enemy->IsDefeated()) {
			continue;
		}

		aabb2 = enemy->GetAABB();
		if (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x && aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) {
			player_->onCollision(enemy);

			if (player_->IsAttacking()) {
				// Player is dashing through the enemy: defeat it instead of dying.
				enemy->OnDefeated();
				// Static factory: allocates + initializes a new HitEffect, added to the active list.
				hitEffects_.push_back(HitEffect::Create(textureHandleHitEffect_, enemy->GetWorldPosition()));
			} else if (deathParticles_->IsFinished() || !deathParticles_->isInitialized_) {
				Vector3 pos = player_->GetWorldPosition();
				deathParticles_->Initialize(deathParticlesModel_, textureHandlePlayer_, &camera_, pos);
			}
		}
	}

	for (ShieldEnemy* shieldEnemy : shieldEnemies_) {
		// Already dying — skip so it can't be re-triggered or block the player.
		if (shieldEnemy->IsDefeated()) {
			continue;
		}

		aabb3 = shieldEnemy->GetAABB();
		if (aabb1.min.x <= aabb3.max.x && aabb1.max.x >= aabb3.min.x && aabb1.min.y <= aabb3.max.y && aabb1.max.y >= aabb3.min.y) {
			// プレイヤー側の応答: 攻撃中(ダッシュ中)は無敵で素通り、
			// それ以外の時に触れたら死ぬ。ガードされるかどうかはこの結果に影響しない。
			player_->onCollision(shieldEnemy);

			// 敵側の応答: 攻撃中でなければ何もしない。攻撃中なら、正面からの
			// 攻撃はガード(デス回避+ノックバック要求)、それ以外はデス演出へ。
			// NOTE: playerはconstポインタではない。ShieldEnemy::onCollision内で
			// player->RequestKnockback()を呼びプレイヤー側のフラグを書き換えるため。
			shieldEnemy->onCollision(player_);

			// ガードが成功した直後であれば、GuardEffectを1回だけ生成する。
			if (shieldEnemy->ConsumeGuardEffectRequest()) {
				guardEffects_.push_back(GuardEffect::Create(textureHandleGuardEffect_, shieldEnemy->GetWorldPosition()));
			}
		}
	}
}

// =========================
// Destructor
// =========================
GameScene::~GameScene() {

	delete blockModel_;
	delete model_;
	delete modelAttack_;
	delete debugCamera_;
	delete player_;
	delete skydome_;
	delete deathParticles_;
	delete deathParticlesModel_;
	delete fade_;
	delete hitEffectmodel_;
	delete guardEffectModel_;
	delete shieldEnemyModel_;
	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}
	for (GuardEffect* guardEffect : guardEffects_) {
		delete guardEffect;
	}
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (ShieldEnemy* shieldEnemy : shieldEnemies_) {
		delete shieldEnemy;
	}
	delete mapchipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}