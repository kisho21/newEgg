#define NOMINMAX	//Windows.hやDxLibに含まれるmin/maxマクロとの競合を防ぐ

//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>
#include <stdexcept>    // std::out_of_range のために必要
#include <limits>		// std::numeric_limits のために必要
#include <string>       // std::string, std::to_string, std::stoi, std::stoll のために必要
#include <algorithm>    // std::sort のために必要
#include <array>        // std::array のために必要

#include "Application.h"
#include "Character.h"
#include "Minigame2.h"

Minigame2::Minigame2(void) : nowState(STATE_INIT), betMoney(0), playerRole(ROLE_NONE), cpuRole(ROLE_NONE), mouseX(0), mouseY(0), prevMouseLeft(0), nowMouseLeft(0), rollTimer(0), resultDisplayTimer(0), gameImage(-1), rollImage(-1), continueImage(-1), quitImage(-1), numberImage(-1), inputBetString("") {

	for (int i = 0; i < 6; i++) {
		diceImage[i] = -1;
	}
}

Minigame2::~Minigame2(void) {

}

bool Minigame2::SystemInit(void) {

	int dImg = LoadDivGraph("image/dice.png", 6, 6, 1, DICE_SIZE, DICE_SIZE, diceImage);
	if (dImg == -1) return false;

	gameImage = LoadGraph("image/minigameFrame.png");
	rollImage = LoadGraph("image/button-roll.png");
	continueImage = LoadGraph("image/button-continue.png");
	quitImage = LoadGraph("image/button-quit.png");
	numberImage = LoadGraph("image/number.png");
	if (gameImage == -1 || rollImage == -1 || continueImage == -1 || quitImage == -1 || numberImage == -1) return false;

	GameInit();		//初回のゲームの状態をBETTINGにする

	return true;
}

//ゲームの初期化(各勝負ごと)
void Minigame2::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_MINIGAME_2;

	int nowMoney = Character::getInst()->GetMoney();
	if (nowMoney <= 0) {
		nowState = STATE_GAMEOVER;		//所持金0以下でゲームオーバー
		return;
	}

	//掛け金、役、出目をリセット
	betMoney = 0;
	inputBetString = "";
	playerRole = ROLE_NONE;
	cpuRole = ROLE_NONE;
	playerDice.fill(0);
	cpuDice.fill(0);

	rollTimer = 0;
	resultDisplayTimer = 0;
	messageText = "";

	//掛け金設定に移行
	nowState = STATE_BETTING;
}

void Minigame2::Update(void) {

	//マウスの状態
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得

	switch (nowState) {
	case STATE_INIT:
		GameInit();
		break;
	case STATE_BETTING:
		UpdateBetting();
		break;
	case STATE_PLAYER_ROLL:
		UpdatePlayerRoll();
		break;
	case STATE_CPU_ROLL:
		UpdateCpuRoll();
		break;
	case STATE_RESULT:
		UpdateResult();
		break;
	case STATE_SELECT_NEXT:
		UpdateSelectNext();
		break;
	case STATE_GAMEOVER:
		nextScene_ID = E_SCENE_GAME;
		break;
	}

	//debug
	prev4Key = now4Key;
	now4Key = CheckHitKey(KEY_INPUT_4);
	if (prev4Key == 1 && now4Key == 0) {
		nextScene_ID = E_SCENE_TITLE;
	}
}

void Minigame2::Draw(void) {

	DrawGraph(0, 0, gameImage, true);

	switch (nowState) {
	case STATE_BETTING:
		DrawBetting();
		break;
	case STATE_PLAYER_ROLL:
		DrawDice(playerDice, 100, 200);
		DrawString(44, 250, "サイコロを振ってください！", GetColor(0, 0, 0));
		DrawGraph(120, 400, rollImage, true);
		break;
	case STATE_CPU_ROLL:
		DrawDice(cpuDice, 100, 200);
		DrawString(24, 160, "相手がサイコロを振っています…", GetColor(0, 0, 0));
		break;
	case STATE_RESULT:
		DrawResult();
		break;
	case STATE_SELECT_NEXT:
		DrawSelectNext();
		break;
	case STATE_GAMEOVER:
		DrawString(44, 300, "おうちに帰ります", GetColor(0, 0, 0));
		break;
	}

	DrawUI();
}

