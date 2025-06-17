#pragma once

#include <string>

#include "StDefine.h"

struct VirtualKey {
	char displayChar;				//表示する文字
	std::string displayString;		//表示する文字列
	int x, y, width, height;		//ボタンの描画範囲
	bool isBackspace;				//バックスペースキーか
	bool isKanaInput;				//ひらがな入力か
	int fontHandle;					//各キー専用のフォントハンドル
};

class SceneName {
public:
	SceneName();
	~SceneName();

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	eScene_ID GetNextSceneID(void) { return nextScene_ID; }

private:
	std::string enteredName;
	int maxNameLength;

	int mouseX, mouseY;						//マウス座標
	int prevMouseLeft, nowMouseLeft;		//マウスの入力状態を格納する変数

	int confirmButtonImage;
	int pushedConfirmButtonImage;
	bool onConfirmButton;
	int confirmButtonX, confirmButtonY;
	int fontHandle;

	eScene_ID nextScene_ID;					//次に遷移するシーンID
};