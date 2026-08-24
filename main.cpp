#include "EndScene.h"
#include "GameScene.h"
#include "StartScene.h"
#include <KamataEngine.h>

using namespace KamataEngine;

// 現在どのシーンを表示しているか
enum class SceneType {
	kStart, // タイトル画面
	kGame,  // メインのゲームシーン
	kEnd,   // 制限時間が終わったあとの終了画面
};

// Windowsアプリのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// KamataEngineの初期化(ウィンドウタイトルは自由に変更してOK)
	KamataEngine::Initialize(L"Star Collector 3D");

	// DirectX共通部分の取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 現在のシーン(最初はタイトル画面から始める)
	SceneType sceneType = SceneType::kStart;

	// タイトル画面の生成と初期化
	StartScene* startScene = new StartScene();
	startScene->Initialize();

	// ゲームシーンの生成(初期化はタイトル画面からゲームを始めるときに行う)
	GameScene* gameScene = new GameScene();

	// 終了画面の生成(初期化はタイムアップしたときに行う)
	EndScene* endScene = new EndScene();

	// メインループ
	while (true) {
		// エンジン更新(falseが返ってきたら終了)
		if (KamataEngine::Update()) {
			break;
		}

		// ----- 更新 -----
		switch (sceneType) {
		case SceneType::kStart:
			startScene->Update();

			// スペースキーでゲームシーンを初期化して開始する
			if (startScene->IsGameStart()) {
				startScene->StopBGM();
				gameScene->Initialize();
				sceneType = SceneType::kGame;
			}
			break;

		case SceneType::kGame:
			gameScene->Update();

			// 制限時間が終わったら終了画面に切り替える
			if (gameScene->IsTimeUp()) {
				gameScene->StopBGM();
				endScene->Initialize(gameScene->GetScore());
				sceneType = SceneType::kEnd;
			}
			break;

		case SceneType::kEnd:
			endScene->Update();

			// Rキーが押されたらゲームシーンをやり直してゲームシーンに戻る
			if (endScene->IsReturnToGame()) {
				gameScene->Reset();
				sceneType = SceneType::kGame;
			}
			break;
		}

		// ----- 描画 -----
		dxCommon->PreDraw();

		// ----- 3Dモデルの描画(GameSceneのみ) -----
		// これを呼ばないとmodel_->Draw()内でcmdListがnullptrになりクラッシュする
		Model::PreDraw();
		if (sceneType == SceneType::kGame) {
			gameScene->Draw();
		}
		Model::PostDraw();

		// ----- 2D画像(Sprite)の描画(StartScene / EndScene) -----
		Sprite::PreDraw();
		switch (sceneType) {
		case SceneType::kStart:
			startScene->Draw();
			break;

		case SceneType::kEnd:
			endScene->Draw();
			break;

		default:
			break;
		}
		Sprite::PostDraw();

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