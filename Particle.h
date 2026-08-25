#pragma once
#include <KamataEngine.h>
#include <list>
#include <string>

using namespace KamataEngine;

// ---------------------------------------------
// フィードバック演出用のパーティクル1個分（3D版：小さいキューブを飛ばす）
// WorldTransformはコピーできないため、必ずポインタで持つ
// ---------------------------------------------
struct ParticleData {
	WorldTransform* worldTransform = nullptr;
	Vector3 velocity{};
	int32_t life = 0;
};

// ---------------------------------------------
// パーティクルをまとめて管理するクラス
// コイン用／敵用など、用途ごとにインスタンスを分けて使う
// ---------------------------------------------
class ParticleManager {
public:
	~ParticleManager();

	// modelName: 使用するモデル名（例："cube"、"particle_coin"、"particle_enemy"）
	void Initialize(const std::string& modelName = "cube");
	void Update();
	void Draw(const Camera& camera);

	// 指定座標にパーティクルを発生させる
	void Spawn(const Vector3& position);

private:
	Model* model_ = nullptr;
	std::list<ParticleData*> particles_;
};