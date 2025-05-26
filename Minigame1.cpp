//
//ゲーム : egg
//氏名   : 永吉
//
#include <DxLib.h>

#include "Application.h"
#include "Minigame1.h"


Minigame1::Minigame1(void) {

    //仮置き画像、後々清書して1ファイルにまとめる
    gameImage = -1;
    squareImage = -1;

    InitializeGame();       //ゲームの初期化
    SetupTileColors();      //タイルの色設定
}

Minigame1::~Minigame1(void) {

}

bool Minigame1::SystemInit(void) {

    gameImage = LoadGraph("image/minigameFrame.png");
    if (gameImage == -1) return false;
    //squareImage = LoadGraph("image/44square.png");
    //if (squareImage == -1) return false;

    int nt = LoadDivGraph("image/numberTile.png", MAX_NUMBERTILE, MAX_NUMBERTILE, 1, 64, 64, numberTile);
    if (nt == -1) return false;

    mouseX = mouseY = 0;

    return true;
}

void Minigame1::GameInit(void) {

    //次のシーンの初期設定
    nextScene_ID = E_SCENE_MINIGAME_1;

    tilePosX = 13;
    tilePosY = 145;
    tileNo = 0;
    score = 0;

    prevMouseLeft = nowMouseLeft = 0;

    //
    prev0Key = now0Key = 0;
    prevUpKey = prevDownKey = prevRightKey = prevLeftKey = nowUpKey = nowDownKey = nowRightKey = nowLeftKey = 0;
}

void Minigame1::Update(void) {
    //キーの状態
    prevMouseLeft = nowMouseLeft;
    nowMouseLeft = (GetMouseInput() & MOUSE_INPUT_LEFT);

    GetMousePoint(&mouseX, &mouseY);		//マウスの座標を取得

    UpdateGame();

    //デバッグ
    /*
    prev0Key = now0Key;
    now0Key = CheckHitKey(KEY_INPUT_0);
    if (prev0Key == 0 && now0Key == 1) tileNo++;
    prevUpKey = nowUpKey;
    nowUpKey = CheckHitKey(KEY_INPUT_UP);
    prevDownKey = nowDownKey;
    nowDownKey = CheckHitKey(KEY_INPUT_DOWN);
    prevRightKey = nowRightKey;
    nowRightKey = CheckHitKey(KEY_INPUT_RIGHT);
    prevLeftKey = nowLeftKey;
    nowLeftKey = CheckHitKey(KEY_INPUT_LEFT);
    if (prevUpKey == 0 && nowUpKey == 1) tilePosY -= SLIDE_RANGE;
    if (prevDownKey == 0 && nowDownKey == 1) tilePosY += SLIDE_RANGE;
    if (prevRightKey == 0 && nowRightKey == 1) tilePosX += SLIDE_RANGE;
    if (prevLeftKey == 0 && nowLeftKey == 1) tilePosX -= SLIDE_RANGE;
    */
}

void Minigame1::Draw(void) {

    DrawGraph(0, 0, gameImage, true);
    DrawGraph(0, 0, squareImage, true);
    DrawGraph(tilePosX, tilePosY, numberTile[tileNo], true);

    DrawGame();

    //デバッグ
    DrawCircle(mouseX, mouseY, 5, GetColor(0, 0, 255));		//マウス座標の場所に円の描画
    DrawFormatString(0, 0, 0x000000, "%d", tileNo);
}

bool Minigame1::Release(void) {

    if (DeleteGraph(gameImage) == -1) return false;
    if (DeleteGraph(squareImage) == -1) return false;

    return true;
}

//タイルの色を設定
void Minigame1::SetupTileColors() {

    tileColors[0] = GetColor(200, 200, 200);        //空きマス
    tileColors[2] = GetColor(238, 228, 218);
    tileColors[4] = GetColor(237, 224, 200);
    tileColors[8] = GetColor(242, 177, 121);
    tileColors[16] = GetColor(245, 149, 99);
    tileColors[32] = GetColor(246, 124, 95);
    tileColors[64] = GetColor(246, 94, 59);
    tileColors[128] = GetColor(237, 207, 114);
    tileColors[256] = GetColor(237, 204, 97);
    tileColors[512] = GetColor(237, 200, 80);
    tileColors[1024] = GetColor(237, 197, 63);
    tileColors[2048] = GetColor(237, 194, 46);
}

