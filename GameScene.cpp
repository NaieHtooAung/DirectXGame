#include "GameScene.h"
#include "Player.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	player_ = new Player();
	player_->Initialize();
}

void GameScene::Update() {

	player_->Update();

}

void GameScene::Draw() {

	player_->Draw();

}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
}