bool Minigame2::Release(void) {

	for (int i = 0; i < 6; i++) {
		if (diceImage[i] != -1) {
			DeleteGraph(diceImage[i]);
			diceImage[i] = -1;
		}
	}

	if (gameImage != -1) DeleteGraph(gameImage);
	if (rollImage != -1) DeleteGraph(rollImage);
	if (continueImage != -1) DeleteGraph(continueImage);
	if (quitImage != -1) DeleteGraph(quitImage);
	if (numberImage != -1) DeleteGraph(numberImage);

	return true;
}

//サイコロを振る
void Minigame2::RollDice(std::array<int, 3>& dice) {

	for (int i = 0; i < 3; ++i) {
		dice[i] = GetRand(5) + 1;	//+1して1から6に
	}
}

//役を判定
int Minigame2::GetRole(const std::array<int, 3>& dice) {

	//出目をソート
	std::array<int, 3> sortedDice = dice;
	std::sort(sortedDice.begin(), sortedDice.end());

	//ゾロ目判定
	if (sortedDice[0] == sortedDice[1] && sortedDice[1] == sortedDice[2]) {
		switch (sortedDice[0]) {
		case 1: return ROLE_PINZORO;
		case 2: return ROLE_2ZORO;
		case 3: return ROLE_3ZORO;
		case 4: return ROLE_4ZORO;
		case 5: return ROLE_5ZORO;
		case 6: return ROLE_6ZORO;
		}
	}

	//シゴロ判定
	if (sortedDice[0] == 4 && sortedDice[1] == 5 && sortedDice[2] == 6) {
		return ROLE_456;
	}

	//ヒフミ判定
	if (sortedDice[0] == 1 && sortedDice[0] == 2 && sortedDice[0] == 3) {
		return ROLE_123;
	}

	//同じ目が2つ
	if (sortedDice[0] == sortedDice[1] && sortedDice[1] != sortedDice[2]) {
		return sortedDice[2] + (ROLE_1EYE - 1);
	}
	if (sortedDice[1] == sortedDice[2] && sortedDice[0] != sortedDice[1]) {
		return sortedDice[0] + (ROLE_1EYE - 1);
	}

	//役無し
	return ROLE_NONE;
}

//役の強さ判定
int Minigame2::GetRoleStrength(int role, const std::array<int, 3>& dice) {

	//シゴロ > ピンゾロ > その他ゾロ目(数字が大きいほど強い) > 各種目(数字が大きいほど強い) > ヒフミ
	switch (role) {
	case ROLE_PINZORO: return 200;
	case ROLE_2ZORO: return 100;
	case ROLE_3ZORO: return 101;
	case ROLE_4ZORO: return 102;
	case ROLE_5ZORO: return 103;
	case ROLE_6ZORO: return 104;
	case ROLE_456: return 500;
	case ROLE_123: return 1;
	case ROLE_1EYE: return 10;
	case ROLE_2EYE: return 11;
	case ROLE_3EYE: return 12;
	case ROLE_4EYE: return 13;
	case ROLE_5EYE: return 14;
	case ROLE_6EYE: return 15;
	default: return 0;
	}
}

//役に応じた倍率を獲得
int Minigame2::GetBetRate(int role) {

	switch (role) {
	case ROLE_PINZORO: return 5;
	case ROLE_2ZORO: return 2;
	case ROLE_3ZORO: return 2;
	case ROLE_4ZORO: return 2;
	case ROLE_5ZORO: return 2;
	case ROLE_6ZORO: return 2;
	case ROLE_456: return 5;
	case ROLE_123: return -2;
	case ROLE_1EYE: return 1;
	case ROLE_2EYE: return 1;
	case ROLE_3EYE: return 1;
	case ROLE_4EYE: return 1;
	case ROLE_5EYE: return 1;
	case ROLE_6EYE: return 1;
	default: return 0;	//役無し
	}
}

