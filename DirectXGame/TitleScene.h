#pragma once
#include "Input.h"
#include "Sprite.h"
#include "DirectXCommon.h"

class TitleScene {
public:
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;
	Input* input_ = nullptr;

	uint32_t titleHandle_ = 0;
	Sprite* sprite_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
};