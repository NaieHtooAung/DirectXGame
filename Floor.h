#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

/// <summary>
/// 床クラス
/// 見た目と大きさだけを持つ、動かないオブジェクト
/// </summary>
class Floor {
public:
	~Floor();

	void Initialize();
	void Draw(const Camera& camera);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	static constexpr float kFieldHalfSize = 15.0f;
};