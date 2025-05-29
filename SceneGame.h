#pragma once

#include "StDefine.h"

class Character;
class Ui;

class SceneGame {
public:
	SceneGame(void);
	~SceneGame(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	eScene_ID GetNextSceneID(void) { return nextScene_ID; }

	//int GetMoney(void) { return money; }
	//void SetMoney(int m) { money = m; }

private:
	//ゲームの状態を表す列挙型
	enum GameState {
		STATE_PLAY,							//プレイ中
		STATE_SELECTGAME,					//ミニゲーム選択中
		STATE_GUIDE,						//操作ガイド表示中
		STATE_PAUSE							//一時停止、余裕あればつくる
	};

	GameState nowGameState = STATE_PLAY;	//現在のゲームの状態、初期プレイ中

	//各クラスのインスタンスのポインタの追加
	Character* charaInst;
	Ui* uiInit;

	int gameImage;							//ゲーム画面の背景画像

	//bool uiFlg;							//UIの表示フラグ、ここに作るか未定、まだ宣言のみ

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数

	int prevSpaceKey, nowSpaceKey;			//キーの状態を格納する変数

	eScene_ID nextScene_ID;					//次に遷移するシーンID

	//デバッグ用
	int prev1Key, now1Key;
};