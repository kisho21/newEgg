//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "SceneGame.h"
#include "Character.h"
#include "Shop.h"
#include "Inventory.h"
#include "Ui.h"
#include "Text.h"

SceneGame::SceneGame(void) : nowWant(CharaWant::none) {

	growthImage = -1;
	gameImage = -1;
	textWindow = -1;

	sInst = nullptr;
	iInst = nullptr;
	uiInst = nullptr;
	tInst = nullptr;
}

SceneGame::~SceneGame(void) {

}

bool SceneGame::SystemInit(void) {

	//画像の読み込み
	growthImage = LoadGraph("image/growth.png");
	if (growthImage == -1) return false;
	gameImage = LoadGraph("image/game.png");
	if (gameImage == -1) return false;
	textWindow = LoadGraph("image/textWindow.png");
	if (textWindow == -1) return false;

	sInst = new Shop();
	if (sInst == nullptr) return false;
	iInst = new Inventory();
	if (iInst == nullptr) return false;
	uiInst = new Ui();
	if (uiInst == nullptr) return false;
	tInst = new Text();
	if (tInst == nullptr) return false;

	Character::getInst()->SystemInit();
	uiInst->SystemInit();

	mouseX = mouseY = 0;

	GameInit();

	return true;
}

void SceneGame::GameInit(void) {

	//次のシーンの初期設定
	nextScene_ID = E_SCENE_GAME;

	Character::getInst()->GameInit();
	uiInst->GameInit();

	growthX = Character::getInst()->GetGrowth();

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
	case STATE_PLAY: {
		Character::getInst()->Update();
		uiInst->Update();

		growthX = Character::getInst()->GetGrowth();

		int x = Character::getInst()->GetCharaX();
		int y = Character::getInst()->GetCharaY();
		int wantTo = Character::getInst()->GetWnatTo();
		int wantx = Character::getInst()->GetWantPosX();
		int wanty = Character::getInst()->GetWantPosY();
		int usedItemId = iInst->GetUsedItemId();
		if (wantTo == Character::play) {
			if (usedItemId == 1) {
				Character::getInst()->CharaPlay();
			}

			nowWant = static_cast<CharaWant>(0);
		}
		else if (wantTo == Character::eat) {
			if (usedItemId == 2) {
				Character::getInst()->CharaEat();
			}

			nowWant = static_cast<CharaWant>(1);
		}
		else if (wantTo == Character::toilet) {
			if (mouseX > wantx + 30 && mouseX < wantx + 64 && mouseY > wanty && mouseY < wanty + 64 && prevMouseLeft == 1 && nowMouseLeft == 0) {
				Character::getInst()->SetIdleF();
				Character::getInst()->SetWalkT();
			}

			nowWant = static_cast<CharaWant>(2);
		}
		else if (wantTo == Character::bath) {
			if (mouseX > wantx + 30 && mouseX < wantx + 64 && mouseY > wanty && mouseY < wanty + 64 && prevMouseLeft == 1 && nowMouseLeft == 0) {
				Character::getInst()->SetIdleF();
				Character::getInst()->SetWalkT();
			}

			nowWant = static_cast<CharaWant>(3);
		}
		else if (wantTo == Character::touch) {
			if (mouseX > x + 30 && mouseX < x + 98 && mouseY > y + 10 && mouseY < y + 120 && prevMouseLeft == 1 && nowMouseLeft == 0) {
				Character::getInst()->CharaTouth();
			}

			nowWant = static_cast<CharaWant>(4);
		}
		else if (wantTo == Character::normal) {
			nowWant = static_cast<CharaWant>(5);
		}

		break;
	}
	case STATE_SELECTGAME:
		uiInst->Update();

		MenuSlide();		//メニューのスライド処理
		SelectGame();		//ミニゲームの選択
		break;
	case STATE_SHOP:
		uiInst->Update();
		sInst->Update(iInst);

		MenuSlide();		//メニューのスライド処理

		break;
	case STATE_ITEM: {
		uiInst->Update();
		iInst->Update();

		if (iInst->ItemUsed()) {
			iInst->SetItemUsedFlgF();
			uiInst->SetCheckDownT();
		}

		MenuSlide();		//メニューのスライド処理

		break;
	}
	case STATE_OPTION:
		uiInst->Update();

		MenuSlide();		//メニューのスライド処理

		break;
	}

	//デバッグ用
	prev1Key = now1Key;
	now1Key = CheckHitKey(KEY_INPUT_1);
	if (prev1Key == 1 && now1Key == 0) {
		//nextScene_ID = E_SCENE_TITLE;
		Character::getInst()->AddGrowth(50);
	}
}

