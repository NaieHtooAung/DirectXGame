#include "StartScene.h"

void StartScene::Initialize() { isGameStart_ = false; }

void StartScene::Update() {
	isGameStart_ = false;
	// SPACEキーでゲームシーンに進む合図を出す
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isGameStart_ = true;
	}
}

void StartScene::Draw() {
	// このシンプルなスタート画面では3Dモデルは描画していません。
	// 本来はここでSpriteなどを使って
	// タイトルロゴや「PRESS SPACE TO START」といったテキストを表示します
	// (Spriteの使い方は別の課題テーマなので、ここでは最小限の構成にしています)。
}