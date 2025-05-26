//
//ゲーム : egg
//氏名   : 永吉
//

#include <DxLib.h>

#include "Application.h"

//WinMain関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstanced, LPSTR lpCmdLine, int nCmdShow) {

	//Applicationクラスの実体化したインスタンス(オブジェクト)を作成
	Application apl;

	//初期化処理
	if (apl.SystemInit() == false) return -1;

	//ゲームの起動
	apl.Run();

	//ゲームの解放
	if (apl.Release() == false) return -1;

	return 0;
}