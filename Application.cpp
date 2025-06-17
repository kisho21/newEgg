//
//ゲーム : egg
//氏名   : 永吉
//
#include <time.h>
#include <DxLib.h>

#include "Application.h"
#include "SceneManager.h"

//コンストラクタ
Application::Application(void) {

	sceneM = nullptr;
}

//デストラクタ
Application::~Application(void) {

}

/*
初期化処理(最初の1回のみ)
input：無し
output：true = 正常終了 / false = エラー
*/
bool Application::SystemInit(void) {

	//システム設定処理
	SetWindowText("egg");							//ウィンドウのタイトル
	SetGraphMode(WINDOW_SIZE_WID, WINDOW_SIZE_HIG, 32);		//ウィンドウのサイズと色モード
	ChangeWindowMode(true);									//ウィンドウの表示方式 (ture = ウィンドウ表示 / false = フルスクリーン)

	SRand(time(NULL));

	if (DxLib_Init() == -1) return false;					//Dxライブラリの初期化

	sceneM = new SceneManager();
	if (sceneM == nullptr) return false;

	sceneM->SystemInit();

	//mouseX = 0;
	//mouseY = 0;

	return true;
}

/*
実行処理
input：無し
output：無し
*/
void Application::Run(void) {

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {

		Update();	//更新
		Draw();		//描画
	}
}

/*
更新処理
input：無し
output：無し
*/
void Application::Update(void) {

	sceneM->Update();
}

/*
描画処理
input：無し
output：無し
*/
void Application::Draw(void) {

	SetDrawScreen(DX_SCREEN_BACK);		// 描画する画面を裏画面に設定する
	ClearDrawScreen();					// 描画する画面をクリアする

	sceneM->Draw();

	ScreenFlip();						// 表示対象画面を入れ替える
}

/*
解放処理
input：無し
output：true = 正常終了 / false = エラー
*/
bool Application::Release(void) {

	sceneM->Release();
	delete sceneM;
	sceneM = nullptr;

	if (DxLib_End() == -1) return false;

	return true;
}