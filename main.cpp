#include "GameScene.h"
#include <KamataEngine.h>

using namespace KamataEngine;

// Windowsアプリのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// KamataEngineの初期化(ウィンドウタイトルは自由に変更してOK)
	KamataEngine::Initialize(L"Star Collector 3D");

	// DirectX共通部分の取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// ゲームシーンの生成と初期化
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジン更新(falseが返ってきたら終了)
		if (KamataEngine::Update()) {
			break;
		}

		// ゲームシーンの更新
		gameScene->Update();

		// 描画開始
		dxCommon->PreDraw();

		// 3Dモデル描画の準備(これを呼ばないとmodel_->Draw()内でcmdListがnullptrになりクラッシュする)
		Model::PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();

		// 3Dモデル描画の終了
		Model::PostDraw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// 後片付け
	delete gameScene;

	KamataEngine::Finalize();

	return 0;
}