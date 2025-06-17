//
//ゲーム : egg
//氏名   : 永吉
//
#include "DxLib.h"

#include "Shop.h"
#include "Inventory.h"
#include "Character.h"
#include "Application.h"

Shop::Shop() : selectedItem(0) {

	playerMoney = Character::getInst()->GetMoney();

	//アイテムを追加
	shopItems.push_back({ 1,"ボール",100 ,LoadGraph("image/item/ball.png") });
	shopItems.push_back({ 2,"ご飯",100 ,LoadGraph("image/item/omu.png") });
	shopItems.push_back({ 3,"くすり",5000,LoadGraph("image/item/medicine.png") });

	//画像の読み込み
	buyImage = LoadGraph("image/button-buy.png");

	mouseX = mouseY = 0;
	prevMouseLeft = nowMouseLeft = 0;
}

Shop::~Shop() {

	//画像の解放
	for (const auto& item : shopItems) {
		if (item.handle != -1) {
			DeleteGraph(item.handle);
		}
	}
	if (buyImage != -1) DeleteGraph(buyImage);
}

void Shop::Update(Inventory* inventory) {

	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	GetMousePoint(&mouseX, &mouseY);						//マウスの座標を取得

	playerMoney = Character::getInst()->GetMoney();

	for (size_t i = 0; i < shopItems.size(); ++i) {
		//ボタンの描画位置
		int buttonx = Application::WINDOW_SIZE_WID - 60;
		int buttony = 160 + i * 34;

		int buttonWid = 48;
		int buttonHig = 32;

		if (prevMouseLeft == 1 && nowMouseLeft == 0 && mouseX >= buttonx && mouseX <= buttonx + buttonWid && mouseY >= buttony && mouseY <= buttony + buttonHig) {
			ShopItem& boughtItem = shopItems[i];
			if (playerMoney >= boughtItem.price) {
				Character::getInst()->AddMoney(-boughtItem.price);
				inventory->AddItem(boughtItem.id, 1, boughtItem.handle, boughtItem.itemName);				//インベントリにアイテム追加
			}
			else {
				//お金足りない処理
			}
			break;	//ボタンが複数回反応しないように、いらないかも
		}
	}

}

void Shop::Draw() {

	for (size_t i = 0; i < shopItems.size(); i++) {
		//アイテム画像の描画
		if (shopItems[i].handle != -1) {
			DrawGraph(10, 160 + i * 34, shopItems[i].handle, true);
		}

		//アイテム名と価格
		DrawFormatString(48, 160 + i * 34 + 8, 0x000000, "%s - %dG", shopItems[i].itemName.c_str(), shopItems[i].price);

		//購入ボタン
		if (buyImage != -1) {
			int buttonx = Application::WINDOW_SIZE_WID - 60;
			int buttony = 160 + i * 34;
			DrawGraph(buttonx, buttony, buyImage, true);
		}
	}
}