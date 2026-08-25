#pragma once
#include <KamataEngine.h>
#include <list>

using namespace KamataEngine;

// ---------------------------------------------
// フィードバック演出用のパーティクル1個分（3D版：小さいキューブを飛ばす）
// ---------------------------------------------
struct ParticleData {
	WorldTransform worldTransform;
	Vector3 velocity;
	int32_t life;
};

// ---------------------------------------------
// パーティクルをまとめて管理するクラス
// ---------------------------------------------
class ParticleManager {
public:
	void Initialize();
	void Update();
	void Draw(const Camera& camera);

	// 指定座標にパーティクルを発生させる
	void Spawn(const Vector3& position);

private:
	// 後で専用パーティクルモデルに差し替える場所
	Model* model_ = nullptr;
	std::list<ParticleData> particles_;
};