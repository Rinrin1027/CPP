//
//  ScrollingMap.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/20.
//

#include "ScrollingMap.hpp"
#include <cstdio>
#include <vector>


using namespace std;


#pragma mark - コンストラクタ・デストラクタ

// コンストラクタ
ScrollingMap::ScrollingMap(const String& mapFilename)
{
    loadMapData(mapFilename);

    mTex = Texture(U"assets/blocks.png");
    mScrollX = 0;
}


#pragma mark - インスタンス関数（パブリック）

// マップデータを1フレーム分更新する
void ScrollingMap::update()
{
    int numCols = static_cast<int>(mMapData.size());

    int totalMapWidth = numCols * Globals::kBlockSize;       // 全体のマップ幅
    int screenWidth = Scene::Width();               // 画面の幅
    int maxScroll = totalMapWidth - screenWidth;    // スクロール可能な最大値（全体幅から画面幅を引く）

    // スクロール位置が最大値未満なら更新
    if (mScrollX < maxScroll) {
        mScrollX += 42 * Scene::DeltaTime();
        if (mScrollX > maxScroll) {
            mScrollX = maxScroll;
        }
    }
}

// マップを描画する
void ScrollingMap::draw() const
{
    const int screenWidth = Scene::Width();

    // 現在のスクロール位置を整数化（描画のため）
    const int intScrollX = static_cast<int>(mScrollX);

    // CSVから読み込んだマップデータの行数と列数を取得
    const size_t numCols = mMapData.size();
    const size_t numRows = (numCols > 0)? mMapData[0].size(): 0;

    // 画面に描画されるチップの範囲を計算（左右+1ずつ多めに描画）
    const int firstChipX = intScrollX / Globals::kBlockSize;
    const int lastChipX = (intScrollX + screenWidth) / Globals::kBlockSize;

    // 描画されるX座標に対応するマップチップのインデックスをマップデータのサイズに合わせて制限
    const int minX = max(0, firstChipX);
    const int maxX = min(static_cast<int>(numCols) - 1, lastChipX);

    // 行数はマップデータから動的に取得
    for (int y = 0; y < static_cast<int>(numRows); y++) {
        for (int mapX = minX; mapX <= maxX; mapX++) {
            // マップ上の左上座標（画面座標に変換）
            double drawX = mapX * Globals::kBlockSize - mScrollX;
            double drawY = y * Globals::kBlockSize;

            int chipType = mMapData[mapX][y];
            if (chipType > 0) {
                drawMapChip(Vec2{ drawX, drawY }, chipType);
            }
        }
    }
    
    // マップ上にグリッド線を描画する
#if SHOW_MAP_GRID
{
    const int screenWidth = Scene::Width();
    const int screenHeight = Scene::Height();
    const int intScrollX = static_cast<int>(mScrollX);

    // 垂直方向のグリッド線
    int offsetX = intScrollX % kBlockSize;
    // 画面を覆うため、画面幅に対して十分な数の線を描画（余分に2本）
    int numVerticalLines = screenWidth / Globals::kBlockSize + 2;
    for (int i = 0; i < numVerticalLines; ++i) {
        int drawX = i * kBlockSize - offsetX;
        Line(Vec2(drawX, 0), Vec2(drawX, screenHeight)).draw(1, Palette::Gray);
    }

    // 水平方向のグリッド線
    int numHorizontalLines = screenHeight / Globals::kBlockSize + 2;
    for (int i = 0; i < numHorizontalLines; ++i) {
        int drawY = i * kBlockSize;
        Line(Vec2(0, drawY), Vec2(screenWidth, drawY)).draw(1, Palette::Gray);
    }
}
#endif  //#if SHOW_MAP_GRID
}

// X方向のスクロール位置を取得
double ScrollingMap::getScrollX() const
{
    return mScrollX;
}

