#include "TitleScene.h"
#include "TextureManager.h"

TitleScene::~TitleScene() { delete sprite_; }

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	titleHandle_ = TextureManager::Load("title.png");
	sprite_ = Sprite::Create(titleHandle_, {0, 0});
	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	finished_ = false;

}

void TitleScene::Update() {
	XINPUT_STATE joyState;
	// ゲームパッド未接続なら何もせず抜ける
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || input_->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}

	/*if (input_->TriggerKey(DIK_SPACE)) {
		finished_ = true;
	}*/
}

void TitleScene::Draw() { 
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList(); 

	#pragma region
	Sprite::PreDraw(commandList);
	sprite_->Draw();

	Sprite::PostDraw();

}
