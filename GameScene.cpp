#include "GameScene.h"

using namespace KamataEngine;

void GameScene::Initialize() {

	soundDataHandle_ = Audio::GetInstance()->LoadWave("./Resources/fanfare.wav");
	Audio::GetInstance()->PlayWave(soundDataHandle_);
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	
		Audio::GetInstance()->StopWave(voiceHandle_);
	
	}

}

void GameScene::Draw() {}
