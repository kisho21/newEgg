#pragma once

#include "StDefine.h"

class Minigame2 {
public:

	Minigame2(void);
	~Minigame2(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	eScene_ID GetNextSceneID(void) { return nextScene_ID; }


private:
	// ゲームの状態
	enum GameState {
		PLAYING,
		GAME_OVER
	};
	GameState nowGameState = PLAYING;

	int mouseX, mouseY;
	int prevMouseLeft, nowMouseLeft;	//マウスの入力状態を格納する変数

	eScene_ID nextScene_ID;				//次に遷移するシーンID
};