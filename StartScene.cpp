#include "StartScene.h"

StartScene::~StartScene() { delete titleSprite_; }

void StartScene::Initialize() {
	isGameStart_ = false;

	// ----- タイトル画像 -----
	// ▼▼▼ ここにタイトル画像のパスを入れる ▼▼▼
	titleTextureHandle_ = TextureManager::Load("./Resources/sample.png");
	// 画面左上(0,0)に原寸で表示。画面中央に置きたい場合は座標を調整する
	titleSprite_ = Sprite::Create(titleTextureHandle_, {0.0f, 0.0f});

	// ----- タイトルBGM -----
	// ▼▼▼ ここにBGMファイルのパスを入れる ▼▼▼
	Audio* audio = Audio::GetInstance();
	bgmSoundHandle_ = audio->LoadWave("./Resources/sound/game_bgm.mp3");
	bgmVoiceHandle_ = audio->PlayWave(bgmSoundHandle_, true); // 第2引数true = ループ再生
}

void StartScene::Update() {
	isGameStart_ = false;

	// スペースキーでゲーム開始の合図を出す
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isGameStart_ = true;
	}
}

void StartScene::Draw() {
	// タイトル画像を描画(2Dなので3Dモデルとは別のタイミングでmain.cppから呼ばれる)
	titleSprite_->Draw();
}

void StartScene::StopBGM() { Audio::GetInstance()->StopWave(bgmVoiceHandle_); }