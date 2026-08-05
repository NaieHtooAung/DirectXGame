#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// 「Star Collector 3D」
/// プレイヤー(立方体)を動かして、フィールド上のアイテムを
/// 制限時間内にできるだけ多く集めるだけの、超シンプルな3Dゲーム。
/// </summary>
class GameScene {
public:
	~GameScene();

	void Initialize();
	void Update();
	void Draw();

private:
	// プレイヤーの移動処理
	void MovePlayer();

	// アイテムとの当たり判定
	void CheckCollision();

	// 指定した番号のアイテムをランダムな位置に配置しなおす
	void SpawnItem(int index);

	// ゲームをリセットする(リトライ用)
	void Reset();

private:
	// カメラ(見下ろし視点)
	Camera camera_;

	// プレイヤー
	WorldTransform playerTransform_;
	Model* playerModel_ = nullptr;
	uint32_t playerTextureHandle_ = 0;

	// 床
	WorldTransform floorTransform_;
	Model* floorModel_ = nullptr;
	uint32_t floorTextureHandle_ = 0;

	// アイテム(星のかわりに立方体を使う)
	static const int kItemCount = 5;
	WorldTransform itemTransforms_[kItemCount];
	bool itemAlive_[kItemCount] = {};
	Model* itemModel_ = nullptr;
	uint32_t itemTextureHandle_ = 0;

	// スコア
	int score_ = 0;

	// 残り時間(フレーム数。60fps換算で30秒 = 1800フレーム)
	int32_t timeLeft_ = 60 * 30;

	// ゲームオーバー(タイムアップ)フラグ
	bool isGameOver_ = false;

	// プレイヤーの移動速度
	static constexpr float kPlayerSpeed = 0.15f;

	// アイテムに触れたと判定する距離
	static constexpr float kCollisionDistance = 1.5f;

	// フィールドの半径(この範囲内だけ移動・出現できる)
	static constexpr float kFieldHalfSize = 15.0f;
};