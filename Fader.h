#pragma once

#include "StDefine.h"

class Fader {
public:
	static constexpr float FADE_SPEED_ALPHA = 2.0f;		//フェードの速さ

	Fader(void);
	~Fader(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	//ゲッター・セッター関数
	eFadeStateID GetNowState(void) { return stat; }
	bool IsEnd(void) { return endFlg; }
	void SetFade(eFadeStateID id);						//フェードアウト・フェードイン開始時に呼び出す関数

private:
	eFadeStateID stat;		//ステータス
	float alpha;			//アルファ値用の変数
	bool endFlg;			//フェード処理終了フラグ(true = 未実行 / false = 実行中)
};