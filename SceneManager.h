#pragma once

#include "StDefine.h"

class SceneTitle;
class SceneName;
class SceneGame;
class Fader;
class Minigame1;
class Minigame2;

class SceneManager {
public:
	SceneManager(void);
	~SceneManager(void);

	bool SystemInit(void);
	void GammeInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

private:
	//インスタンスのポインタを作成
	SceneTitle* titleInst;
	SceneName* nameInst;
	SceneGame* gameInst;
	Fader* fader;
	Minigame1* mini1Inst;
	Minigame2* mini2Inst;


	eScene_ID scene_ID;		//現在のシーンID
	eScene_ID waitScene;	//シーンチェンジで次に遷移するシーンのIDを保管
	bool sceneChangeFlg;	//シーンチェンジ実行中フラグ

	//シーン切り替え処理
	bool ChangeScene(eScene_ID id);
	void ReleaseScene(eScene_ID id);
};