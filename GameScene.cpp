#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
}

void GameScene::Update() {}

void GameScene::Draw() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}