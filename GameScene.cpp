#include "GameScene.h"
#include "2d/ImGuiManager.h"
using namespace KamataEngine;


void GameScene::Initialize() {

	textureHandle1_ = TextureManager::Load("./Resources/mario.png");
	model_ = Model::Create();
	textureHandle2_ = TextureManager::Load("./Resources/uvChecker.png");
	sprite_ = Sprite::Create(textureHandle2_, {100, 50});
	soundDataHandle_ = Audio::GetInstance()->LoadWave("./Resources/fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_,true);
	worldTransform_.Initialize();
	camera_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
	PrimitiveDrawer::GetInstance()->SetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {

	
	debugCamera_->Update();
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
#ifndef  _DEBUG
	ImGui::ShowDemoWindow();
	ImGui::Begin("Debug1");
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("sliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
#endif // ! _DEBUG

	
}

void GameScene::Draw() {

	Sprite::PreDraw();
	sprite_->Draw();
	Sprite::PostDraw();

	Model::PreDraw();
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle1_);

	Model::PostDraw();

	const int size = 64;

	for (int x = 0; x <= size; x++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({(float)x, 0, 0}, {(float)x, 0, (float)size}, {1, 0, 0, 1});
	}

	for (int z = 0; z <= size; z++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, (float)z}, {(float)size, 0, (float)z}, {0, 0, 1, 1});
	}
}
	GameScene::~GameScene() {

	delete sprite_;
	delete model_;
	delete debugCamera_;
}

