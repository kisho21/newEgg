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

	charaWantText[CharaWant::play] = "あそびたいみたいだよ！";
	charaWantText[CharaWant::eat] = "おなかがすいたみたいだよ！";
	charaWantText[CharaWant::toilet] = "トイレにいきたいみたいだよ！";
	charaWantText[CharaWant::bath] = "おふろにはいりたいみたいだよ！";
	charaWantText[CharaWant::touch] = "なでてほしいみたいだよ！";
	charaWantText[CharaWant::none] = "";//未設置時のデフォルト
}

const std::string& Text::GetCharacterWantString(CharaWant want) const {

	auto i = charaWantText.find(want);
	if (i != charaWantText.end()) {
		return i->second;
	}

	return charaWantText.at(CharaWant::none);
}