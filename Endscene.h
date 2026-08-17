#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// 制限時間が終わったときに表示する「終了画面」
/// Rキーが押されたら、ゲームシーンに戻りたい合図(IsReturnToGame)を出す。
/// 実際のシーン切り替え(GameSceneの再初期化など)はmain.cppが行う。
/// </summary>
class EndScene {
public:
	void Initialize(int score);
	void Update();
	void Draw();

	// Rキーが押されて「ゲームシーンに戻りたい」ときにtrueを返す
	bool IsReturnToGame() const { return isReturnToGame_; }

private:
	int score_ = 0;
	bool isReturnToGame_ = false;
};