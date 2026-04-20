#pragma once
#include <KamataEngine.h>

class GameScene
{
public:
	void Initialize();

	void Update();

	void Draw();

	private:
	uint32_t soundDataHandle_ = 0;
	    uint32_t voiceHandle_ = 0;
};