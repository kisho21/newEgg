#pragma once

class Character {
public:
	static constexpr int CHARA_WID = 128;
	static constexpr int CHARA_HIG = 128;

	//キャラクターのアニメーション
	static constexpr int ANIM_NUM_CHARA = 2;
	static constexpr int MAX_ANIM = ANIM_NUM_CHARA;

	//吹き出しのアニメーション
	static constexpr int BUBBLE_SIZE = 144;
	static constexpr int ANIM_NUM_BUBBLE = 2;

	//アニメーションインターバル
	//static constexpr int ANIM_INTERVAL = 10;

	//シングルトンインスタンスを取得する静的メソッド
	static Character* getInst();

	~Character(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

	void CharaPlay(void);		//各要求解消時の処理
	void CharaEat(void);		//↓
	void CharaToilet(void);		//↓
	void CharaBath(void);		//↓
	void CharaTouth(void);		//↓

	enum CharaWantTo {
		normal,
		play,
		eat,
		toilet,
		bath,
		touch,

		max
	};

	//ゲッターセッター関数
	//成長度関係
	int GetGrowth() const { return growthRate; }	//成長度を取得、const：値を変更しない
	void SetGrowth(int growth);						//成長度を設定
	void AddGrowth(int amount);						//成長度を増加させる
	//お金関係
	int GetMoney() const { return gameMoney; }		//所持金を取得
	void SetMoney(int money);						//所持金を設定
	void AddMoney(int amount);						//所持金を増加させる
	//要求カウント関係
	int GetRequest() const { return requestCount; }	//要求カウントを取得
	void SetRequest(int request);					//要求カウントを設定
	void AddRequest(int amount);					//要求カウントを増加させる
	void SetRequestFlgF(void) { requestFlg = false; }
	void RequestCount(void);

	int GetCharaX(void) { return charaX; }
	int GetCharaY(void) { return charaY; }

	bool GetPlayFlg(void) { return playFlg; }
	bool GetEatFlg(void) { return eatFlg; }
	bool GetToiletFlg(void) { return toiletFlg; }
	bool GetBathFlg(void) { return bathFlg; }
	bool GetTouchFlg(void) { return touchFlg; }

	int GetWantPosX(void) { return wantPosX; }
	int GetWantPosY(void) { return wantPosY; }
	CharaWantTo GetWnatTo(void) { return wantTo; }

private:
	int growthRate;
	int gameMoney;
	int requestCount;
	bool requestFlg;

	CharaWantTo wantTo;

	bool playFlg;
	bool eatFlg;
	bool toiletFlg;
	bool bathFlg;
	bool touchFlg;

	int playImage, eatImage, toiletImage, bathImage, touchImage;
	int wantPosX, wantPosY;

	int charaImage[MAX_ANIM];
	int charaX, charaY;
	int bubbleImage[MAX_ANIM];
	int frame;									//アニメーションの表示フレーム
	int frameAdjust;							//アニメーション表示の調整フレーム

	static Character* inst;						//シングルトンインスタンスへのポインタ
	Character(void);							//コンストラクタ

	//コピーコンストラクタの削除
	//コピーコンストラクタ：既存オブジェクトから新しいオブジェクトを生成する際に呼び出される
	//シングルトンではインスタンスは常に1つであるべきなので、オブジェクトのコピーを禁止する
	Character(const Character&) = delete;

	//代入演算子の削除
	//代入演算子：既存オブジェクトに別のオブジェクトの値を代入する際に呼び出される
	//既存のシングルトンインスタンスが別のインスタンスに上書きされるのを防ぐ
	Character& operator=(const Character&) = delete;

	void Animation(void);
	void Request(void);
	void DrawRequest(void);
};