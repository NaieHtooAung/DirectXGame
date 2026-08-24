#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// アイテム(集める対象)クラス
/// モデル・テクスチャはGameSceneが1つだけ読み込んで、
/// 各アイテムに渡す(使い回す)ことでメモリと読み込み時間を節約する
/// </summary>
class Item {
public:
	// model, textureHandle はGameScene側で読み込んだものを渡してもらう(所有はしない)
	void Initialize(Model* model, uint32_t textureHandle);
	void Update();
	void Draw(const Camera& camera);

	// 別の場所にランダムで再配置する
	void Respawn();

	bool IsAlive() const { return isAlive_; }
	void SetAlive(bool alive) { isAlive_ = alive; }

	const Vector3& GetPosition() const { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	Model* itemModel_ = nullptr; // 借りているだけ(delete しない)
	uint32_t textureHandle_ = 0;
	bool isAlive_ = true;

	static constexpr float kFieldHalfSize = 15.0f;
};