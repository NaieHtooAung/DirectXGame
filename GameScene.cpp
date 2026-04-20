#include "GameScene.h"

using namespace KamataEngine;


void GameScene::Initialize() {
	
	textureHandle1_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
}

void GameScene::Update() {


	
}

void GameScene::Draw() {

	Model::PreDraw();

	model_->Draw(worldTransform_,camera_,textureHandle1_);

	Model::PostDraw();
	

}
GameScene::~GameScene() {
	  
	delete model_;
}