// 指定された位置のマップチップの種類を取得
int ScrollingMap::getChipTypeAt(const Vec2& pos) const
{
    // ワールド座標をマップ座標に変換（スクロールを加味）
    int mapX = static_cast<int>((pos.x + mScrollX) / Globals::kBlockSize);
    int mapY = static_cast<int>(pos.y / Globals::kBlockSize);

    // 範囲外チェック（負の値と配列サイズ）
    if (mapX < 0 || mapY < 0 ||
        static_cast<size_t>(mapX) >= mMapData.size() ||
        static_cast<size_t>(mapY) >= mMapData[0].size())
    {
        return -1; // 範囲外のときは無効値（例: -1）を返す
    }

    return mMapData[mapX][mapY];
}

// 指定された位置が衝突判定のあるマップチップかどうかを判定
bool ScrollingMap::isBlocked(const Vec2& pos) const
{
    // ワールド座標をマップインデックスに変換
    int mapX = static_cast<int>((pos.x + mScrollX) / Globals::kBlockSize);
    int mapY = static_cast<int>(pos.y / Globals::kBlockSize);

    // 範囲外チェック
    if (mapX < 0 || static_cast<size_t>(mapX) >= mMapData.size() ||
        mapY < 0 || static_cast<size_t>(mapY) >= mMapData[0].size())
    {
        return false;
    }

    // チップタイプが1以上ならブロックとみなす
    return (mMapData[mapX][mapY] >= 1);
}

// 指定された矩形がマップチップと交差しているかどうかを判定
bool ScrollingMap::intersectsRect(const RectF& area) const
{
    // 左上と右下のインデックスを取得
    int startX = static_cast<int>((area.x + mScrollX) / Globals::kBlockSize);
    int endX = static_cast<int>((area.x + area.w + mScrollX) / Globals::kBlockSize);
    int startY = static_cast<int>(area.y / Globals::kBlockSize);
    int endY = static_cast<int>((area.y + area.h) / Globals::kBlockSize);

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            if (x < 0 || static_cast<size_t>(x) >= mMapData.size() ||
                y < 0 || static_cast<size_t>(y) >= mMapData[0].size())
            {
                continue;
            }

            if (mMapData[x][y] >= 1) {
                return true;
            }
        }
    }

    return false;
}


#pragma mark - インスタンス関数（プライベート）

// マップデータを読み込む
// @param mapFilename マップデータのファイル名
void ScrollingMap::loadMapData(const String& mapFilename)
{
    CSV csv{mapFilename};
    if (!csv) {
        throw Error{U"Failed to open CSV File: " + mapFilename};
    }
    
    // CSVの行数と、各行の列数の最大値を取得（mMapDataの外側のvectorのサイズとする）
    size_t numRows = csv.rows();
    size_t numCols = 0;
    for (size_t y = 0; y < numRows; y++) {
        numCols = std::max(numCols, csv.columns(y));
    }
    
    // '#'から始まるコメント行をカウントしない
    while (csv[numRows-1][0][0] == U'#') {
        numRows--;
    }

    // mMapDataをCSVに合わせたサイズに再構築（列×行：mMapData[x][y]形式）
    mMapData = vector<vector<int>>(numCols, vector<int>(numRows, -1));
    
    // CSVの各セルの値をマップデータに格納
    for (size_t y = 0; y < numRows; y++) {
        for (size_t x = 0; x < csv.columns(y); x++) {
            mMapData[x][y] = csv.get<int>(y, x);
        }
    }
}

// 指定された位置にマップチップを描画
void ScrollingMap::drawMapChip(const Vec2& topLeftPos, int chipType) const
{
    if (chipType >= 1 && chipType <= 30) {
        int chipX = (chipType - 1) % 10;
        int chipY = (chipType - 1) / 10;
        // マップチップの画像がある場合は画像の描画
        mTex(chipX * Globals::kChipSize,
             chipY * Globals::kChipSize,
             Globals::kChipSize, Globals::kChipSize).resized(Globals::kBlockSize, Globals::kBlockSize).draw(topLeftPos);
    } else {
        // それ以外のマップチップの描画
        RectF chipRect{ topLeftPos, Globals::kBlockSize, Globals::kBlockSize };
        chipRect.draw(ColorF{ 1.0, 0.0, 1.0 });
    }
}