//掛け金の設定
void Minigame2::UpdateBetting(void) {

	const int keypadX = 44;
	const int keypadY = 180;

	HandleKeypadInput(keypadX, keypadY);
}

//テンキー入力処理
void Minigame2::HandleKeypadInput(int keypadX, int keypadY) {

	if (prevMouseLeft == 1 && nowMouseLeft == 0) {
		int keypadRelativeX = mouseX - keypadX;		//テンキー座標、テンキーの左上の座標を0とした相対座標
		int keypadRelativeY = mouseY - keypadY;

		//画像範囲外のとき
		if (keypadRelativeX < 0 || keypadRelativeX >= 200 || keypadRelativeY < 0 || keypadRelativeY >= 266) {
			return;
		}

		int buttonValue = -99;		//何も押されていない時の初期値、ありえない数値ならなんでも

		const int buttonSize = Minigame2::KEY_SIZE + Minigame2::KEY_BORDER;	//ボタンと枠の合計

		/*
		テンキーレイアウト
		7, 8, 9
		4, 5, 6
		1, 2, 3
		0, 削除, 決定
		*/

		//列と行を計算
		int col = keypadRelativeX / buttonSize;
		int row = keypadRelativeY / buttonSize;

		//各ボタンの領域内にあるか、枠部分を除外
		int innerX = keypadRelativeX % buttonSize;
		int innerY = keypadRelativeY % buttonSize;
		if (innerX >= Minigame2::KEY_SIZE || innerY >= Minigame2::KEY_SIZE) {		//枠をクリックした
			return;
		}

		//レイアウトに基づいて値を割り当てる
		if (row == 0) {		//1段目：7、8、9
			if (col == 0) buttonValue = 7;
			else if (col == 1) buttonValue = 8;
			else if (col == 2) buttonValue = 9;
		}
		else if (row == 1) {	//2段目：4、5、6
			if (col == 0) buttonValue = 4;
			else if (col == 1) buttonValue = 5;
			else if (col == 2) buttonValue = 6;
		}
		else if (row == 2) {	//3段目：1、2、3
			if (col == 0) buttonValue = 1;
			else if (col == 1) buttonValue = 2;
			else if (col == 2) buttonValue = 3;
		}
		else if (row == 3) {	//4段目：0、削除、決定
			if (col == 0) buttonValue = 0;
			else if (col == 1) buttonValue = -1;
			else if (col == 2) buttonValue = -2;
		}

		//ボタンに応じた処理
		if (buttonValue >= 0 && buttonValue <= 9) {		//数字キー
			if (inputBetString.length() < 9) {			//入力文字列の長さ制限、9桁
				if (inputBetString == "0") {			//入力が0のとき、0000となるのを防ぐ
					inputBetString = std::to_string(buttonValue);	//0を置き換え
				}
				else {
					inputBetString += std::to_string(buttonValue);	//数字を文字列に追加
				}

				//所持金の上限確認
				long long tempBet = 0;
				try {
					tempBet = std::stoll(inputBetString);	//longlongで一旦受ける
				}
				catch (const std::out_of_range& eo) {
					tempBet = std::numeric_limits<long long>::max();
				}
				/*
				try{ } catch{ }　→例外処理
				tryで例外が発生した場合、プログラムがクラッシュするのではなく、対応するcatchに処理が移る
				ここでは、long longで宣言した変数にinputBetStringをわたす
				その際、引数が有効な数値形式でない、もしくは表現できる最大値を超える場合、std::stollは例外をスローする
				std::out_of_rangeはstd::stollなどが数値変換を行う際に、変換しようとした値が目的の数値型の範囲外であるときにスローされる例外
				ここではstd::numeric_limits<long long>::max()でlong long型が格のできる最大値を返している
				*/

				if (tempBet > Character::getInst()->GetMoney()) {
					betMoney = Character::getInst()->GetMoney();
					inputBetString = std::to_string(betMoney);
					messageText = "所持金が上限です！";
				}
				else if (tempBet > std::numeric_limits<int>::max()) {	//intの最大値も超えないように
					betMoney = std::numeric_limits<int>::max();
					inputBetString = std::to_string(betMoney);
					messageText = "掛け金が最大値を超えました！";
				}
				else {
					betMoney = static_cast<int>(tempBet);
					messageText = "";
				}
			}
			else {
				messageText = "これ以上入力できません！";
			}
		}
		else if (buttonValue == -1) {		//1文字削除
			if (!inputBetString.empty()) {
				inputBetString.pop_back();	//最後の1文字を削除
				if (!inputBetString.empty()) {
					betMoney = std::stoi(inputBetString);
				}
				else {
					betMoney = 0;
				}
				messageText = "";
			}
		}
		else if (buttonValue == -2) {		//掛け金決定
			if (betMoney > 0 && betMoney <= Character::getInst()->GetMoney()) {			//掛け金が0より大きく、所持金以下のとき決定可能
				nowState = STATE_PLAYER_ROLL;
				messageText = "サイコロを振ってください";
				rollTimer = 0;
			}
			else if (betMoney == 0) {
				messageText = "掛け金を設定してください";
			}
			else if (betMoney > Character::getInst()->GetMoney()) {
				messageText = "所持金が足りません！";
			}
		}
	}
}

