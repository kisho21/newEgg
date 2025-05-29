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
	mini1Image = -1;
	downImage = pushedDownImage = -1;
}

Ui::~Ui(void) {

}

bool Ui::SystemInit(void) {

	int bubble = LoadDivGraph("image/bubble.png", MAX_BUBBLE, ANIM_NUM_BUBBLE, 1, BUBBLE_SIZE, BUBBLE_SIZE, bubbleImage);
	if (bubble == -1) return false;

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

	mini1Image = LoadGraph("image/icon-2048.png");
	if (mini1Image == -1) return false;

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

	frame = 0;
	frameAdjust = 0;

	onMinigame = onShop = onBag = onSet = onDown =false;
	checkDown = false;

	prevMouseLeft = nowMouseLeft = 0;
}

void Ui::Update(void) {
	
	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);

	GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得

	BubbleAnim();
	
	//メニューの各種アイコンの処理
	if (mouseX >= iconX && mouseX <= iconX + ICON_SIZE && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onMinigame = true;
	else onMinigame = false;
	if (mouseX >= iconX + ICON_SIZE + 8 && mouseX <= iconX + ICON_SIZE * 2 + 8 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onShop = true;
	else onShop = false;
	if (mouseX >= iconX + ICON_SIZE * 2 + 16 && mouseX <= iconX + ICON_SIZE * 3 + 16 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onBag = true;
	else onBag = false;
	if (mouseX >= iconX + ICON_SIZE * 3 + 24 && mouseX <= iconX + ICON_SIZE * 4 + 24 && mouseY >= iconY && mouseY <= iconY + ICON_SIZE) onSet = true;
	else onSet = false;
	if (mouseX >= Application::WINDOW_SIZE_WID / 2 - 32 && mouseX <= Application::WINDOW_SIZE_WID / 2 + 32 && mouseY >= menuY + 400 && mouseY <= menuY + 432) onDown = true;
	else onDown = false;

	//checkDown = (onDown && prevMouseLeft == 0 && nowMouseLeft == 1) ? true : false;
	if (onDown && prevMouseLeft == 0 && nowMouseLeft == 1) checkDown = true;
}

void Ui::Draw(void) {

	DrawGraph(2, 3, upsideImage, true);
	DrawGraph(Application::WINDOW_SIZE_WID / 2 + 2, 3, upsideImage, true);
	DrawGraph(Application::WINDOW_SIZE_WID / 2 + 8, 7, coinImage, true);

	DrawGraph(6, 48, growthImage, true);				//成長ゲージ仮置き、ゲージを可変にする

	DrawGraph(130, 100, bubbleImage[frame], true);

	//メニューの描画
	DrawGraph(menuX, menuY, menuImage, true);
	if (onMinigame == false) DrawGraph(iconX, iconY, minigameImage, true);
	else DrawGraph(iconX, iconY, pushedMinigameImage, true);
	if (onShop == false) DrawGraph(iconX + ICON_SIZE + 8, iconY, shopImage, true);
	else DrawGraph(iconX + ICON_SIZE + 8, iconY, pushedShopImage, true);
	if (onBag == false) DrawGraph(iconX + ICON_SIZE * 2 + 16, iconY, bagImage, true);
	else DrawGraph(iconX + ICON_SIZE * 2 + 16, iconY, pushedBagImage, true);
	if (onSet == false) DrawGraph(iconX + ICON_SIZE * 3 + 24, iconY, setImage, true);
	else DrawGraph(iconX + ICON_SIZE * 3 + 24, iconY, pushedSetImage, true);
	//ミニゲームアイコンの描画
	DrawGraph(9, menuY + 120, mini1Image, true);
	//メニュー下降ボタンの設置
	if (onDown == false) DrawGraph(Application::WINDOW_SIZE_WID / 2 - 32, menuY + 400, downImage, true);
	else DrawGraph(Application::WINDOW_SIZE_WID / 2 - 32, menuY + 400, pushedDownImage, true);

	//デバッグ
	//if (checkDown)DrawBox(0, 0, 100, 100, 0xffffff, true);
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
	if (DeleteGraph(mini1Image) == -1) return false;
	if (DeleteGraph(downImage) == -1) return false;
	if (DeleteGraph(pushedDownImage) == -1) return false;

	return true;
}

void Ui:: BubbleAnim(void) {

	if (frameAdjust == 60) {
		frame++;
		frameAdjust = 0;
	}
	if (frame == MAX_BUBBLE) {
		frame = 0;
	}
	frameAdjust++;
}