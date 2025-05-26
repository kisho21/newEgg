//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(void) {

	titleImage = -1;
	startImage = -1;
	pushedStartImage = -1;

	//titleBgm = -1;
}

SceneTitle::~SceneTitle(void) {

}

bool SceneTitle::SystemInit(void) {

	titleImage = LoadGraph("image/title2.png");
	if (titleImage == -1) return false;
	startImage = LoadGraph("image/button-start2.png");
	if (startImage == -1) return false;
	pushedStartImage = LoadGraph("image/pushed-start2.png");
	if (pushedStartImage == -1) return false;

	//titleBgm = LoadSoundMem("bgm/title.mp3");

	
	mouseX = mouseY = 0;

	GameInit();

	return true;
}

void SceneTitle::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_TITLE;

	startX = Application::WINDOW_SIZE_WID / 2 - START_BUTTON_WID / 2;
	startY = 300;
	onStart = false;
	checkStart = false;

	prevMouseLeft = nowMouseLeft = 0;
	prevSpaceKey = nowSpaceKey = 0;
}

void SceneTitle::Update(void) {

	/*
	//bgmのループ処理
	if (CheckSoundMem(titleBgm) == 0) {
		PlaySoundMem(titleBgm, DX_PLAYTYPE_LOOP);
	}
	*/

	//キーの状態
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);


	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得
	CheckStartButton();						//スタートボタンの処理

	if (checkStart == true) {
		//StopSoundMem(titleBgm);
		nextScene_ID = E_SCENE_GAME;
	}

	//デバッグ用
	prev1Key = now1Key;
	now1Key = CheckHitKey(KEY_INPUT_1);
	if (prev1Key == 1 && now1Key == 0) {
		nextScene_ID = E_SCENE_GAME;
	}
	prev2Key = now2Key;
	now2Key = CheckHitKey(KEY_INPUT_2);
	if (prev2Key == 1 && now2Key == 0) {
		nextScene_ID = E_SCENE_MINIGAME_1;
	}
	prev3Key = now3Key;
	now3Key = CheckHitKey(KEY_INPUT_3);
	if (prev3Key == 1 && now3Key == 0) {
		//nextScene_ID = E_SCENE_EVENT;
	}
	/*
	1:ゲーム
	2:ミニゲーム
	3:
	4:
	5:
	*/
}

void SceneTitle::Draw(void) {


	//仮置き
	DrawGraph(0, 0, titleImage, true);
	if (onStart == false) DrawGraph(startX, startY, startImage, true);
	if (onStart == true) DrawGraph(startX, startY, pushedStartImage, true);

	//DrawString(100, Application::WINDOW_SIZE_HIG - 100, "スペースキーでスタート", 0x000000);
	DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));		//マウス座標の場所に円の描画
}

bool SceneTitle::Release(void) {

	if (DeleteGraph(titleImage) == -1) return false;
	if (DeleteGraph(startImage) == -1) return false;
	if (DeleteGraph(pushedStartImage) == -1) return false;

	//if (DeleteSoundMem(titleBgm) == -1) return false;

	return true;
}

//スタートボタンの処理
void SceneTitle::CheckStartButton(void) {

	if (mouseX >= startX && mouseX <= startX + START_BUTTON_WID && mouseY >= startY && mouseY <= startY + START_BUTTON_HIG) {
		onStart = true;

		if (prevMouseLeft == 1 && nowMouseLeft == 0) {
			checkStart = true;
		}
	}
	else {
			onStart = false;
	}
}