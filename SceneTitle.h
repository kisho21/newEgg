#pragma once

#include "StDefine.h"

class Application;

class SceneTitle {
public:
	static constexpr int START_BUTTON_WID = 200;
	static constexpr int START_BUTTON_HIG = 100;

	SceneTitle(void);
	~SceneTitle(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	eScene_ID GetNextSceneID(void) { return nextScene_ID; }

private:
	int titleImage;						//背景画像のハンドル番号
	int startImage;
	int pushedStartImage;

	int startX, startY;					//スタートボタンの座標
	bool onStart;						//カーソルがスタートボタン上にあるか
	bool checkStart;					//スタートボタンが押されているか

	//int titleBgm;						//bgmを格納する変数

	int mouseX, mouseY;					//マウス座標
	int prevMouseLeft, nowMouseLeft;	//マウスの入力状態を格納する変数
	int prevSpaceKey, nowSpaceKey;		//キーの状態を格納する変数

	eScene_ID nextScene_ID;				//次に遷移するシーンID

	//各処理の関数
	void CheckStartButton(void);		//スタートボタンの処理

	//デバッグ用
	int prev1Key, now1Key;
	int prev2Key, now2Key;
	int prev3Key, now3Key;
};