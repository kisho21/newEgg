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

Character::Character(void) :growthRate(0), gameMoney(1000), requestCount(0)/*,requestFlg(false)*/ {
	requestFlg = false;
	playFlg = eatFlg = toiletFlg = bathFlg = touchFlg = false;
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
	Request();
}

void Character::Draw(void) {

	DrawGraph(charaX, charaY, charaImage[charaFrame], true);
	if(requestFlg) DrawRequest();
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
		if (requestCount >= 100) {
			std::uniform_int_distribution<> number(0, wants.size() - 1);		//std::uniform_int_distribution<> x(min, max)：指定された範囲で整数を均一な確率で生成
			CharaWantTo wantsNum = wants[number(randWants)];					//生成された乱数から要素を取得
			if (wantsNum == 0) wantTo = play;
			else if (wantsNum == 1) wantTo = eat;
			else if (wantsNum == 2) wantTo = toilet;
			else if (wantsNum == 3) wantTo = bath;
			else if (wantsNum == 4) wantTo = touch;

			requestFlg = true;
			requestCount = 0;
		}
	}
	else {
		//リクエストフラグが立つと、ランダムで要求提示
		
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
		CharaPlay();

		//要求解消時
		//requestFlg = false;
		requestCount = 0;
		break;
	case eat:
		CharaEat();


		//要求解消時
		//requestFlg = false;
		requestCount = 0;
		break;
	case toilet:
		CharaToilet();

		//要求解消時
		//requestFlg = false;
		requestCount = 0;
		break;
	case bath:
		CharaBath();

		//要求解消時
		//requestFlg = false;
		requestCount = 0;
		break;
	case touch:
		CharaTouth();

		//要求解消時
		//requestFlg = false;
		requestCount = 0;
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
		DrawString(0, 16, "play", 0x000000);
		if (playFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case eat:
		DrawString(0, 16, "eat", 0x000000);
		if (eatFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case toilet:
		DrawString(0, 16, "toilet", 0x000000);
		if (toiletFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case bath:
		DrawString(0, 16, "bath", 0x000000);
		if (bathFlg) DrawString(0, 32, "true", 0x000000);
		break;
	case touch:
		DrawString(0, 16, "touch", 0x000000);
		if (touchFlg) DrawString(0, 32, "true", 0x000000);
		break;
	}
}

//
void Character::CharaPlay(void) {
	//要求提示時の挙動
	//要求解消時の数値の変化、成長度の加算
	playFlg = true;
	/*
	playFlg = true;  初期はfalse
	if(!playFlg) {		SceneGameからセッターでfalseに
		this->AddGrowth(10);
		requestFlg = false;
		requestCount = 0;
	}	てきな
	*/
	
	//解消するためのプレイヤーの操作はSceneGameに書く
}

//
void Character::CharaEat(void) {
	eatFlg = true;
}

//
void Character::CharaToilet(void) {
	toiletFlg = true;
}

//
void Character::CharaBath(void) {
	bathFlg = true;
}

//
void Character::CharaTouth(void) {
	touchFlg = true;

}