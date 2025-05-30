//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "SceneGame.h"
#include "Character.h"
#include "Ui.h"

SceneGame::SceneGame(void) {

	gameImage = -1;

	charaInst = nullptr;
	uiInit = nullptr;
}

SceneGame::~SceneGame(void) {

}

bool SceneGame::SystemInit(void) {

	//画像の読み込み
	gameImage = LoadGraph("image/game2.png");
	if (gameImage == -1) return false;

	charaInst = new Character();
	if (charaInst == nullptr) return false;
	uiInit = new Ui();
	if (uiInit == nullptr) return false;

	charaInst->SystemInit();
	uiInit->SystemInit();

	

	mouseX = mouseY = 0;

	GameInit();

	return true;
}

void SceneGame::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_GAME;

	charaInst->GameInit();
	uiInit->GameInit();

	prevMouseLeft = nowMouseLeft = 0;
	prevSpaceKey = nowSpaceKey = 0;
}

void SceneGame::Update(void) {

	//キーの状態
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	prevSpaceKey = nowSpaceKey;
	nowSpaceKey = CheckHitKey(KEY_INPUT_SPACE);

	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得

	//状態ごとの更新処理
	switch (nowGameState) {
	case STATE_PLAY:
		if (nowGameState == STATE_PLAY) {
			charaInst->Update();
			uiInit->Update();

			int iconx = uiInit->GetIconX();
			int icony = uiInit->GetIconY();
			if (mouseX > iconx && mouseX<iconx + 64 && mouseY>icony && mouseY < icony + 64 && prevMouseLeft == 1 && nowMouseLeft == 0) {
				nowGameState = STATE_SELECTGAME;
			}
		}
		break;
	case STATE_SELECTGAME:
		uiInit->Update();

		MenuSlide();		//メニューのスライド処理
		SelectGame();		//ミニゲームの選択
	}

	//デバッグ用
	prev1Key = now1Key;
	now1Key = CheckHitKey(KEY_INPUT_1);
	if (prev1Key == 1 && now1Key == 0) {
		nextScene_ID = E_SCENE_TITLE;
	}
}

void SceneGame::Draw(void) {

	DrawGraph(0, 0, gameImage, true);

	charaInst->Draw();
	uiInit->Draw();

	//DrawGuide();

	//DrawBox(0, 0, 32, 32, 0xff0000, true);
	DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));		//マウス座標の場所に円の描画
	//DrawFormatString(0, 0, 0x000000, "%d", money);
}

bool SceneGame::Release(void) {
	
	if (DeleteGraph(gameImage) == -1) return false;

	uiInit->Release();
	delete uiInit;
	uiInit = nullptr;

	charaInst->Release();
	delete charaInst;
	charaInst = nullptr;

	return true;
}

//メニューのスライド処理
void SceneGame::MenuSlide(void) {

	bool down = uiInit->GetCheckDown();
	if (!down) {
		int nexty = uiInit->GetMenuY() - 15;						//ここの実数値でスライドの速さ変更可能
		for (int y = uiInit->GetMenuY(); y >= nexty; y -= 5) {
			if (y <= 40) {
				y = 40;
				break;
			}

			uiInit->SetMenuY(y);
			uiInit->SetIconY(y + 16);
		}
		//break;
	}
	if (down) {
		int nexty = uiInit->GetMenuY() + 15;
		for (int y = uiInit->GetMenuY(); y <= nexty; y += 5) {
			if (y >= 420) {
				y = 415;
				nowGameState = STATE_PLAY;
				uiInit->SetCheckDownF();
				break;
			}

			uiInit->SetMenuY(y);
			uiInit->SetIconY(y + 16);
		}
		//break;
	}
}

//
void SceneGame::SelectGame(void) {

	int mGameY = uiInit->GetMenuY() + 120;
	//2048を選択
	if (mouseX > 9 && mouseX<93 && mouseY>mGameY && mouseY < mGameY + 84 && prevMouseLeft == 1 && nowMouseLeft == 0) {
		nextScene_ID = E_SCENE_MINIGAME_1;
	}
}