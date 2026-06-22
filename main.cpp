#include "GameScene.h"
#include "TitleScene.h"
#include <KamataEngine.h>
#include <Windows.h>

using namespace KamataEngine;

// シーン列挙
enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
};

// グローバル変数
Scene scene = Scene::kUnknown;
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

// 関数宣言
void ChangeScene();
void UpdateScene();
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"GC2A_04_ネイ_トウーアウン_AL3");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 最初のシーンをタイトルに設定
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新(ウィンドウの×ボタンなどで終了)
		if (KamataEngine::Update()) {
			break;
		}

		// シーン切り替え処理
		ChangeScene();

		// シーンの更新処理
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		// シーンの描画処理
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 各シーンの解放
	delete titleScene;
	titleScene = nullptr;
	delete gameScene;
	gameScene = nullptr;

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

// シーン切り替え
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene && titleScene->IsFinished()) {
			// タイトル終了 → ゲームシーンへ
			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (gameScene && gameScene->IsFinished()) {
			// ゲーム終了 → タイトルへ
			scene = Scene::kTitle;

			delete gameScene;
			gameScene = nullptr;

			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;

	default:
		break;
	}
}

// シーンの更新
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene) {
			titleScene->Update();
		}
		break;

	case Scene::kGame:
		if (gameScene) {
			gameScene->Update();
		}
		break;

	default:
		break;
	}
}

// シーンの描画
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene) {
			titleScene->Draw();
		}
		break;

	case Scene::kGame:
		if (gameScene) {
			gameScene->Draw();
		}
		break;

	default:
		break;
	}
}