#pragma once
#include "Floor.h"
#include "Item.h"
#include "Player.h"
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// 「Star Collector 3D」
/// プレイヤー(Player)を動かして、フィールド上のアイテム(Item)を
/// 制限時間内にできるだけ多く集める、超シンプルな3Dゲーム。
/// GameScene自体はPlayer/Floor/Itemをまとめて動かすだけの役割。
/// </summary>
class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

	// 制限時間が終わったらtrueを返す(main.cppがこれを見て終了画面に切り替える)
	bool IsTimeUp() const { return isGameOver_; }

	// 現在のスコアを取得(終了画面に渡す用)
	int GetScore() const { return score_; }

	// ゲームを最初からやり直す(main.cppがEndSceneからの復帰時に呼ぶ)
	void Reset();

private:
	void CheckCollision();

private:
	// カメラ(見下ろし視点)
	Camera camera_;

	Player player_;
	Floor floor_;

	// アイテムは全部同じモデル・テクスチャを使い回す(GameSceneが1つだけ読み込む)
	static const int kItemCount = 5;
	Item items_[kItemCount];
	Model* itemModel_ = nullptr;
	uint32_t itemTextureHandle_ = 0;

	// スコア
	int score_ = 0;

	// 残り時間(フレーム数。60fps換算で30秒 = 1800フレーム)
	int32_t timeLeft_ = 60 * 30;

	// ゲームオーバー(タイムアップ)フラグ
	bool isGameOver_ = false;

	// アイテムに触れたと判定する距離
	static constexpr float kCollisionDistance = 1.5f;
};