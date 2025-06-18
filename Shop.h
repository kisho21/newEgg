#pragma once

#include <vector>
#include <string>

class Inventory;

struct ShopItem {
	int id;
	std::string itemName;
	int price;
	int handle;
};

class Shop {
public:
	Shop();
	~Shop();

	void Update(Inventory* inventory);
	void Draw(void);

private:
	std::vector<ShopItem> shopItems;	//売っているアイテムリスト
	int selectedItem;					//選択中のアイテム
	int playerMoney;					//所持金Charaクラスから取得する

	int buyImage;	//購入ボタン

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数
};