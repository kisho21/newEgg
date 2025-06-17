#pragma once

//列挙型定数
//シーン定義
enum eScene_ID {
	E_SCENE_non = -1,
	E_SCENE_TITLE,			//タイトル画面
	E_SCENE_GAME,			//ゲーム画面
	E_SCENE_MINIGAME_1,		//ミニゲーム1画面、2048
	E_SCENE_MINIGAME_2,		//ミニゲーム2画面、チンチロ

	E_SCENE_ID_MAX,
};

//フェードの状態
enum eFadeStateID {
	E_STAT_FADE_non = -1,
	E_STAT_FADE_OUT,		//フェードアウト(徐々に暗転)
	E_STAT_FADE_IN,			//フェードイン(徐々に明転)

	E_STAT_FADE_MAX,
};

//構造体定義
struct POSITION {
	int x;
	int y;
};