#include "WorldTransform.h"
#include "player.h"

void WorldTransform::UpdateMatrix() {
//スケール、回転、平行移動を合成して行列を作成する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	//定数バッファに転送する
	TransferMatrix();
}