//ゲームの初期化
void Minigame1::InitializeGame() {

    //ボードを0で初期化（すべてのマスを空きにする）
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = 0;
        }
    }
    score = 0;
    nowGameState = PLAYING;

    //はじめにタイルを2つ生成
    CreateNewTile();
    CreateNewTile();
}

//新しいタイルを生成
void Minigame1::CreateNewTile() {

    //staticをつけることで、呼び出された最初の一回は初期化し、それ以降は値を保持する
    static std::random_device seed_gen;
    static std::mt19937 engine(seed_gen());
    /*
    random_device : 非決定論的な乱数生成エンジン
                  : 擬似乱数生成エンジンのシード初期化や、暗号化といった用途に使用
    mt19937       : 32ビット版のメルセンヌ・ツイスターと呼ばれる、擬似乱数生成アルゴリズム
                  : 非常に長い周期と高い均等性を持つ
    */

    //可変長配列emptyCellsを宣言、空いているマスの座標（行と列）をstd::pairとして格納
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyCells.push_back({ i, j });         //もしマスが空いていれば行番号iと列番号jをstd::pairとしてemptyCellsベクトルに追加
            }
        }
    }

    //空きマスがない
    if (emptyCells.empty()) {
        return;     //新タイルを生成できないため関数を終了
    }

    std::uniform_int_distribution<> dist(0, emptyCells.size() - 1);
    int randomIndex = dist(engine);             //staticなengineを使う
    int row = emptyCells[randomIndex].first;
    int col = emptyCells[randomIndex].second;
    /*
    uniform_int_distribution   : ()内の範囲で均一な確率で整数を生成する
                               : emptyCells内のどのインデックスも等しい確率で選ばれるようになる
    randomIndex = dist(engine) : distにstaticで初期化したengineを渡して乱数を生成

    randomIndexを使ってemptyCellsからランダムに選ばれたstd::pairを取り出し、その行と列をそれぞれrowとcolに格納
    新タイルを配置する場所を決める
    */

    //2か4をランダムに生成 (2が90%, 4が10%)
    std::uniform_int_distribution<> valDist(1, 10);         //1から10のあいだで整数をランダムに生成
    board[row][col] = (valDist(engine) <= 9) ? 2 : 4;       //生成した値が9以下なら2を、そうでないなら4を代入
    /*
    三項演算子
    →条件演算子 ? x : y ;
    条件演算子：第1オペランドは暗黙的にboolに変換される
    x：true(1)ならこの第2オペランドが評価される
    y：false(2)ならこの第3オペランドが評価される
    */
}

