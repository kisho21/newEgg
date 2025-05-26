#pragma once


class Ui {
public:
	static constexpr int MENU_WID = 288;
	static constexpr int MENU_HIG = 97;
	static constexpr int ICON_SIZE = 64;

	Ui(void);
	~Ui(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	int GetIconX(void) { return iconX; }
	int GetIconY(void) { return iconY; }
	int GetMenuY(void) { return menuY; }
	void SetIconY(int y) { iconY = y; }
	void SetMenuY(int y) { menuY = y; }

	//bool SetMiniGame(void) { checkMinigame = true; }

private:
	//画面上部のウィンドウの追加←ここから
	int upsideImage;																//画面上部の枠
	int coinImage;																	//お金アイコン画像
	int growthImage;																//成長ゲージ画像、仮置き段階
	int menuImage;																	//メニュー画像
	int minigameImage, shopImage, bagImage, setImage;								//メニューアイコン画像
	int pushedMinigameImage, pushedShopImage, pushedBagImage, pushedSetImage;		//押されたアイコン画像
	int downImage, pushedDownImage;													//メニュー降下ボタン画像

	int menuX, menuY;						//メニューの座標
	int iconX, iconY;						//アイコンの座標

	bool onMinigame, onShop, onBag, onSet;
	bool checkMinigame, checkShop, checkBag, checkSet;

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数
};