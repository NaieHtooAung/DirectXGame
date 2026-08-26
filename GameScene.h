#pragma once
#include <KamataEngine.h>
#include <vector>
#include <functional>
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Particle.h"
#include "Ground.h"
#include "SkyDome.h"
#include "PressEnterText.h"
#include "PressRText.h"
#include "PressBText.h"

using namespace KamataEngine;

// ---------------------------------------------
// シーン一覧：タイトル→説明→ゲーム→（勝敗が決まったらフェード）→エンド→（Rでフェード）→ゲーム、のループ
// ---------------------------------------------
enum class Scene {
	kStart,
	kExplanation, // タイトルとゲーム本編の間の説明画面
	kGame,
	kStageClear,  // ステージクリア演出（次のステージへ行く前のつなぎ）
	kEnd,         // 勝敗結果の表示＋リトライ待ち（Rキーでループ）
};

class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	void UpdateStart();
	void UpdateExplanation();
	void UpdateGame();
	void UpdateStageClear();
	void UpdateEnd();

	// ゲームの状態を初期値に戻す（エンド→ゲームのループ、ポーズ→タイトルへ戻る時などで使う）
	void Reset();

	// ---- "Start!"ポップアップをカメラ基準の初期状態にセットして表示開始する ----
	// 説明画面→ゲーム本編、および エンド画面→ゲーム本編（リトライ）の両方で呼ぶ
	void ShowStartPopup();

	// 指定したステージ番号の敵・コイン配置を読み込む（難易度がだんだん上がる）
	void LoadStage(int32_t stageNumber);
	// 現在の敵・コインを全部消す（LoadStageの前処理）
	void ClearStageObjects();

	// ---- 画面遷移（フェードアウト→シーン切替→フェードイン）----
	// onComplete はフェードが完全に黒くなった瞬間（シーン切替と同時）に1回だけ呼ばれる
	void StartTransition(Scene target, std::function<void()> onComplete = nullptr);
	void UpdateTransition();

	Scene scene_ = Scene::kStart;
	bool isPaused_ = false;
	bool won_ = false; // 結果（true=クリア, false=ゲームオーバー）

	// ---- ステージ進行（全3ステージ、進むほど難しくする） ----
	int32_t currentStage_ = 1;
	static const int32_t kMaxStage_ = 3;
	int32_t stageClearTimer_ = 0;
	static const int32_t kStageClearDuration_ = 90; // 約1.5秒（60fps想定）

	// ---- 画面遷移（フェード）----
	float fade_ = 0.0f;        // 0=何も無し（見える）, 1=真っ黒
	bool isFading_ = false;    // フェード演出中かどうか
	bool fadingOut_ = true;    // true=黒くしていく途中, false=黒から戻していく途中
	Scene fadeTargetScene_ = Scene::kGame; // フェードが終わった先で切り替えるシーン
	std::function<void()> onFadeComplete_ = nullptr; // 切替と同時に呼ぶ処理（Resetなど）
	static constexpr float kFadeSpeed_ = 0.02f;

	// ---- 3Dカメラ（カメラワーク） ----
	Camera camera_;

	uint32_t bgmVoiceHandle_ = 0u;

	Player* player_ = nullptr;
	std::vector<Enemy*> enemies_;
	std::vector<Coin*> coins_;

	// ---- パーティクル：コイン取得用／敵撃破・被弾用で分ける ----
	ParticleManager* coinParticle_ = nullptr;
	ParticleManager* enemyParticle_ = nullptr;

	Ground* ground_ = nullptr;
	SkyDome* skyDome_ = nullptr;

	// フェード用の黒背景スプライト（画面遷移のみ2D）
	uint32_t blackTextureHandle_ = 0;
	Sprite* fadeSprite_ = nullptr;

	// タイトル画面用スプライト（後で画像に差し替える）
	uint32_t titleTextureHandle_ = 0;
	int32_t textureHandleSky_ = 0;
	Sprite* titleSprite_ = nullptr;
	uint32_t bgmSoundHandle_ = 0;

	// ---- 説明画面用スプライト ----
	uint32_t explanationTextureHandle_ = 0;
	Sprite* explanationSprite_ = nullptr;

	// ---- 結果画面用スプライト（クリア画像／ゲームオーバー画像）----
	uint32_t gameClearTextureHandle_ = 0;
	uint32_t gameOverTextureHandle_ = 0;
	Sprite* gameClearSprite_ = nullptr;
	Sprite* gameOverSprite_ = nullptr;

	// エンド画面用の色付きオーバーレイ（クリア=緑、ゲームオーバー=赤）／ポーズ中の暗転オーバーレイにも流用
	Sprite* endSprite_ = nullptr;

	// ---- 浮遊する3D文字（タイトルの"Press Enter"／リトライ待ちの"Press R"／ポーズ中の"Press B"）----
	PressEnterText* pressEnterText_ = nullptr;
	PressRText* pressRText_ = nullptr;
	PressBText* pressBText_ = nullptr;

	// ---- ゲーム開始時に一瞬表示する"Start!"の3Dモデル ----
	// 説明画面→ゲーム本編／エンド画面→ゲーム本編（リトライ）の直後に表示する。カメラを基準にした位置に毎フレーム追従させ、
	// 最初は奥・小さい状態から手前・等倍まで近づきながら大きくなり、少し浮遊した後、
	// 最後に縮みながら消える。専用クラスは作らずGameScene内で直接Model+WorldTransformを扱う。
	// Resources/Start/Start.obj を用意する場所
	Model* startModel_ = nullptr;
	WorldTransform startWorldTransform_;
	bool showStartPopup_ = false;
	int32_t startPopupTimer_ = 0;
	static const int32_t kStartPopupDuration_ = 60;        // 表示している総フレーム数（約1秒/60fps想定）
	static const int32_t kStartPopupGrowFrames_ = 20;      // 奥→手前・小→大きくなるまでのフレーム数
	static const int32_t kStartPopupFadeFrames_ = 15;      // 最後にこのフレーム数だけ縮んで消える
	static constexpr float kStartPopupFarZOffset_ = 40.0f;   // 登場開始時：カメラからこれだけ奥（遠く）に置く
	static constexpr float kStartPopupNearZOffset_ = 15.0f;  // 登場完了後：カメラの前のこの距離で止まる
	static constexpr float kStartPopupHeightOffset_ = 3.0f;  // カメラを基準にした高さ
	static constexpr float kStartPopupStartScale_ = 0.1f;    // 登場開始時の小ささ（等倍=1.0f）

	// ---- HPバー：画像不要、白1x1テクスチャを色付きブロックとして3つ並べる ----
	// 被弾するとhp_が減り、その分のブロックが描画されなくなる（背景の暗い枠は残る）
	static const int32_t kMaxHP_ = 3;
	static const int32_t kHPBlockWidth_ = 50;
	static const int32_t kHPBlockHeight_ = 30;
	static const int32_t kHPBlockGap_ = 10;
	Sprite* hpBlockBg_[kMaxHP_] = { nullptr, nullptr, nullptr }; // 空スロットの暗い枠（常に表示）
	Sprite* hpBlockFg_[kMaxHP_] = { nullptr, nullptr, nullptr }; // 残りHP分だけ表示する赤ブロック
};