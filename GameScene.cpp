#include "GameScene.h"
#include "2d/ImGuiManager.h"
using namespace KamataEngine;


void GameScene::Initialize() {

	textureHandle1_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	
	worldTransform_.Initialize();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {

	
	debugCamera_->Update();
	
}

void GameScene::Draw() {

	

	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle1_);
	Model::PostDraw();
}
GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;
}