//タイルを移動・結合する関数
//dx, dy : 移動方向 (-1, 0, 1)、(例)左へ移動なら dx=-1, dy=0
//成功したらtrue、変化がなければfalseを返す
bool Minigame1::MoveTiles(int dx, int dy) {

    bool moved = false;                                 //1回の操作でタイルが移動、結合したかどうか
    bool merged[BOARD_SIZE][BOARD_SIZE] = { false };    //そのターンで結合済みかどうかを記録、1回の操作で同じ場余のタイルが複数回結合されるのを防ぐ、1度結合したらtrueに

    //移動方向に応じてループの開始位置、終了位置、増減を調整
    //上下に移動する場合
    int startRow = (dy == 1) ? BOARD_SIZE - 1 : 0;      //trueなら最下行、falseなら最上行から
    int endRow = (dy == 1) ? -1 : BOARD_SIZE;           //trueなら上、falseなら下に向かって
    int stepRow = (dy == 1) ? -1 : 1;                   //1行ずつループ
    //左右に移動する場合
    int startCol = (dx == 1) ? BOARD_SIZE - 1 : 0;      //trueなら1番右の列、falseなら1番左の列から
    int endCol = (dx == 1) ? -1 : BOARD_SIZE;           //trueなら左、falseなら右に向かって
    int stepCol = (dx == 1) ? -1 : 1;                   //1列ずつループ

    for (int i = startRow; i != endRow; i += stepRow) {
        for (int j = startCol; j != endCol; j += stepCol) {
            if (board[i][j] == 0) continue;             //空きマスはスキップ、移動や結合の対象にならないので次のマスに進む

            int currentRow = i;
            int currentCol = j;                         //空きマスでない場合現在のマスとして扱い保持する

            //現在のタイルが可能な限り移動方向に移動し続ける
            while (true) {
                int nextRow = currentRow + dy;
                int nextCol = currentCol + dx;          //移動先の座標

                //移動先がボードの範囲外、または移動先が空きマスでない
                if (nextRow < 0 || nextRow >= BOARD_SIZE || nextCol < 0 || nextCol >= BOARD_SIZE) {
                    break;      //現在のタイルの処理を終了し次のタイルへ
                }

                //移動先が空きマス
                if (board[nextRow][nextCol] == 0) {
                    board[nextRow][nextCol] = board[currentRow][currentCol];    //現在のタイルが持っている値を次のタイルにコピー
                    board[currentRow][currentCol] = 0;                          //タイルが移動したため元のタイルを空きマスに
                    currentRow = nextRow;
                    currentCol = nextCol;                                       //タイルがさらに移動できるか次のループで確認
                    moved = true;                                               //タイルに変化があったのでtrueに
                }
                //移動先が同じ数値のタイル、さらに、まだ結合していない
                else if (board[nextRow][nextCol] == board[currentRow][currentCol] && !merged[nextRow][nextCol]) {
                    board[nextRow][nextCol] *= 2;                               //結合するためタイルの値を倍に
                    score += board[nextRow][nextCol];                           //結合されて倍になった値をスコアを加算
                    board[currentRow][currentCol] = 0;                          //結合したため元のタイルを空きマスに
                    merged[nextRow][nextCol] = true;                            //結合済み、さらに連続して結合しないように
                    moved = true;

                    //2048に到達したかチェック、クリアなしでもいい、無限にできるように
                    if (board[nextRow][nextCol] == 2048) {
                        nowGameState = GAME_CLEAR;
                    }
                    break; //結合したらそれ以上移動しない
                }
                //移動先が空きマスでなく、値も異なるタイルのとき
                else {
                    break; //移動終了
                }
            }
        }
    }
    return moved;       //今回の操作で変化があったかどうかを返す
}

// ゲームの描画
void Minigame1::DrawGame() {

    // グリッドの描画
    DrawBox(GRID_START_X - TILE_GAP, GRID_START_Y - TILE_GAP, GRID_START_X + BOARD_SIZE * (TILE_SIZE + TILE_GAP), GRID_START_Y + BOARD_SIZE * (TILE_SIZE + TILE_GAP), GetColor(187, 173, 160), TRUE);

    //タイルの描画
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            DrawTile(j, i, board[i][j]);        //列、行の順で渡す
        }
    }

    //スコアの描画
    DrawString(GRID_START_X, GRID_START_Y + BOARD_SIZE * (TILE_SIZE + TILE_GAP) + TILE_GAP, "Score:", 0xffffff);
    DrawFormatString(GRID_START_X + 80, GRID_START_Y + BOARD_SIZE * (TILE_SIZE + TILE_GAP) + TILE_GAP, 0xffffff, "%lld", score);

    //ゲームオーバー/クリアメッセージ
    if (nowGameState == GAME_OVER) {
        SetFontSize(32);        //フォントサイズを変更
        DrawString(0, 0,"GAME OVER!", GetColor(255, 0, 0));
        SetFontSize(16);        //元に戻す
    }
    else if (nowGameState == GAME_CLEAR) {
        SetFontSize(32);
        DrawString(0, 0,"YOU WIN!", GetColor(0, 255, 0));
        SetFontSize(16);
    }
}

