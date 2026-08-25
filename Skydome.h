#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

// ---------------------------------------------
// スカイドーム（背景の空）を担当するクラス。
// カメラを常に中心にして大きな球を表示するだけの簡易版。
// ---------------------------------------------
class SkyDome {
public:
	void Initialize();
	void Update(const Camera& camera);
	void Draw(const Camera& camera);

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	uint32_t textureHandleSky_ = 0;
};