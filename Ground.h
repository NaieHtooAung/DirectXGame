#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// 地面（床）を担当するクラス。ずっと同じ場所に置いておくだけの簡易版。
// ---------------------------------------------
class Ground {
public:
	void Initialize();
	void Draw(const Camera& camera);

private:
	// 後で専用の地面モデルに差し替える場所（Resources/floor/floor.obj）
	int32_t textureHandleFloor_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
};