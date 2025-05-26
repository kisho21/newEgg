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
	// ƒQ[ƒ€‚Ìó‘Ô
	enum GameState {
		PLAYING,
		GAME_OVER,
		GAME_CLEAR
	};
	GameState nowGameState = PLAYING;

	eScene_ID nextScene_ID;				//Ÿ‚É‘JˆÚ‚·‚éƒV[ƒ“ID
};