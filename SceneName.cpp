//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>
#include <string>
#include <vector>	//仮想キーボードのボタン管理用

#include "Application.h"
#include "SceneName.h"
#include "Character.h"

//日本語文字はUTF-8で扱うため、string型で管理
std::vector<VirtualKey> virtualKeyboardKeys;

SceneName::SceneName() {

	nextScene_ID = E_SCENE_NAME;
	enteredName = "";
	maxNameLength = 5;
	fontHandle = -1;

	confirmButtonImage = -1;
	pushedConfirmButtonImage = -1;
	onConfirmButton = false;
}

SceneName::~SceneName() {

	if (fontHandle != -1) DeleteFontToHandle(fontHandle);
	if (confirmButtonImage != -1) DeleteGraph(confirmButtonImage);
	if (pushedConfirmButtonImage != -1) DeleteGraph(pushedConfirmButtonImage);

	//仮想キーボードのフォントハンドルを解放
	for (auto& key : virtualKeyboardKeys) {
		if (key.fontHandle != -1) {
			DeleteFontToHandle(key.fontHandle);
		}
	}
	//ベクターをクリア
	virtualKeyboardKeys.clear();
}

bool SceneName::SystemInit(void) {

	fontHandle = CreateFontToHandle("ＭＳ 明朝", 16, 5, DX_FONTTYPE_NORMAL);
	if (fontHandle == -1) return false;

	//確定ボタンの画像
	confirmButtonImage = LoadGraph("image/button_confirm.png");
	if (confirmButtonImage == -1) return false;
	pushedConfirmButtonImage = LoadGraph("image/pushed_confirm.png");
	if (pushedConfirmButtonImage == -1) return false;

	GameInit();

	return true;
}

