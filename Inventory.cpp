//
//ゲーム : egg
//氏名   : 永吉
//
#include "DxLib.h"

#include "Application.h"
#include "Inventory.h"

Inventory::Inventory() :selectedItem(0) {

	useImage = LoadGraph("image/button-use.png");

	lastUsedItem = -1;
	itemUsedFlg = false;

	mouseX = mouseY = 0;
	prevMouseLeft = nowMouseLeft = 0;
}

Inventory::~Inventory() {

	if (useImage != -1) DeleteGraph(useImage);
}

void Inventory::Update() {

	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	GetMousePoint(&mouseX, &mouseY);						//マウスの座標を取得

	if (!playerInventory.empty()) {

		for (size_t i = 0; i < playerInventory.size(); ++i) {
			int buttonx = Application::WINDOW_SIZE_WID - 60;
			int buttony = 160 + i * 34;

			int buttonWid = 48;
			int buttonHig = 32;

			if (prevMouseLeft == 1 && nowMouseLeft == 0 && mouseX >= buttonx && mouseX <= buttonx + buttonWid && mouseY >= buttony && mouseY <= buttony + buttonHig) {
				//アイテムを使用(1つ削除)
				lastUsedItem = playerInventory[i].id;
				RemoveItem(playerInventory[i].id, 1);

				if (selectedItem >= playerInventory.size() && !playerInventory.empty()) {
					selectedItem = playerInventory.size() - 1;		//最後のアイテムを消したら表示を調整
				}
				else if (playerInventory.empty()) {
					selectedItem = 0;	//すべてのアイテムがなくなったら選択をリセット
				}

				itemUsedFlg = true;
				return;
			}
		}
	}
	else {
		selectedItem = 0;		//すべてのアイテムがなくなったら選択をリセット
	}
}

void Inventory::Draw() {

	if (playerInventory.empty()) {
		DrawString(10, 160, "アイテムはありません", 0x000000);
	}
	else {
		for (size_t i = 0; i < playerInventory.size(); i++) {
			if (playerInventory[i].handle != -1) {
				DrawGraph(10, 160 + i * 34, playerInventory[i].handle, true);
			}
			DrawFormatString(48, 160 + i * 34 + 8, 0x000000, "%s x %d", playerInventory[i].name.c_str(), playerInventory[i].quantity);
			if (useImage != -1) {
				int buttonx = Application::WINDOW_SIZE_WID - 60;
				int buttony = 160 + i * 34;
				DrawGraph(buttonx, buttony, useImage, true);
			}
		}
	}
}

void Inventory::AddItem(int itemId, int quantity, int handle, const std::string& name) {

	for (auto& item : playerInventory) {
		if (item.id == itemId) {
			item.quantity += quantity;
			return;
		}
	}

	playerInventory.push_back({ itemId,name,quantity,handle });		//新規の場合名前もセット
}

void Inventory::RemoveItem(int itemId, int quantity) {

	for (auto item = playerInventory.begin(); item != playerInventory.end(); ++item) {
		if (item->id == itemId) {
			item->quantity -= quantity;
			if (item->quantity <= 0) {
				playerInventory.erase(item);
			}
			return;
		}
	}
}

int Inventory::GetUsedItemId(void) {

	int id = lastUsedItem;
	lastUsedItem = -1;
	return id;
}