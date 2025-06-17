#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>	//std::sortのために必要
#include <limits>		//std::numeric_limitsのために必要
#include <stdexcept>

#include "StDefine.h"

//ゲームの状態
enum State {
	STATE_INIT,			//初期化
	STATE_BETTING,		//掛け金設定
	STATE_PLAYER_ROLL,	//プレイヤーがサイコロを振る
	STATE_CPU_ROLL,		//CPUがサイコロを振る
	STATE_RESULT,		//結果表示
	STATE_SELECT_NEXT,	//次の選択(継続/メインへ)
	STATE_GAMEOVER		//ゲームオーバー
};

//チンチロの役
enum Role {
	ROLE_NONE,		//役無し
	ROLE_PINZORO,	//ピンゾロ
	ROLE_2ZORO,		//2ゾロ
	ROLE_3ZORO,		//3ゾロ
	ROLE_4ZORO,		//4ゾロ
	ROLE_5ZORO,		//5ゾロ
	ROLE_6ZORO,		//6ゾロ
	ROLE_456,		//シゴロ
	ROLE_123,		//ヒフミ
	ROLE_1EYE,		//1の目
	ROLE_2EYE,		//2の目
	ROLE_3EYE,		//3の目
	ROLE_4EYE,		//4の目
	ROLE_5EYE,		//5の目
	ROLE_6EYE		//6の目
};


class Minigame2 {
public:
	static constexpr int DICE_SIZE = 32;
	static constexpr int DICE_TOTAL_WID = 192;
	static constexpr int KEY_SIZE = 64;
	static constexpr int KEY_BORDER = 2;

	Minigame2(void);
	~Minigame2(void);

	bool SystemInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	bool IsGameOver(void) const { return nowState == STATE_GAMEOVER; }

	eScene_ID GetNextSceneID(void) { return nextScene_ID; }


private:
	State nowState;		//現在の状態

	int betMoney;		//掛け金
	std::string inputBetString;			//入力中の金額

	std::array<int, 3> playerDice;		//プレイヤーの出目
	std::array<int, 3> cpuDice;			//CPUの出目

	int playerRole;		//プレイヤーの役
	int cpuRole;		//CPUの役

	int diceImage[6];	//サイコロの画像

	int rollTimer;				//サイコロを振るタイマー
	int resultDisplayTimer;		//結果表示のタイマー

	int gameImage;						//ミニゲーム背景画像
	int rollImage;
	int continueImage;
	int quitImage;
	int numberImage;					//テンキー画像

	std::string messageText;

	int mouseX, mouseY;
	int prevMouseLeft, nowMouseLeft;	//マウスの入力状態を格納する変数

	eScene_ID nextScene_ID;				//次に遷移するシーンID

	void GameInit(void);							//ゲームの初期化(各勝負ごと)
	void RollDice(std::array<int, 3>& dice);		//サイコロを振る
	int GetRole(const std::array<int, 3>& dice);	//役を判定
	int GetRoleStrength(int role, const std::array<int, 3>& dice);	//役の強さ判定
	int GetBetRate(int role);						//役に応じた倍率を獲得
	void UpdateBetting(void);						//掛け金の設定
	void HandleKeypadInput(int keypadX, int keypadY);	//テンキー入力処理
	void UpdatePlayerRoll(void);					//プレイヤーのサイコロ振り
	void UpdateCpuRoll(void);						//CPUのサイコロ振り
	void UpdateResult(void);						//結果表示
	void UpdateSelectNext(void);					//次の選択
	void DrawBetting(void);							//掛け金設定の描画
	void DrawDice(const std::array<int, 3>& dice, int x, int y);	//サイコロの描画
	void DrawResult(void);							//結果表示の描画
	void DrawSelectNext(void);						//次の選択の描画
	void DrawUI(void);								//UIの描画
	std::string GetRoleName(int role) const;		//役の名前を取得
};