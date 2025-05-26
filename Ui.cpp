//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "Ui.h"

Ui::Ui(void) {

	upsideImage = -1;
	coinImage = -1;
	growthImage = -1;
	menuImage = -1;
	minigameImage = shopImage = bagImage = setImage = -1;
	pushedMinigameImage = pushedShopImage = pushedBagImage = pushedSetImage = -1;
	downImage = pushedDownImage = -1;
}

Ui::~Ui(void) {

}

bool Ui::SystemInit(void) {

	upsideImage = LoadGraph("image/UpsideWindow.png");
	if (upsideImage == -1) return false;
	coinImage = LoadGraph("image/coin2.png");
	if (coinImage == -1) return false;
	growthImage = LoadGraph("image/growth.png");
	if (growthImage == -1) return false;

	menuImage = LoadGraph("image/UiMenu.png");
	if (menuImage == -1) return false;

	minigameImage = LoadGraph("image/button-minigame.png");
	if (minigameImage == -1) return false;
	shopImage = LoadGraph("image/button-shop.png");
	if (shopImage == -1) return false;
	bagImage = LoadGraph("image/button-bag.png");
	if (bagImage == -1) return false;
	setImage = LoadGraph("image/button-setting.png");
	if (setImage == -1) return false;
	pushedMinigameImage = LoadGraph("image/pushed-minigame.png");
	if (pushedMinigameImage == -1) return false;
	pushedShopImage = LoadGraph("image/pushed-shop.png");
	if (pushedShopImage == -1) return false;
	pushedBagImage = LoadGraph("image/pushed-bag.png");
	if (pushedBagImage == -1) return false;
	pushedSetImage = LoadGraph("image/pushed-setting.png");
	if (pushedSetImage == -1) return false;
	downImage = LoadGraph("image/button-down.png");
	if (downImage == -1) return false;
	pushedDownImage = LoadGraph("image/pushed-down.png");
	if (pushedDownImage == -1) return false;

	mouseX = mouseY = 0;

	return true;
}

void Ui::GameInit(void) {

	menuX = 0;
	menuY = 415;
	iconX = 4;
	iconY = menuY + 16;

	onMinigame = onShop = onBag = onSet = false;

	prevMouseLeft = nowMouseLeft = 0;
}

void Ui::Update(void) {
	
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);

	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得
	
	//メニューの各種アイコンの処理
	if (mouseX >= iconX && mouseX <= iconX + ICON_SIZE && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onMinigame = true;
	else onMinigame = false;
	if (mouseX >= iconX + ICON_SIZE + 8 && mouseX <= iconX + ICON_SIZE * 2 + 8 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onShop = true;
	else onShop = false;
	if (mouseX >= iconX + ICON_SIZE * 2 + 16 && mouseX <= iconX + ICON_SIZE * 3 + 16 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onBag = true;
	else onBag = false;
	if (mouseX >= iconX + ICON_SIZE * 3 + 24 && mouseX <= iconX + ICON_SIZE * 4 + 24 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onSet = true;
	else onSet = false;
}

void Ui::Draw(void) {

	DrawGraph(2, 3, upsideImage, true);
	DrawGraph(Application::WINDOW_SIZE_WID / 2 + 2, 3, upsideImage, true);
	DrawGraph(Application::WINDOW_SIZE_WID / 2 + 8, 7, coinImage, true);

	DrawGraph(6, 48, growthImage, true);				//成長ゲージ仮置き、ゲージを可変にする

	DrawGraph(menuX, menuY, menuImage, true);

	if (onMinigame == false) DrawGraph(iconX, iconY, minigameImage, true);
	else DrawGraph(iconX, iconY, pushedMinigameImage, true);
	if (onShop == false) DrawGraph(iconX + ICON_SIZE + 8, iconY, shopImage, true);
	else DrawGraph(iconX + ICON_SIZE + 8, iconY, pushedShopImage, true);
	if (onBag == false) DrawGraph(iconX + ICON_SIZE * 2 + 16, iconY, bagImage, true);
	else DrawGraph(iconX + ICON_SIZE * 2 + 16, iconY, pushedBagImage, true);
	if (onSet == false) DrawGraph(iconX + ICON_SIZE * 3 + 24, iconY, setImage, true);
	else DrawGraph(iconX + ICON_SIZE * 3 + 24, iconY, pushedSetImage, true);
	//ここから、メニュー下降ボタンの設置
}

bool Ui::Release(void) {

	if (DeleteGraph(menuImage) == -1) return false;
	if (DeleteGraph(coinImage) == -1) return false;
	if (DeleteGraph(growthImage) == -1) return false;
	if (DeleteGraph(minigameImage) == -1) return false;
	if (DeleteGraph(shopImage) == -1) return false;
	if (DeleteGraph(bagImage) == -1) return false;
	if (DeleteGraph(setImage) == -1) return false;
	if (DeleteGraph(pushedMinigameImage) == -1) return false;
	if (DeleteGraph(pushedShopImage) == -1) return false;
	if (DeleteGraph(pushedBagImage) == -1) return false;
	if (DeleteGraph(pushedSetImage) == -1) return false;
	if (DeleteGraph(downImage) == -1) return false;
	if (DeleteGraph(pushedDownImage) == -1) return false;

	return true;
}