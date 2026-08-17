#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// ゲーム開始前の「スタート画面」
/// SPACEキーが押されたら、ゲームを開始したい合図(IsGameStart)を出す。
/// 実際のシーン切り替えはmain.cppが行う。
/// </summary>
class StartScene {
public:
	void Initialize();
	void Update();
	void Draw();

	// SPACEキーが押されて「ゲームを開始したい」ときにtrueを返す
	bool IsGameStart() const { return isGameStart_; }

private:
	bool isGameStart_ = false;
};