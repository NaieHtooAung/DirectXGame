#include "Particle.h"

void ParticleManager::Initialize() {
	// 後で専用パーティクルモデルに差し替える場所
	// model_ = Model::CreateFromOBJ("particle", true);
	model_ = Model::CreateFromOBJ("cube", true);
}

void ParticleManager::Spawn(const Vector3& position) {
	for (int32_t i = 0; i < 8; ++i) {
		ParticleData p;
		p.worldTransform.Initialize();
		p.worldTransform.translation_ = position;
		p.worldTransform.scale_ = { 0.2f, 0.2f, 0.2f };
		p.velocity = {
			(float(rand() % 200) / 100.0f - 1.0f) * 0.1f,
			(float(rand() % 200) / 100.0f) * 0.1f,
			(float(rand() % 200) / 100.0f - 1.0f) * 0.1f,
		};
		p.life = 20;
		particles_.push_back(p);
	}
}

void ParticleManager::Update() {
	for (auto it = particles_.begin(); it != particles_.end();) {
		it->worldTransform.translation_.x += it->velocity.x;
		it->worldTransform.translation_.y += it->velocity.y;
		it->worldTransform.translation_.z += it->velocity.z;
		it->life--;

		if (it->life <= 0) {
			it = particles_.erase(it);
		}
		else {
			it->worldTransform.UpdateMatrix();
			++it;
		}
	}
}

void ParticleManager::Draw(const Camera& camera) {
	for (auto& p : particles_) {
		model_->Draw(p.worldTransform, camera);
	}
}