//個々のタイルを描画
void Minigame1::DrawTile(int col, int row, int value) {

    int x1 = GRID_START_X + col * (TILE_SIZE + TILE_GAP);
    int y1 = GRID_START_Y + row * (TILE_SIZE + TILE_GAP);
    int x2 = x1 + TILE_SIZE;
    int y2 = y1 + TILE_SIZE;

    //タイルの背景色
    int bgColor = tileColors[0];                //デフォルトは空きマスの色
    if (tileColors.count(value)) {              //タイルの値に対応する色があるかチェック
        bgColor = tileColors[value];            //あればそれを背景色にする
    }
    DrawBox(x1, y1, x2, y2, bgColor, true);

    //タイルの数値
    if (value != 0) {
        SetFontSize(32);
        //値の桁数を計算する
        int digitCount = 0;
        if (value > 0) {
            digitCount = static_cast<int>(log10(value)) + 1;
            /*
            10を底とする対数をとる
            log10(100)なら2となる
            +1して桁数を計算する
            */
        }
        else {
            digitCount = 1;//0の場合
        }
        //数値の桁数に応じてフォントサイズを調整
        //文字列の幅と高さを取得して中央に描画
        char text[10];
        sprintf_s(text, "%d", value);                               //valueを文字列に変換
        int textWidth = GetDrawStringWidth(text, strlen(text));     //指定した文字列の幅を取得
        int textHeight = GetFontSize();                             //指定した文字列の高さを取得

        DrawString(x1 + (TILE_SIZE - textWidth) / 2, y1 + (TILE_SIZE - textHeight) / 2, text, GetColor(0, 0, 0));
        SetFontSize(16);
    }
}

//ゲームの更新処理 (キー入力、ゲームロジックの実行)
void Minigame1::UpdateGame() {

    if (nowGameState != PLAYING) {
        //ゲームオーバー、クリアの場合はRキーでリスタートできるようにする
        if (CheckHitKey(KEY_INPUT_R)) {
            InitializeGame();
        }
        return;     //ゲームオーバー、クリア時にタイルが動かないようにする
    }

    bool moved = false;
    static int prevKey = 0;         //staticが付いているので初期化されず、前のフレームのキー状態を保持
    
    int nowKey = CheckHitKey(KEY_INPUT_UP) | CheckHitKey(KEY_INPUT_DOWN) | CheckHitKey(KEY_INPUT_LEFT) | CheckHitKey(KEY_INPUT_RIGHT) | CheckHitKey(KEY_INPUT_W) | CheckHitKey(KEY_INPUT_S) | CheckHitKey(KEY_INPUT_A) | CheckHitKey(KEY_INPUT_D);
    /*
    | : OR演算子
    複数のキーのいずれかが押されれば0以外の値を返す
    */

    //デバッグ用、マウス操作に変更する
    //キーが押された瞬間のみ処理
    if (nowKey != 0 && prevKey == 0) {
        if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {
            moved = MoveTiles(0, -1);       //タイルの移動、連結があればtrueを返す
        }
        else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {
            moved = MoveTiles(0, 1);
        }
        else if (CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_A)) {
            moved = MoveTiles(-1, 0);
        }
        else if (CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_D)) {
            moved = MoveTiles(1, 0);
        }

        if (moved) {
            CreateNewTile();    //移動したら新しいタイルを生成
            if (CheckGameOver()) {
                nowGameState = GAME_OVER;
            }
        }
    }
    prevKey = nowKey;           //現在のキー状態を保存
}

//ゲームオーバー判定
bool Minigame1::CheckGameOver() {

    //空きマスがあるか
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                return false;   //空きマスがあればゲームオーバーではない
            }
        }
    }

    //隣接する同じ数値のタイルがあるか
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int currentValue = board[i][j];

            //右列が範囲内かつ、右隣りの値が同じ→ゲームオーバーでない
            if (j + 1 < BOARD_SIZE && board[i][j + 1] == currentValue) return false;
            //下行が範囲内かつ、下の値が同じ→ゲームオーバーでない
            if (i + 1 < BOARD_SIZE && board[i + 1][j] == currentValue) return false;
        }
    }

    //空きマスも結合できるタイルもない→ゲームオーバー
    return true;
}