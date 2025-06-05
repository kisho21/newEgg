//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "Character.h"

//シングルトンインスタンスの初期化
Character* Character::inst = nullptr;

Character::Character(void) :growthRate(0), gameMoney(1000) {

}

Character::~Character(void) {

}

//シングルトンインスタンスを取得する
Character* Character::getInst() {

	if (inst == nullptr) inst = new Character();

	return inst;
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

//成長度を設定
void Character::SetGrowth(int growth) {

	if (growth >= 0) growthRate = growth;	//最小値のチェック
	else growthRate = 0;					//最小値に設定
}

//成長度を増加させる
void Character::AddGrowth(int amount) {

	if (amount > 0) growthRate += amount;
}

//所持金を設定
void Character::SetMoney(int money) {

	if (money >= 0) gameMoney = money;
	else gameMoney = 0;
}

//所持金を増加させる
void Character::AddMoney(int amount) {

	gameMoney += amount;
	if (gameMoney < 0) gameMoney = 0;
}