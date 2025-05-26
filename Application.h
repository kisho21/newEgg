#pragma once

class SceneManager;

class Application {

public:
	static const int WINDOW_SIZE_WID = 288;		//ウィンドウの幅
	static const int WINDOW_SIZE_HIG = 512;		//ウィンドウの高さ、16:9

	Application(void);		//コンストラクタ
	~Application(void);		//デストラクタ

	bool SystemInit(void);
	void Run(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

private:
	SceneManager* sceneM;	//SceneManagerクラスのインスタンスのポインタ

};