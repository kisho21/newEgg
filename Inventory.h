#pragma once

#include <vector>
#include <string>

struct InventoryItem {
	int id;
	std::string name;
	int quantity;
	int handle;
};

class Inventory {
public:
	Inventory();
	~Inventory();

	void Update(void);
	void Draw(void);
	void AddItem(int itemId, int quantity, int handle, const std::string& name);			//アイテム追加処理
	void RemoveItem(int itemId, int quantity);		//アイテム削除処理(アイテム使用時)

private:
	std::vector<InventoryItem> playerInventory;		//プレイヤーの持ち物リスト
	int selectedItem;								//選択中のアイテム

	/*std::string GetItemName(int id);
	int GetItemHandle(int id);*/
	int useImage;

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数
};