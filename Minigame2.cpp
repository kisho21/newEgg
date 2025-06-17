//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "Minigame2.h"

Minigame2::Minigame2(void) {

}

Minigame2::~Minigame2(void) {

}

bool Minigame2::SystemInit(void) {


	return true;
}

void Minigame2::GameInit(void) {

}

void Minigame2::Update(void) {

	// キーの状態
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);

	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得
}

void Minigame2::Draw(void) {

}

bool Minigame2::Release(void) {


	return true;
}