//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "Fader.h"
#include "Minigame1.h"

SceneManager::SceneManager(void) {

	/*
	gHuntInst = nullptr;
	gOverInst = nullptr;
	eventInst = nullptr;
	gBaseInst = nullptr;
	*/

	titleInst = nullptr;
	gameInst = nullptr;
	fader = nullptr;
	mini1Inst = nullptr;

	scene_ID = waitScene = E_SCENE_non;
}

SceneManager::~SceneManager(void) {

}

bool SceneManager::SystemInit(void) {

	fader = new Fader();
	if (fader == nullptr) return false;

	fader->SystemInit();

	sceneChangeFlg = false;

	ChangeScene(E_SCENE_TITLE);

	return true;
}

void SceneManager::GammeInit(void) {

}

void SceneManager::Update(void) {

	fader->Update();

	if (sceneChangeFlg) {
		if (fader->IsEnd() && waitScene != E_SCENE_non) {
			//フェードアウト終了
			ChangeScene(waitScene);
			waitScene = E_SCENE_non;
			fader->SetFade(E_STAT_FADE_IN);
		}
		else if (fader->IsEnd() && waitScene == E_SCENE_non) {
			//フェードイン終了
			sceneChangeFlg = false;
		}
	}
	else {
		eScene_ID nextScene_ID = scene_ID;

		switch (scene_ID) {
		case E_SCENE_TITLE:
			titleInst->Update();
			nextScene_ID = titleInst->GetNextSceneID();
			break;
		case E_SCENE_GAME:
			gameInst->Update();
			nextScene_ID = gameInst->GetNextSceneID();
			break;
		case E_SCENE_MINIGAME_1:
			mini1Inst->Update();
			nextScene_ID = mini1Inst->GetNextSceneID();
			break;
			/*
		case E_SCENE_EVENT:
			eventInst->Update();
			nextScene_ID = eventInst->GetNextSceneID();
			break;
		case E_SCENE_TUTORIAL:
			gGameInst->Update();
			nextScene_ID = gGameInst->GetNextSceneID();
			break;
		case E_SCENE_GAMEOVER:
			gHuntInst->Update();
			nextScene_ID = gHuntInst->GetNextSceneID();
			break;
			*/
		}

		//シーン遷移判定
		if (scene_ID != nextScene_ID) {
			sceneChangeFlg = true;				//シーン遷移処理開始
			waitScene = nextScene_ID;
			fader->SetFade(E_STAT_FADE_OUT);
		}
	}
}

void SceneManager::Draw(void) {

	switch (scene_ID) {
	case E_SCENE_TITLE:
		titleInst->Draw();
		break;
	case E_SCENE_GAME:
		gameInst->Draw();
		break;
	case E_SCENE_MINIGAME_1:
		mini1Inst->Draw();
		break;
		/*
	case E_SCENE_EVENT:
		eventInst->Draw();
		break;
	case E_SCENE_TUTORIAL:
		gGameInst->Draw();
		break;
	case E_SCENE_GAMEOVER:
		gHuntInst->Draw();
		break;
		*/
	}

	fader->Draw();
}

bool SceneManager::Release(void) {

	//インスタンスの解放
	ReleaseScene(E_SCENE_TITLE);
	ReleaseScene(E_SCENE_GAME);
	ReleaseScene(E_SCENE_MINIGAME_1);
	//ReleaseScene(E_SCENE_EVENT);
	//ReleaseScene(E_SCENE_TUTORIAL);
	//ReleaseScene(E_SCENE_GAMEOVER);

	if (fader->Release() == false) return false;
	delete fader;
	fader = nullptr;

	return true;
}

bool SceneManager::ChangeScene(eScene_ID id) {

	ReleaseScene(scene_ID);

	//シーンIDを変更
	scene_ID = id;

	switch (scene_ID) {
	case E_SCENE_TITLE:
		if (titleInst == nullptr) {
			titleInst = new SceneTitle();
			if (titleInst == nullptr) return false;
			titleInst->SystemInit();
			titleInst->GameInit();
		}
		break;
	case E_SCENE_GAME:
		if (gameInst == nullptr) {
			gameInst = new SceneGame();
			if (gameInst == nullptr) return false;
			gameInst->SystemInit();
			gameInst->GameInit();
		}
		break;
	case E_SCENE_MINIGAME_1:
		if (mini1Inst == nullptr) {
			mini1Inst = new Minigame1();
			if (mini1Inst == nullptr) return false;
			mini1Inst->SystemInit();
			mini1Inst->GameInit();
		}
		break;
		/*
	case E_SCENE_EVENT:
		if (eventInst == nullptr) {
			eventInst = new SceneEvent();
			if (eventInst == nullptr) return false;
			eventInst->SystemInit();
			eventInst->GameInit();
		}
		break;
	case E_SCENE_TUTORIAL:
		if (gGameInst == nullptr) {
			gGameInst = new SceneGame();
			if (gGameInst == nullptr) return false;
			gGameInst->SystemInit();
			gGameInst->GameInit();
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gHuntInst == nullptr) {
			gHuntInst = new GameHuntScene();
			if (gHuntInst == nullptr) return false;
			gHuntInst->SystemInit();
			gHuntInst->GameInit();
		}
		break;
		*/
	}

	return true;
}

void SceneManager::ReleaseScene(eScene_ID id) {

	switch (scene_ID) {
	case E_SCENE_TITLE:
		if (titleInst != nullptr) {
			titleInst->Release();
			delete titleInst;
			titleInst = nullptr;
		}
		break;
	case E_SCENE_GAME:
		if (gameInst != nullptr) {
			gameInst->Release();
			delete gameInst;
			gameInst = nullptr;
		}
		break;
	case E_SCENE_MINIGAME_1:
		if (mini1Inst != nullptr) {
			mini1Inst->Release();
			delete mini1Inst;
			mini1Inst = nullptr;
		}
		break;
		/*
	case E_SCENE_EVENT:
		if (eventInst != nullptr) {
			eventInst->Release();
			delete eventInst;
			eventInst = nullptr;
		}
		break;
	case E_SCENE_TUTORIAL:
		if (gGameInst != nullptr) {
			gGameInst->Release();
			delete gGameInst;
			gGameInst = nullptr;
		}
		break;
	case E_SCENE_GAMEOVER:
		if (gHuntInst != nullptr) {
			gHuntInst->Release();
			delete gHuntInst;
			gHuntInst = nullptr;
		}
		break;
		*/
	}
}