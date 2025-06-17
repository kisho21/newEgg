//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>
#include <vector>
#include <random>

#include "Application.h"
#include "Character.h"

//シングルトンインスタンスの初期化
Character* Character::inst = nullptr;

Character::Character(void) :growthRate(-267), gameMoney(1000), requestCount(0)/*,requestFlg(false)*/ {

	playImage = eatImage = toiletImage = bathImage = touchImage = -1;
	wantTo = normal;
	requestFlg = false;
	playFlg = eatFlg = toiletFlg = bathFlg = touchFlg = false;
	idleFlg = true;
	walkFlg = backFlg = false;
}

Character::~Character(void) {

}

//シングルトンインスタンスを取得する
Character* Character::getInst() {

	if (inst == nullptr) inst = new Character();

	return inst;
}

bool Character::SystemInit(void) {

	int chara = LoadDivGraph("image/chara-128.png", MAX_ANIM, ANIM_NUM_CHARA, 1, CHARA_WID, CHARA_HIG, charaImage);
	if (chara == -1) return false;
	int cWalk = LoadDivGraph("image/chara-walk.png", MAX_ANIM, ANIM_NUM_CHARA, 1, CHARA_WID, CHARA_HIG, cWalkImage);
	if (cWalk == -1) return false;
	int cBack = LoadDivGraph("image/chara-back.png", MAX_ANIM, ANIM_NUM_CHARA, 1, CHARA_WID, CHARA_HIG, cBackImage);
	if (cBack == -1) return false;
	int bubble = LoadDivGraph("image/bubble.png", MAX_ANIM, ANIM_NUM_BUBBLE, 1, BUBBLE_SIZE, BUBBLE_SIZE, bubbleImage);
	if (bubble == -1) return false;

	playImage = LoadGraph("image/wantPlay.png");
	if (playImage == -1) return false;
	eatImage = LoadGraph("image/wantEat.png");
	if (eatImage == -1) return false;
	toiletImage = LoadGraph("image/wantToilet.png");
	if (toiletImage == -1) return false;
	bathImage = LoadGraph("image/wantBath.png");
	if (bathImage == -1) return false;
	touchImage = LoadGraph("image/wantTouch.png");
	if (touchImage == -1) return false;

	return true;
}

void Character::GameInit(void) {

	wantPosX = 169;
	wantPosY = 129;
	charaX = Application::WINDOW_SIZE_WID / 2 - CHARA_WID / 2;
	charaY = 240;
	frame = 0;
	frameAdjust = 0;
	bathTime = 0;
}

void Character::Update(void) {

	WalkProc();
	Animation();
	RequestCount();
}

void Character::Draw(void) {

	if(idleFlg) DrawGraph(charaX, charaY, charaImage[frame], true);
	if (walkFlg) DrawGraph(charaX, charaY, cWalkImage[frame], true);
	if (backFlg) DrawGraph(charaX, charaY, cBackImage[frame], true);

	if (requestFlg) {
		DrawGraph(130, 100, bubbleImage[frame], true);
		DrawRequest();
	}
}

bool Character::Release(void) {

	if (DeleteGraph(playImage) == -1) return false;
	if (DeleteGraph(eatImage) == -1) return false;
	if (DeleteGraph(toiletImage) == -1) return false;
	if (DeleteGraph(bathImage) == -1) return false;
	if (DeleteGraph(touchImage) == -1) return false;

	return true;
}

void Character::WalkProc(void) {

	if (walkFlg) {
		charaX--;
		bathTime = 0;
		if (charaX < -128) {
			walkFlg = false;
			backFlg = true;
		}
	}
	else if (backFlg) {
		bathTime++;
		if (bathTime > 500) {
			charaX++;
		}
		if (charaX >= Application::WINDOW_SIZE_WID / 2 - CHARA_WID / 2) {
			backFlg = false;
			idleFlg = true;
			CharaBath();
		}
	}
}

void Character::Animation(void) {

	if (frameAdjust == 60) {
		frame++;
		frameAdjust = 0;
	}
	if (frame == MAX_ANIM) {
		frame = 0;
	}
	frameAdjust++;
}

//成長度を設定
void Character::SetGrowth(int growth) {

	if (growth >= -267) growthRate = growth;	//最小値のチェック
	else growthRate = -267;						//最小値に設定
}

//成長度を増加させる
void Character::AddGrowth(int amount) {

	if (amount > 0) growthRate += amount;
}

//所持金を設定
void Character::SetMoney(int money) {

	gameMoney = money;
	if (gameMoney < 0) { gameMoney = 0; }
}

//所持金を増加させる
void Character::AddMoney(int amount) {

	gameMoney += amount;
	if (gameMoney < 0) gameMoney = 0;
}

