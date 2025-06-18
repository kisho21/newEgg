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