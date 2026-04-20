#include "GameScene.h"
#include "2d/ImGuiManager.h"
using namespace KamataEngine;


void GameScene::Initialize() {
	//textureHandle_ = TextureManager::Load("./Resources/mario.png"); 
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});
	textureHandle1_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	//soundDataHandle_ = Audio::GetInstance()->LoadWave("./Resources/fanfare.wav");
	//Audio::GetInstance()->PlayWave(soundDataHandle_);
	//voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	worldTransform_.Initialize();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {

	//Vector2 position = sprite_->GetPosition();
	//sprite_->SetPosition(position);
	debugCamera_->Update();
	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

	//	Audio::GetInstance()->StopWave(voiceHandle_);

	//}
	//debugCamera_->Update();
	//ImGui::Begin("Debug1");
	//ImGui::Text("Kamata Tarou ", 2050, 12, 31);
	//ImGui::InputFloat3("InputFloat3", inputFloat3);
	//ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	//ImGui::End();
}

void GameScene::Draw() {

	//Sprite::PreDraw();

	//sprite_->Draw();

	//Sprite::PostDraw();

	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle1_);
	Model::PostDraw();
}
GameScene::~GameScene() {
	//delete sprite_;   
	delete model_;
	delete debugCamera_;
}

