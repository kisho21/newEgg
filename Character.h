#pragma once

class Character {
public:
	static constexpr int CHARA_WID = 128;
	static constexpr int CHARA_HIG = 128;

	//キャラクターのアニメーション
	static constexpr int ANIM_NUM_CHARA = 2;
	static constexpr int MAX_CHARA = ANIM_NUM_CHARA;

	//アニメーションインターバル
	//static constexpr int ANIM_INTERVAL = 10;

	Character(void);
	~Character(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

private:
	int charaImage[MAX_CHARA];

	int charaX, charaY;
	int charaFrame;								//アニメーションの表示フレーム
	int charaFrameAdjust;						//アニメーション表示の調整フレーム
};