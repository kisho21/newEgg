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

	//シングルトンインスタンスを取得する静的メソッド
	static Character* getInst();

	~Character(void);

	bool SystemInit(void);
	void GameInit(void);
	void Update(void);
	void Draw(void);
	bool Release(void);

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
	/*
	カウントは常に加算し続ける
	一定に達したら何かしらの要求を提示
	要求提示中はカウントしない
	要求を解消したらカウントを0に初期化し再度加算を始める
	*/

	void RequestCount(void);
	void Request(void);
	void DrawRequest(void);

	void CharaPlay(void);
	void CharaEat(void);
	void CharaToilet(void);
	void CharaBath(void);
	void CharaTouth(void);

private:
	int growthRate;
	int gameMoney;
	int requestCount;
	bool requestFlg;

	enum CharaWantTo {
		normal,
		play,
		eat,
		toilet,
		bath,
		touch,

		max
	};
	CharaWantTo wantTo = normal;
	//CharaWantTo wantsNum;						//生成された乱数から要素を取得する変数

	bool playFlg;
	bool eatFlg;
	bool toiletFlg;
	bool bathFlg;
	bool touchFlg;

	int charaImage[MAX_CHARA];
	int charaX, charaY;
	int charaFrame;								//アニメーションの表示フレーム
	int charaFrameAdjust;						//アニメーション表示の調整フレーム

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
};