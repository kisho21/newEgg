//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void) {

	titleImage = -1;
}

SceneTitle::~SceneTitle(void) {

}

bool SceneTitle::SystemInit(void) {

	titleImage = LoadGraph("image/title.png");
	if (titleImage == -1) return false;
	
	mouseX = mouseY = 0;

	GameInit();

	return true;
}

void SceneTitle::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_TITLE;

	prevMouseLeft = nowMouseLeft = 0;
	prevSpaceKey = nowSpaceKey = 0;
}

void SceneTitle::Update(void) {

	//キーの状態
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);


	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得

	if (prevMouseLeft == 1 && nowMouseLeft == 0) {
		nextScene_ID = E_SCENE_NAME;
	}

	//デバッグ用
	prev1Key = now1Key;
	now1Key = CheckHitKey(KEY_INPUT_1);
	if (prev1Key == 1 && now1Key == 0) {
		nextScene_ID = E_SCENE_NAME;
	}
	prev2Key = now2Key;
	now2Key = CheckHitKey(KEY_INPUT_2);
	if (prev2Key == 1 && now2Key == 0) {
		nextScene_ID = E_SCENE_GAME;
	}
	prev3Key = now3Key;
	now3Key = CheckHitKey(KEY_INPUT_3);
	if (prev3Key == 1 && now3Key == 0) {
		nextScene_ID = E_SCENE_MINIGAME_1;
	}
	prev4Key = now4Key;
	now4Key = CheckHitKey(KEY_INPUT_4);
	if (prev4Key == 1 && now4Key == 0) {
		nextScene_ID = E_SCENE_MINIGAME_2;
	}
	/*
	1:名前決め
	2:ゲーム
	3:ミニゲーム1
	4:ミニゲーム2
	5:
	*/
}

void SceneTitle::Draw(void) {

	//タイトル画像の描画
	DrawGraph(0, 0, titleImage, true);
	DrawString(73, 481, "- Click to Start -", 0x808080);
	DrawString(72, 480, "- Click to Start -", 0xffffff);
	
	//debug
	/*DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));*/		//マウス座標の場所に円の描画
}

bool SceneTitle::Release(void) {

	if (DeleteGraph(titleImage) == -1) return false;

	return true;
}