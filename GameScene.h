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

private:
	void CheckCollision();
	void Reset();

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