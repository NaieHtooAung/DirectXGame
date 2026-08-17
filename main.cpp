#include "EndScene.h"
#include "GameScene.h"
#include "StartScene.h"
#include <KamataEngine.h>

using namespace KamataEngine;

// シーンの種類
enum class Scene {
	kStart,
	kGame,
	kEnd,
};

// Windowsアプリのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// KamataEngineの初期化(ウィンドウタイトルは自由に変更してOK)
	KamataEngine::Initialize(L"Star Collector 3D");

	// DirectX共通部分の取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 現在のシーン(最初はスタート画面)
	Scene scene = Scene::kStart;

	// 各シーンの生成
	StartScene* startScene = new StartScene();
	startScene->Initialize();

	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	EndScene* endScene = new EndScene();

	// メインループ
	while (true) {
		// エンジン更新(falseが返ってきたら終了)
		if (KamataEngine::Update()) {
			break;
		}

		// ----- シーンごとの更新とシーン切り替え -----
		switch (scene) {
		case Scene::kStart:
			startScene->Update();
			if (startScene->IsGameStart()) {
				// スタート画面 → ゲーム画面
				gameScene->Reset();
				scene = Scene::kGame;
			}
			break;

		case Scene::kGame:
			gameScene->Update();
			if (gameScene->IsTimeUp()) {
				// ゲーム画面 → 終了画面
				endScene->Initialize(gameScene->GetScore());
				scene = Scene::kEnd;
			}
			break;

		case Scene::kEnd:
			endScene->Update();
			if (endScene->IsReturnToGame()) {
				// 終了画面 → ゲーム画面(やり直し)
				gameScene->Reset();
				scene = Scene::kGame;
			}
			break;
		}

		// 描画開始
		dxCommon->PreDraw();

		// 3Dモデル描画の準備(これを呼ばないとmodel_->Draw()内でcmdListがnullptrになりクラッシュする)
		Model::PreDraw();

		// ----- シーンごとの描画 -----
		switch (scene) {
		case Scene::kStart:
			startScene->Draw();
			break;
		case Scene::kGame:
			gameScene->Draw();
			break;
		case Scene::kEnd:
			endScene->Draw();
			break;
		}

		// 3Dモデル描画の終了
		Model::PostDraw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 後片付け
	delete startScene;
	delete gameScene;
	delete endScene;

	KamataEngine::Finalize();

	return 0;
}