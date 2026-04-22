#include <Windows.h>
#include <KamataEngine.h>
#include "GameScene.h"
// Windowsアプリでのエントリーポイント(main関数)
using namespace KamataEngine;
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"GC2A_04_ネイ_トウーアウン_AL3");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();
	// ゲームループ
	while (true) {
		
		if (KamataEngine::Update()) {
			break;
		}
		imguiManager->Begin();
		gameScene->Update();
		imguiManager->End();
		dxCommon->PreDraw();
		// 描画処理

		gameScene->Draw();
		imguiManager->Draw();
		AxisIndicator::GetInstance()->Draw();
		
		dxCommon->PostDraw();
	}
	delete gameScene;
	gameScene = nullptr;

	KamataEngine::Finalize();
	return 0;
}
