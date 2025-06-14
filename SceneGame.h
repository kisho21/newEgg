#pragma once

#include "StDefine.h"
#include "Text.h"

class Character;
class Ui;
class Text;

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
		STATE_SELECTGAME,					//ミニゲーム選択スライド
		STATE_SHOP,							//買い物スライド
		STATE_ITEM,							//アイテム一覧スライド
		STATE_OPTION						//設定スライド
	};
	GameState nowGameState = STATE_PLAY;	//現在のゲームの状態、初期プレイ中

	//各クラスのインスタンスのポインタの追加
	//Character* charaInst;
	Ui* uiInst;
	Text* tInst;

	int growthImage;						//成長ゲージ画像、仮置き段階
	int gameImage;							//ゲーム画面の背景画像

	char moneyText[32];						//所持金を表示させるのに十分なバッファ

	//int growthPoint;						//成長ポイント
	//int growthX;							//成長ゲージの長さ、X座標を変えることで表す

	CharaWant nowWant;	//現在の要望

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数

	int prevSpaceKey, nowSpaceKey;			//キーの状態を格納する変数

	eScene_ID nextScene_ID;					//次に遷移するシーンID

	void DrawFontProc(void);				//文字列の表示処理
	void MenuSlide(void);					//メニューのスライド処理
	void SelectMenu(void);					//メニューの選択
	void SelectGame(void);					//ミニゲームの選択

	//デバッグ用
	int prev1Key, now1Key;
};