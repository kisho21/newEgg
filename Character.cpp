//
//ÉQÅ[ÉÄ : egg
//éÅñº   : âiãg
//
#include <DxLib.h>

#include "Application.h"
#include "Character.h"

Character::Character(void) {

}

Character::~Character(void) {

}

bool Character::SystemInit(void) {

	int chara = LoadDivGraph("image/chara-128.png", MAX_CHARA, ANIM_NUM_CHARA, 1, CHARA_WID, CHARA_HIG, charaImage);
	if (chara == -1) return false;

	return true;
}

void Character::GameInit(void) {

	charaX = Application::WINDOW_SIZE_WID / 2 - CHARA_WID / 2;
	charaY = 240;
	charaFrame = 0;
	charaFrameAdjust = 0;
}

void Character::Update(void) {

	Animation();
}

void Character::Draw(void) {

	DrawGraph(charaX, charaY, charaImage[charaFrame], true);
}

bool Character::Release(void) {

	return true;
}

void Character::Animation(void) {

	if (charaFrameAdjust == 60) {
		charaFrame++;
		charaFrameAdjust = 0;
	}
	if (charaFrame == MAX_CHARA) {
		charaFrame = 0;
	}
	charaFrameAdjust++;
}