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

	//int titleBgm;						//bgmを格納する変数

	int mouseX, mouseY;					//マウス座標
	int prevMouseLeft, nowMouseLeft;	//マウスの入力状態を格納する変数
	int prevSpaceKey, nowSpaceKey;		//キーの状態を格納する変数

	eScene_ID nextScene_ID;				//次に遷移するシーンID

	//デバッグ用
	int prev1Key, now1Key;
	int prev2Key, now2Key;
	int prev3Key, now3Key;
	int prev4Key, now4Key;
};