#pragma once
#include <KamataEngine.h>

class Fade {

public:
	void Initialize(uint32_t textureHandle);
	void Update();
	void Draw();
	
	enum class Status {
		None,
		FadeIn,
		FadeOut,
	};
	
	Status status_ = Status::None;
	void Start(Status status, float duration);
	void Stop();
	bool IsFinished() const;

private:
	float duration_ = 0.0f;
	float counter_ = 0.0f;
	
	KamataEngine::Sprite* sprite_ = nullptr;
};