//プレイヤーのサイコロ振り
void Minigame2::UpdatePlayerRoll(void) {

	int buttonx = 120;
	int buttony = 400;
	int buttonWid = 48;
	int buttonHig = 32;

	if (mouseX >= buttonx && mouseX <= buttonx + buttonWid && mouseY >= buttony && mouseY <= buttony + buttonHig && prevMouseLeft == 1 && nowMouseLeft == 0) {
		RollDice(playerDice);
		playerRole = GetRole(playerDice);
		nowState = STATE_CPU_ROLL;
		messageText = "相手がサイコロを振っています…";
		rollTimer = 0;
	}
}

//CPUのサイコロ振り
void Minigame2::UpdateCpuRoll(void) {

	rollTimer++;
	if (rollTimer > 120) {
		RollDice(cpuDice);
		cpuRole = GetRole(cpuDice);
		nowState = STATE_RESULT;
		resultDisplayTimer = 0;
	}
}

//結果表示、勝敗判定と所持金の更新
void Minigame2::UpdateResult(void) {

	messageText = "";
	resultDisplayTimer++;
	if (resultDisplayTimer < 480) return;	//8秒表示

	int plPayput = 0;

	//どちらか、もしくは両方役無しの時
	if (playerRole == ROLE_NONE && cpuRole != ROLE_NONE) {
		//プレイヤーの負け
		plPayput = -betMoney * GetBetRate(cpuRole);
	}
	else if (playerRole != ROLE_NONE && cpuRole == ROLE_NONE) {
		//プレイヤーの勝ち
		plPayput = betMoney * GetBetRate(playerRole);
	}
	else if (playerRole == ROLE_NONE && cpuRole == ROLE_NONE) {
		//引き分け、所持金の変動なし
		plPayput = 0;
	}
	//両方役あり
	else {
		int plStrength = GetRoleStrength(playerRole, playerDice);
		int cpuStrength = GetRoleStrength(cpuRole, cpuDice);

		if (plStrength > cpuStrength) plPayput = betMoney * GetBetRate(playerRole);
		else if (plStrength < cpuStrength) plPayput = -betMoney * GetBetRate(cpuRole);
		else plPayput = 0;
	}

	//どちらかがヒフミ(確定負け)
	if (playerRole == ROLE_123) plPayput = betMoney * GetBetRate(ROLE_123);
	if (cpuRole == ROLE_123) plPayput = betMoney * -GetBetRate(ROLE_123);

	Character::getInst()->AddMoney(plPayput);		//所持金の更新
	if (Character::getInst()->GetMoney() <= 0) {
		nowState = STATE_GAMEOVER;
	}
	else {
		nowState = STATE_SELECT_NEXT;					//次の状態へ移行
	}
}