void SceneGame::Draw(void) {

	SetDrawBright(255, 255, 255);

	DrawGraph(growthX, 48, growthImage, true);
	DrawGraph(0, 0, gameImage, true);

	Character::getInst()->Draw();
	uiInst->Draw();

	DrawFontProc();

	switch (nowGameState) {
	case STATE_PLAY: {
		int x = Character::getInst()->GetWantPosX();
		int y = Character::getInst()->GetWantPosY();
		if (mouseX > x && mouseX < x + 64 && mouseY > y && mouseY < y + 64) {
			bool rFlg = Character::getInst()->GetRequestFlg();
			if (rFlg) DrawGraph(10, 370, textWindow, true);
			const std::string& displayText = tInst->GetCharacterWantString(nowWant);
			DrawString(16, 378, displayText.c_str(), 0x000000);
		}

		break;
	}
	case STATE_SELECTGAME:
		uiInst->DrawMinigame();
		break;
	case STATE_SHOP:
		sInst->Draw();

		break;
	case STATE_ITEM:
		iInst->Draw();

		break;

	case STATE_OPTION:

		break;
	}

	int g = Character::getInst()->GetGrowth();
	if (g >= 7) {
		Character::getInst()->GrownMax();
	}

	//DrawBox(0, 0, 32, 32, 0xff0000, true);
	//DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));		//マウス座標の場所に円の描画
	//DrawFormatString(0, 0, 0x000000, "%d", money);
	//if(nowGameState== STATE_SELECTGAME)DrawString(0, 0, "1", true);
	//else DrawString(0, 0, "0", true);

	/*int r = Character::getInst()->GetRequest();
	DrawFormatString(0, 0, 0x000000, "%d", r);*/
	/*int g = Character::getInst()->GetGrowth();
	DrawFormatString(0, 48, 0x000000, "成長度：%d", g);*/
}

bool SceneGame::Release(void) {
	
	if (DeleteGraph(growthImage) == -1) return false;
	if (DeleteGraph(gameImage) == -1) return false;

	uiInst->Release();
	delete uiInst;
	uiInst = nullptr;

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

	bool down = uiInst->GetCheckDown();
	if (!down) {
		int nexty = uiInst->GetMenuY() - 15;						//ここの実数値でスライドの速さ変更可能
		for (int y = uiInst->GetMenuY(); y >= nexty; y -= 5) {
			if (y <= 40) {
				y = 40;
				break;
			}

			uiInst->SetMenuY(y);
			uiInst->SetIconY(y + 16);
		}
		//break;
	}
	if (down) {
		int nexty = uiInst->GetMenuY() + 15;
		for (int y = uiInst->GetMenuY(); y <= nexty; y += 5) {
			if (y >= 420) {
				y = 415;
				nowGameState = STATE_PLAY;
				uiInst->SetCheckDownF();
				break;
			}

			uiInst->SetMenuY(y);
			uiInst->SetIconY(y + 16);
		}
		//break;
	}
}

//メニューの選択
void SceneGame::SelectMenu(void) {

	int iconx = uiInst->GetIconX();
	int icony = uiInst->GetIconY();
	if (mouseX > iconx && mouseX < iconx + Ui::ICON_SIZE && mouseY > icony && mouseY < icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_SELECTGAME;
	else if (mouseX >= iconx + Ui::ICON_SIZE + 8 && mouseX <= iconx + Ui::ICON_SIZE * 2 + 8 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_SHOP;
	else if (mouseX >= iconx + Ui::ICON_SIZE * 2 + 16 && mouseX <= iconx + Ui::ICON_SIZE * 3 + 16 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_ITEM;
	else if (mouseX >= iconx + Ui::ICON_SIZE * 3 + 24 && mouseX <= iconx + Ui::ICON_SIZE * 4 + 24 && mouseY >= icony && mouseY <= icony + Ui::ICON_SIZE && prevMouseLeft == 1 && nowMouseLeft == 0) nowGameState = STATE_OPTION;
}

//ミニゲームの選択
void SceneGame::SelectGame(void) {

	int mGameY = uiInst->GetMenuY() + 120;
	//2048を選択
	if (mouseX > 9 && mouseX < 93 && mouseY > mGameY && mouseY < mGameY + 84 && prevMouseLeft == 1 && nowMouseLeft == 0) {
		nextScene_ID = E_SCENE_MINIGAME_1;
	}
	//チンチロを選択
	if (mouseX > 102 && mouseX < 186 && mouseY > mGameY && mouseY < mGameY + 84 && prevMouseLeft == 1 && nowMouseLeft == 0) {
		nextScene_ID = E_SCENE_MINIGAME_2;
	}
}