#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// タイトル画面(スタート画面)
/// タイトル画像(Sprite)とBGM(Audio)を表示・再生する。
/// スペースキーが押されたら、ゲームを始めたい合図(IsGameStart)を出す。
/// 実際のシーン切り替え(GameSceneの初期化など)はmain.cppが行う。
/// </summary>
class StartScene {
public:
	~StartScene();

	void Initialize();
	void Update();
	void Draw();

	// スペースキーが押されて「ゲームを始めたい」ときにtrueを返す
	bool IsGameStart() const { return isGameStart_; }

	// このシーンのBGMを止める(main.cppがゲーム開始時に呼ぶ)
	void StopBGM();

private:
	bool isGameStart_ = false;

	// タイトル画像
	Sprite* titleSprite_ = nullptr;
	uint32_t titleTextureHandle_ = 0;

	// タイトルBGM
	uint32_t bgmSoundHandle_ = 0;
	uint32_t bgmVoiceHandle_ = 0;
};