void SceneName::GameInit(void) {

	enteredName = "";
	nextScene_ID = E_SCENE_NAME;

	mouseX = mouseY = 0;
	prevMouseLeft = nowMouseLeft = 0;

	//確定ボタンの座標
	int confirmButtonWidth = 120;
	int confirmButtonHeight = 40;
	confirmButtonX = Application::WINDOW_SIZE_WID / 2 - confirmButtonWidth / 2;
	confirmButtonY = Application::WINDOW_SIZE_HIG - confirmButtonHeight - 20;

	onConfirmButton = false;

	//仮想キーボードの初期化
	virtualKeyboardKeys.clear();

	int keyWid = 36;
	int keyHig = 36;
	int startX = (Application::WINDOW_SIZE_WID - (keyWid * 7 + 5 * 6)) / 2;
	int startY = 160;
	int gapX = 5;
	int gapY = 5;

	int nowX = startX;
	int nowY = startY;

	//1行目
	std::string hiragana1[] = { "あ","い","う","え","お","か","き" };
	for (int i = 0; i < sizeof(hiragana1) / sizeof(hiragana1[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana1[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//2行目
	std::string hiragana2[] = { "く","け","こ","さ","し","す","せ" };
	for (int i = 0; i < sizeof(hiragana2) / sizeof(hiragana2[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana2[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//3行目
	std::string hiragana3[] = { "そ","た","ち","つ","て","と","な" };
	for (int i = 0; i < sizeof(hiragana3) / sizeof(hiragana3[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana3[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//4行目
	std::string hiragana4[] = { "に","ぬ","ね","の","は","ひ","ふ" };
	for (int i = 0; i < sizeof(hiragana4) / sizeof(hiragana4[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana4[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//5行目
	std::string hiragana5[] = { "へ","ほ","ま","み","む","め","も" };
	for (int i = 0; i < sizeof(hiragana5) / sizeof(hiragana5[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana5[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//6行目
	std::string hiragana6[] = { "や","ゆ","よ","ら","り","る","れ" };
	for (int i = 0; i < sizeof(hiragana6) / sizeof(hiragana6[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana6[i],nowX,nowY,keyWid,keyHig,false,true,CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//7行目
	std::string hiragana7[] = { "ろ","わ","を","ん","ー","！","？" };
	for (int i = 0; i < sizeof(hiragana7) / sizeof(hiragana7[0]); ++i) {
		virtualKeyboardKeys.push_back({ 0,hiragana7[i],nowX,nowY,keyWid,keyHig,false,true, CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });
		nowX += keyWid + gapX;
	}
	nowY += keyHig + gapY;
	nowX = startX;

	//バックスペースキー
	virtualKeyboardKeys.push_back({ 0,"←",nowX,nowY,keyWid + 20, keyHig, true, false, CreateFontToHandle("ＭＳ 明朝", 20, 5, DX_FONTTYPE_NORMAL) });

	//フォント作成失敗チェック
	for (const auto& key : virtualKeyboardKeys) {
		if (key.fontHandle == -1) {
			return;
		}
	}
}

void SceneName::Update(void) {

	prevMouseLeft = nowMouseLeft;
	nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);
	GetMousePoint(&mouseX, &mouseY);

	//キー入力処理
	if (prevMouseLeft == 1 && nowMouseLeft == 0) {
		for (const auto& key : virtualKeyboardKeys) {
			if (mouseX >= key.x && mouseX <= key.x + key.width && mouseY >= key.y && mouseY <= key.y + key.height) {
				//キーがクリックされた
				if (key.isBackspace) {
					if (enteredName.length() > 0) {
						enteredName.pop_back();		//とりあえずバイト単位で削除
					}
				}
				else if (enteredName.length() < maxNameLength * 3) {
					//ひらがなやその他の文字を追加
					enteredName += key.displayString;
				}
				break;
			}
		}
	}

	//確定ボタンの処理
	int buttonWid = 120;
	int buttonHig = 40;

	if (mouseX >= confirmButtonX && mouseX <= confirmButtonX + buttonWid && mouseY >= confirmButtonY && mouseY <= confirmButtonY + buttonHig) {
		onConfirmButton = true;
		if (prevMouseLeft == 1 && nowMouseLeft == 0) {
			//名前確定
			Character::getInst()->SetCharacterName(enteredName);

			//所持金無限
			if (enteredName == INFINITE_MONEY_NAME) {
				Character::getInst()->SetMoney(INFINITE_MONEY_AMOUNT);
				Character::getInst()->SetInfiniteMoneyMode(true);
			}
			else {
				//通常の所持金初期設定
				Character::getInst()->SetInfiniteMoneyMode(false);
			}

			nextScene_ID = E_SCENE_GAME;
		}
	}
	else {
		onConfirmButton = false;
	}
}

void SceneName::Draw(void) {

	//ガイドメッセージ
	DrawStringToHandle(Application::WINDOW_SIZE_WID / 2 - GetDrawStringWidthToHandle("なまえをにゅうりょくしてください", strlen("なまえをにゅうりょくしてください"), fontHandle) / 2, 50, "なまえをにゅうりょくしてください", 0xffffff, fontHandle);

	//入力中の名前を表示する枠
	int inputFieldWidth = 240;
	int inputFieldHeight = 40;
	int inputFieldX = Application::WINDOW_SIZE_WID / 2 - inputFieldWidth / 2;
	int inputFieldY = 100;
	DrawBox(inputFieldX, inputFieldY, inputFieldX + inputFieldWidth, inputFieldY + inputFieldHeight, GetColor(200, 200, 200), TRUE);
	DrawBox(inputFieldX, inputFieldY, inputFieldX + inputFieldWidth, inputFieldY + inputFieldHeight, GetColor(0, 0, 0), FALSE);

	//入力中の名前
	DrawStringToHandle(inputFieldX + (inputFieldWidth - GetDrawStringWidthToHandle(enteredName.c_str(), GetStringLength(enteredName.c_str()), fontHandle)) / 2 - 16, inputFieldY + (inputFieldHeight - GetFontSizeToHandle(fontHandle)) / 2, enteredName.c_str(), 0x000000, fontHandle);
	
	//仮想キーボードの描画
	for (const auto& key : virtualKeyboardKeys) {
		DrawBox(key.x, key.y, key.x + key.width, key.y + key.height, GetColor(150, 150, 150), TRUE);
		DrawBox(key.x, key.y, key.x + key.width, key.y + key.height, GetColor(50, 50, 50), FALSE); // 枠線

		DrawStringToHandle(key.x + (key.width - GetDrawStringWidthToHandle(key.displayString.c_str(), GetStringLength(key.displayString.c_str()), key.fontHandle)) / 2 - 10, key.y + (key.height - GetFontSizeToHandle(key.fontHandle)) / 2, key.displayString.c_str(), 0x000000, key.fontHandle);
	}

	//確認ボタンの描画
	if (onConfirmButton) {
		DrawGraph(confirmButtonX, confirmButtonY, pushedConfirmButtonImage, TRUE);
	}
	else {
		DrawGraph(confirmButtonX, confirmButtonY, confirmButtonImage, TRUE);
	}
}

bool SceneName::Release(void) {

	if (fontHandle != -1) DeleteFontToHandle(fontHandle);
	if (confirmButtonImage != -1) DeleteGraph(confirmButtonImage);
	if (pushedConfirmButtonImage != -1) DeleteGraph(pushedConfirmButtonImage);

	//仮想キーボードのフォントハンドル
	for (auto& key : virtualKeyboardKeys) {
		if (key.fontHandle != -1) {
			DeleteFontToHandle(key.fontHandle);
		}
	}
	virtualKeyboardKeys.clear();

	return true;
}