#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// プレイヤー(自機)クラス
/// 自分の見た目・位置・移動だけを担当する
/// </summary>
class Player {
public:
	~Player();

	void Initialize();
	void Update();
	void Draw(const Camera& camera);

	const Vector3& GetPosition() const { return worldTransform_.translation_; }

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	static constexpr float kSpeed = 0.15f;
	static constexpr float kFieldHalfSize = 15.0f;
};