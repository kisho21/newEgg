//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "Fader.h"

//コンストラクタ
Fader::Fader(void) {

}

//デストラクタ
Fader::~Fader(void) {

}

//初期化処理(最初の1回のみ)
bool Fader::SystemInit(void) {

	GameInit();

	return true;
}

//ゲーム起動・再開時に必ず呼び出す処理
void Fader::GameInit(void) {

	stat = E_STAT_FADE_non;
	alpha = 0.0f;						//透過度(0~255)、小さいほど透過される
	endFlg = true;
}

//更新処理
void Fader::Update(void) {

	if (endFlg == true) return;			//未実行なら何もしない

	switch (stat) {
	case E_STAT_FADE_non:
		return;
	case E_STAT_FADE_OUT:
		alpha += FADE_SPEED_ALPHA;		//徐々に暗転
		if (alpha > 255.0f) {
			//フェードアウト終了
			alpha = 255.0f;
			endFlg = true;
			stat = E_STAT_FADE_non;
		}
		break;
	case E_STAT_FADE_IN:
		alpha -= FADE_SPEED_ALPHA;		//徐々に明転
		if (alpha < 0.0f) {
			//フェードイン終了
			alpha = 0.0f;
			endFlg = true;
			stat = E_STAT_FADE_non;
		}
		break;
	default:
		return;
	}

}

//描画処理
void Fader::Draw(void) {

	switch (stat) {
	case E_STAT_FADE_non:
		return;
	case E_STAT_FADE_OUT:
	case E_STAT_FADE_IN:
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
		DrawBox(0, 0, Application::WINDOW_SIZE_WID, Application::WINDOW_SIZE_HIG, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	default:
		return;
	}
}

//解放処理(最後の１回のみ)
bool Fader::Release(void) {

	return true;
}

//フェードアウト・フェードイン開始時に呼び出す処理
void Fader::SetFade(eFadeStateID id) {

	stat = id;
	if (stat != E_STAT_FADE_non) {
		endFlg = false;
	}
}