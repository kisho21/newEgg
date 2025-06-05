#pragma once


class Ui {
public:
	static constexpr int MENU_WID = 288;
	static constexpr int MENU_HIG = 97;
	static constexpr int ICON_SIZE = 64;

	//吹き出しのアニメーション
	static constexpr int BUBBLE_SIZE = 144;
	static constexpr int ANIM_NUM_BUBBLE = 2;
	static constexpr int MAX_BUBBLE = ANIM_NUM_BUBBLE;

	Ui(void);
	~Ui(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	void DrawMinigame(void);
	bool Release(void);

	int GetIconX(void) { return iconX; }
	int GetIconY(void) { return iconY; }
	int GetMenuY(void) { return menuY; }
	void SetIconY(int y) { iconY = y; }
	void SetMenuY(int y) { menuY = y; }

	bool GetCheckDown(void) { return checkDown; }
	void SetCheckDownF(void) { checkDown = false; }

	//bool SetMiniGame(void) { checkMinigame = true; }

private:
	int bubbleImage[MAX_BUBBLE];
	int upsideImage;																//画面上部の枠
	int coinImage;																	//お金アイコン画像
	int menuImage;																	//メニュー画像
	int minigameImage, shopImage, bagImage, setImage;								//メニューアイコン画像
	int pushedMinigameImage, pushedShopImage, pushedBagImage, pushedSetImage;		//押されたアイコン画像
	int mini1Image;																	//ミニゲームアイコン画像
	int downImage, pushedDownImage;													//メニュー降下ボタン画像

	int menuX, menuY;						//メニューの座標
	int iconX, iconY;						//アイコンの座標

	int frame;							//アニメーションの表示フレーム
	int frameAdjust;					//アニメーション表示の調整フレーム

	bool onMinigame, onShop, onBag, onSet, onDown;
	bool checkDown;
	//bool checkMinigame, checkShop, checkBag, checkSet, checkDown;

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数

	void BubbleAnim(void);
};