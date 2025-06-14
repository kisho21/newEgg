#pragma once
#include "StDefine.h"

#include <map>
#include <string>

enum class CharaWant {
	play,
	eat,
	toilet,
	bath,
	touch,
	none
};

class Text {
public:
	Text();

	const std::string& GetCharacterWantString(CharaWant want) const;

private:
	std::map<CharaWant, std::string> charaWantText;

	void TextInit();
};

#if 0
class Text {
public:
	Text();
	~Text();

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	void SetTextIndex(int textNum) { ch_i = textNum; }

private:
	int i = 0;					//表示中の文字列内の文字数をカウントする変数
	static int ch_i;		//表示中の文字列のインデックス管理
	
	//int prevReturnKey, nowReturnKey;

	char disp[128] = "";			//表示する文字列を一時的に格納する配列
	char ch[5][128] = {				//表示するテキスト
			"あそびたいみたいだよ",
			"おなかがすいたみたいだよ",
			"トイレにいきたいみたいだよ",
			"おふろにはいりたいみたいだよ",
			"なでてほしいみたいだよ"
	};
};
#endif