//次の選択
void Minigame2::UpdateSelectNext(void) {

	//継続
	int continuex = 15;
	int continuey = 400;
	int buttonWid = 120;
	int buttonHig = 60;
	if (mouseX >= continuex && mouseX <= continuex + buttonWid && mouseY >= continuey && mouseY <= continuey + buttonHig && prevMouseLeft == 1 && nowMouseLeft == 0) {
		GameInit();
		return;			//BETTINGに戻るのでここでreturn
	}

	//やめる
	int quitx = 153;
	int quity = 400;
	if (mouseX >= quitx && mouseX <= quitx + buttonWid && mouseY >= quity && mouseY <= quity + buttonHig && prevMouseLeft == 1 && nowMouseLeft == 0) {
		nowState = STATE_GAMEOVER;
	}
}

//掛け金設定の描画
void Minigame2::DrawBetting(void) {

	const int keypadX = 44;
	const int keypadY = 180;

	DrawGraph(keypadX, keypadY, numberImage, true);

	//仮、後で変更
	DrawString(44, 456, "掛け金を設定してください", GetColor(0, 0, 0));
	DrawFormatString(44, 472, GetColor(0, 0, 0), "現在の掛け金: %s", inputBetString.c_str());
	DrawFormatString(44, 488, GetColor(0, 0, 0), "%s", messageText.c_str());	//エラーメッセージ
	
}

//サイコロの描画
void Minigame2::DrawDice(const std::array<int, 3>& dice, int x, int y) {

	for (int i = 0; i < 3; ++i) {
		if (dice[i] >= 1 && dice[i] <= 6) {
			DrawGraph(x + i * (DICE_SIZE + 10), y, diceImage[dice[i] - 1], true);
		}
	}
}

//結果表示の描画
void Minigame2::DrawResult(void) {

	//プレイヤーのサイコロと役
	DrawString(44, 160, "あなたの出目:", GetColor(0, 0, 0));
	DrawDice(playerDice, 44, 190);
	DrawFormatString(44, 250, GetColor(0, 0, 0), "あなたの役: %s", GetRoleName(playerRole).c_str());

	//CPUのサイコロと役
	DrawString(44, 310, "CPUの出目:", GetColor(0, 0, 0));
	DrawDice(cpuDice, 44, 340);
	DrawFormatString(44, 400, GetColor(0, 0, 0), "CPUの役: %s", GetRoleName(cpuRole).c_str());
	
	//勝敗メッセージ
	DrawString(44, 432, messageText.c_str(), GetColor(0, 0, 0));
}

//次の選択の描画
void Minigame2::DrawSelectNext(void) {

	DrawString(44, 300, "次の勝負をしますか？", GetColor(0, 0, 0));

	//「継続する」ボタン
	DrawGraph(15, 400, continueImage, true);
	//「メインに戻る」ボタン
	DrawGraph(153, 400, quitImage, true);
}

//UIの描画
void Minigame2::DrawUI(void) {

	//所持金の表示
	DrawFormatString(100, 140, GetColor(0, 0, 0), "所持金：%d", Character::getInst()->GetMoney());
	//デバッグ用
	DrawFormatString(10, 30, GetColor(0, 0, 0), "状態: %d", nowState);
}

//役の名前を取得
std::string Minigame2::GetRoleName(int role) const {

	switch (role) {
	case ROLE_NONE: return "役無し";
	case ROLE_PINZORO: return "ピンゾロ";
	case ROLE_2ZORO: return "二ゾロ";
	case ROLE_3ZORO: return "三ゾロ";
	case ROLE_4ZORO: return "四ゾロ";
	case ROLE_5ZORO: return "五ゾロ";
	case ROLE_6ZORO: return "六ゾロ";
	case ROLE_456: return "シゴロ";
	case ROLE_123: return "ヒフミ";
	case ROLE_1EYE: return "一";
	case ROLE_2EYE: return "二";
	case ROLE_3EYE: return "三";
	case ROLE_4EYE: return "四";
	case ROLE_5EYE: return "五";
	case ROLE_6EYE: return "六";
	default: return "不明な役";
	}
}