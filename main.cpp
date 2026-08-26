#include <KamataEngine.h>
#include "GameScene.h"

using namespace KamataEngine;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	KamataEngine::Initialize(L"スターコレクター");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	while (true) {
		// ウィンドウが閉じられたらループを抜ける
		if (KamataEngine::Update()) {
			break;
		}

		gameScene->Update();

		dxCommon->PreDraw();
		gameScene->Draw();
		dxCommon->PostDraw();
	}

	delete gameScene;
	KamataEngine::Finalize();
	return 0;
}