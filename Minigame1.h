#pragma once
#include <vector> // ゲームボードにstd::vectorを使う場合
#include <random> // 乱数生成
//#include <algorithm> // std::shuffleなど（必要に応じて）
#include <map>//キーと値のペアを格納する連想コンテナ、タイルの値と色を対応させる

#include "StDefine.h"

class Minigame1 {
public:
	static constexpr int SLIDE_RANGE = 66;
	static constexpr int MAX_NUMBERTILE = 11;

	//ゲームボードのサイズ
	static constexpr int BOARD_SIZE = 4;
	//タイルのサイズ（描画用）
	static constexpr int TILE_SIZE = 64;
	//グリッドの描画開始位置（左上）
	static constexpr int GRID_START_X = 9;
	static constexpr int GRID_START_Y = 140;
	//タイル間の隙間
	static constexpr int TILE_GAP = 2;

	Minigame1(void);
	~Minigame1(void);

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
		GAME_OVER,
		GAME_CLEAR
	};
	GameState nowGameState = PLAYING;

	std::map<int, int> tileColors;		//タイルの色を定義するマップ（数値に応じて色を変える）

	int board[BOARD_SIZE][BOARD_SIZE];	//ゲームボード
	long long score = 0;				//スコア

	int gameImage;						//ミニゲーム背景画像

	int mouseX, mouseY;					//マウス座標
	int prevMouseLeft, nowMouseLeft;	//マウスの入力状態を格納する変数

	eScene_ID nextScene_ID;				//次に遷移するシーンID

	//デバッグ
	int prev0Key, now0Key;
	int prevUpKey, prevDownKey, prevRightKey, prevLeftKey, nowUpKey, nowDownKey, nowRightKey, nowLeftKey;

	// --- 関数プロトタイプ宣言 ---
	void InitializeGame();
	void CreateNewTile();
	bool MoveTiles(int dx, int dy); // dx, dyは移動方向 (-1, 0, 1)
	void DrawGame();
	void DrawTile(int x, int y, int value);
	void UpdateGame();
	bool CheckGameOver();
	void SetupTileColors(); // タイルの色設定

	// 乱数生成器
	//std::random_device seed_gen;
	//std::mt19937 engine(seed_gen());
};