//要求カウントを設定
void Character::SetRequest(int request) {

	if (request >= 0) requestCount = request;
	else requestCount = 0;
}

//要求カウントを増加させる
void Character::AddRequest(int amount) {

	if (amount > 0) requestCount += amount;
}

//要求カウントの加算処理
void Character::RequestCount(void) {

	int requestAmount = 0;

	std::vector<CharaWantTo> wants = {
			CharaWantTo::play,
			CharaWantTo::eat,
			CharaWantTo::toilet,
			CharaWantTo::bath,
			CharaWantTo::touch
	};

	std::random_device rand;
	std::mt19937 randWants(rand());	

	if (!requestFlg) {
		requestAmount++;
		this->AddRequest(requestAmount);
		if (requestCount >= 200) {
			std::uniform_int_distribution<> number(0, wants.size() - 1);		//std::uniform_int_distribution<> x(min, max)：指定された範囲で整数を均一な確率で生成
			CharaWantTo wantsNum = wants[number(randWants)];					//生成された乱数から要素を取得
			if (wantsNum == 0) wantTo = normal;
			else if (wantsNum == 1) wantTo = eat;
			else if (wantsNum == 2) wantTo = toilet;
			else if (wantsNum == 3) wantTo = bath;
			else if (wantsNum == 4) wantTo = touch;
			else if (wantsNum == 5) wantTo = play;

			/*if (wantsNum >= 0 && wantsNum <= 4)*/ requestFlg = true;
			/*else requestFlg = false;*/
			requestCount = 0;
		}
	}
	else {
		//リクエストフラグが立つと、ランダムで要求提示
		Request();
		//else wantTo = normal;

		/*
		requestAmount++;
		this->AddRequest(requestAmount);
		if (requestCount >= 500) {
			requestFlg = false;
			requestCount = 0;
		}
		//trueのときもカウントできる
		*/
	}

}

//要求提示処理
void Character::Request(void) {

	switch(wantTo) {
	case normal:
		if (requestFlg) {
			requestFlg = false;
			requestCount = 0;
		}
		break;
	case play:
		playFlg = true;
		//CharaPlay();

		break;
	case eat:
		eatFlg = true;
		//CharaEat();

		break;
	case toilet:
		toiletFlg = true;
		//CharaToilet();

		break;
	case bath:
		bathFlg = true;
		//CharaBath();

		break;
	case touch:
		touchFlg = true;
		//CharaTouth();

		break;
	}
}

//要求描画処理
void Character::DrawRequest(void) {

	switch (wantTo) {
	case normal:
		DrawString(0, 16, "normal", 0x000000);
		break;
	case play:
		DrawGraph(wantPosX, wantPosY, playImage, true);

		DrawString(0, 16, "play", 0x000000);
		if (playFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case eat:
		DrawGraph(wantPosX, wantPosY, eatImage, true);

		DrawString(0, 16, "eat", 0x000000);
		if (eatFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case toilet:
		DrawGraph(wantPosX, wantPosY, toiletImage, true);

		DrawString(0, 16, "toilet", 0x000000);
		if (toiletFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case bath:
		DrawGraph(wantPosX, wantPosY, bathImage, true);

		DrawString(0, 16, "bath", 0x000000);
		if (bathFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case touch:
		DrawGraph(wantPosX, wantPosY, touchImage, true);

		DrawString(0, 16, "touch", 0x000000);
		if (touchFlg) DrawString(0, 32, "true", 0x000000);
		break;
	}
}

//遊びたいを解消
void Character::CharaPlay(void) {
	//要求提示時の挙動
	//要求解消時の数値の変化、成長度の加算
	this->AddGrowth(5);
	requestFlg = false;
	requestCount = 0;
	wantTo = normal;
	playFlg = false;
	
	//解消するためのプレイヤーの操作はSceneGameに書く
}

//おなかすいたを解消
void Character::CharaEat(void) {
	
	this->AddGrowth(5);
	requestFlg = false;
	requestCount = 0;
	wantTo = normal;
	eatFlg = false;
}

//トイレ行きたいを解消
void Character::CharaToilet(void) {
	
	this->AddGrowth(5);
	requestFlg = false;
	requestCount = 0;
	wantTo = normal;
	toiletFlg = false;
}

//お風呂入りたいを解消
void Character::CharaBath(void) {
	
	this->AddGrowth(5);
	requestFlg = false;
	requestCount = 0;
	wantTo = normal;
	bathFlg = false;
}

//なでてほしいを解消
void Character::CharaTouth(void) {

	this->AddGrowth(5);
	requestFlg = false;
	requestCount = 0;
	wantTo = normal;
	touchFlg = false;
}