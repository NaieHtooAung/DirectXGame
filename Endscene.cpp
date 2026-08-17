#include "EndScene.h"

void EndScene::Initialize(int score) {
	score_ = score;
	isReturnToGame_ = false;
}

void EndScene::Update() {
	isReturnToGame_ = false;

	// Rキーでゲームシーンに戻る合図を出す
	if (Input::GetInstance()->PushKey(DIK_R)) {
		isReturnToGame_ = true;
	}
}

void EndScene::Draw() {
	// このシンプルな終了画面では3Dモデルは描画していません。
	// 本来はここでSpriteなどを使って
	// 「TIME UP!」「Score: 〇〇」「Press R to retry」といったテキストや
	// 背景画像を表示します(Spriteの使い方は別の課題テーマなので、
	// ここでは最小限の構成にしています)。
	//
	// score_ の値はここで使えます(例: sprintfで文字列に変換してSprite表示など)。
}