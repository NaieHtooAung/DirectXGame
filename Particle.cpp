#include "Particle.h"
#include "UpdateMatrix.h"
ParticleManager::~ParticleManager() {
	for (auto* p : particles_) {
		delete p->worldTransform;
		delete p;
	}
}

void ParticleManager::Initialize(const std::string& modelName) {
	model_ = Model::CreateFromOBJ(modelName, true);
}

void ParticleManager::Spawn(const Vector3& position) {
	for (int32_t i = 0; i < 8; ++i) {
		ParticleData* p = new ParticleData();
		p->worldTransform = new WorldTransform();
		p->worldTransform->Initialize();
		p->worldTransform->translation_ = position;
		p->worldTransform->scale_ = { 0.2f, 0.2f, 0.2f };
		p->velocity = {
			(float(rand() % 200) / 100.0f - 1.0f) * 0.1f,
			(float(rand() % 200) / 100.0f) * 0.1f,
			(float(rand() % 200) / 100.0f - 1.0f) * 0.1f,
		};
		p->life = 20;
		particles_.push_back(p);
	}
}

void ParticleManager::Update() {
	for (auto it = particles_.begin(); it != particles_.end();) {
		ParticleData* p = *it;
		p->worldTransform->translation_.x += p->velocity.x;
		p->worldTransform->translation_.y += p->velocity.y;
		p->worldTransform->translation_.z += p->velocity.z;
		p->life--;

		if (p->life <= 0) {
			delete p->worldTransform;
			delete p;
			it = particles_.erase(it);
		}
		else {
			p->worldTransform->matWorld_ = MakeAffineMatrix(p->worldTransform->scale_, p->worldTransform->rotation_, p->worldTransform->translation_);
			p->worldTransform->TransferMatrix();
			++it;
		}
	}
}

void ParticleManager::Draw(const Camera& camera) {
	for (auto& p : particles_) {
		model_->Draw(*p->worldTransform, camera);
	}
}