#include "GameScene.h"
#include "2d/ImGuiManager.h"
using namespace KamataEngine;

void GameScene::Initialize() {}

void GameScene::Update() {

	ImGui::Begin("Debug1");
	ImGui::Text("Kamata Tarou ", 2050, 12, 31);
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

}

void GameScene::Draw() {}
