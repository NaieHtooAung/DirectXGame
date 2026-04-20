#include "GameScene.h"

using namespace KamataEngine;


void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png"); 
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	textureHandle1_ = TextureManager::Load("./Resources/mario.jpg");
	soundDataHandle_ = Audio::GetInstance()->LoadWave("./Resources/fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	model_ = Model::Create();
	worldTransform_.Initialize();
	camera_.Initialize();
}

void GameScene::Update() {

	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		Audio::GetInstance()->StopWave(voiceHandle_);

	}
}

void GameScene::Draw() {

	Sprite::PreDraw();

	sprite_->Draw();
	
	Sprite::PostDraw();
	
	Model::PreDraw();
	model_->Draw(worldTransform_,camera_,textureHandle1_);
	Model::PostDraw();

}
GameScene::~GameScene() {
	delete sprite_;   
	delete model_;
}

