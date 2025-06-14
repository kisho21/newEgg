//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Text.h"

Text::Text() {

	TextInit();
}

void Text::TextInit() {

	charaWantText[CharaWant::play] = "あそびたいみたいだよ";
	charaWantText[CharaWant::eat] = "おなかがすいたみたいだよ";
	charaWantText[CharaWant::toilet] = "トイレにいきたいみたいだよ";
	charaWantText[CharaWant::bath] = "おふろにはいりたいみたいだよ";
	charaWantText[CharaWant::touch] = "なでてほしいみたいだよ";
	charaWantText[CharaWant::none] = "";//未設置時のデフォルト
}

const std::string& Text::GetCharacterWantString(CharaWant want) const {

	auto i = charaWantText.find(want);
	if (i != charaWantText.end()) {
		return i->second;
	}

	return charaWantText.at(CharaWant::none);
}

#if 0
//静的メンバ変数の初期化
int Text::ch_i = 0;

Text::Text() {

}

Text::~Text() {

}

bool Text::SystemInit(void) {

	//GameInit();

	return true;
}

void Text::GameInit(void) {

}

void Text::Update(void) {

	i = 0;                              //iを初期値に戻し
	ch_i++;                             //表示する配列を1つずらす

	//配列の終端-2以下で、今回コピーするiまでの配列要素のi番目が\0じゃなくカウンターが5の倍数なら
	if (i <= 128 - 2 && ch[ch_i][i] != '\0') {
		i += 2;                             //全角文字なので配列要素2つずつカウントアップ
		strncpy_s(disp, ch[ch_i], i);       //iバイトまでの配列要素をdisp配列にコピー
		disp[i] = '\0';                     //終端記号を代入
	}
}

void Text::Draw(void) {

	DrawFormatString(0, 480, 0x000000, "%s", disp);
}

bool Text::Release(void) {

	return true;
}
#endif