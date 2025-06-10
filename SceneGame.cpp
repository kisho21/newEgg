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

	growthImage = -1;
	gameImage = -1;

	//charaInst = nullptr;
	uiInit = nullptr;
}

SceneGame::~SceneGame(void) {

}

bool SceneGame::SystemInit(void) {

	//画像の読み込み
	growthImage = LoadGraph("image/growth.png");
	if (growthImage == -1) return false;
	gameImage = LoadGraph("image/game.png");
	if (gameImage == -1) return false;

	//charaInst = new Character();
	//if (charaInst == nullptr) return false;
	uiInit = new Ui();
	if (uiInit == nullptr) return false;

	//charaInst->SystemInit();
	Character::getInst()->SystemInit();
	uiInit->SystemInit();

	mouseX = mouseY = 0;

	GameInit();

	return true;
}

void SceneGame::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_GAME;

	//charaInst->GameInit();
	Character::getInst()->GameInit();
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

	SelectMenu();							//メニューの選択

	//状態ごとの更新処理
	switch (nowGameState) {
	case STATE_PLAY:
		//charaInst->Update();
		Character::getInst()->Update();
		uiInit->Update();

		break;
	case STATE_SELECTGAME:
		uiInit->Update();

		MenuSlide();		//メニューのスライド処理
		SelectGame();		//ミニゲームの選択
		break;
	case STATE_SHOP:
		uiInit->Update();

		MenuSlide();		//メニューのスライド処理

		break;
	case STATE_ITEM:
		uiInit->Update();

		MenuSlide();		//メニューのスライド処理

		break;

	case STATE_OPTION:
		uiInit->Update();

		MenuSlide();		//メニューのスライド処理

		break;
	}

	//デバッグ用
	prev1Key = now1Key;
	now1Key = CheckHitKey(KEY_INPUT_1);
	if (prev1Key == 1 && now1Key == 0) {
		//nextScene_ID = E_SCENE_TITLE;
		Character::getInst()->AddMoney(500);
	}
}

void SceneGame::Draw(void) {

	DrawGraph(7, 48, growthImage, true);				//成長ゲージ仮置き、ゲージを可変にする
	DrawGraph(0, 0, gameImage, true);

	//charaInst->Draw();
	Character::getInst()->Draw();
	uiInit->Draw();

	DrawFontProc();

	switch (nowGameState) {
	case STATE_PLAY:
		
		break;
	case STATE_SELECTGAME:
		uiInit->DrawMinigame();
		break;
	case STATE_SHOP:

		break;
	case STATE_ITEM:

		break;

	case STATE_OPTION:

		break;
	}

	//DrawBox(0, 0, 32, 32, 0xff0000, true);
	DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));		//マウス座標の場所に円の描画
	//DrawFormatString(0, 0, 0x000000, "%d", money);
	//if(nowGameState== STATE_SELECTGAME)DrawString(0, 0, "1", true);
	//else DrawString(0, 0, "0", true);
}

bool SceneGame::Release(void) {
	
	if (DeleteGraph(growthImage) == -1) return false;
	if (DeleteGraph(gameImage) == -1) return false;

	uiInit->Release();
	delete uiInit;
	uiInit = nullptr;

	return true;
}

//文字列の表示処理
void SceneGame::DrawFontProc(void) {

	int m = Character::getInst()->GetMoney();
	sprintf_s(moneyText, "%d", m);

	int textWidth = GetDrawStringWidth(moneyText, strlen(moneyText));
	int moneyX = Application::WINDOW_SIZE_WID - textWidth - 10;
	int moneyY = 16;

	DrawString(moneyX, moneyY, moneyText, 0x000000);
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

//メニューの選択
void SceneGame::SelectMenu(void) {

	int iconx = uiInit->GetIconX();
	int icony = uiInit->GetIconY();
	if (mouseX > iconx && mouseX < iconx + Ui::ICON_SIZE && mouseY > icony && mouseY < icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_SELECTGAME;
	else if (mouseX >= iconx + Ui::ICON_SIZE + 8 && mouseX <= iconx + Ui::ICON_SIZE * 2 + 8 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_SHOP;
	else if (mouseX >= iconx + Ui::ICON_SIZE * 2 + 16 && mouseX <= iconx + Ui::ICON_SIZE * 3 + 16 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_ITEM;
	else if (mouseX >= iconx + Ui::ICON_SIZE * 3 + 24 && mouseX <= iconx + Ui::ICON_SIZE * 4 + 24 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_OPTION;
}

//ミニゲームの選択
void SceneGame::SelectGame(void) {

	int mGameY = uiInit->GetMenuY() + 120;
	//2048を選択
	if (mouseX > 9 && mouseX < 93 && mouseY > mGameY && mouseY < mGameY + 84 && prevMouseLeft == 1 && nowMouseLeft == 0) {
		nextScene_ID = E_SCENE_MINIGAME_1;
	}
}