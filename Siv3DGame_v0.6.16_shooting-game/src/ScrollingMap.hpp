//
//  ScrollingMap.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/20.
//

#ifndef ScrollingMap_hpp
#define ScrollingMap_hpp

#include <Siv3D.hpp>
#include <vector>
#include "Globals.hpp"


using namespace std;


/// スクロールするマップを表すクラス
class ScrollingMap
{
private:
    double  mScrollX;               //!< X方向のスクロール分量
    vector<vector<int>> mMapData;   //!< マップデータ
    Texture  mTex;                  //!< マップチップのテクスチャ
    
public:
    /// コンストラクタ
    /// @param mapFilename マップデータのファイル名
    ScrollingMap(const String& mapFilename);

    /// デストラクタ
    ~ScrollingMap() = default;
    
    /// 1フレーム分のスクロールを行う
    void update();

    /// 現在のスクロール位置に応じたマップを描画する
    void draw() const;

    /// スクロール位置を取得
    double getScrollX() const;

    /// 指定された位置のマップチップの種類を取得
    /// @param pos 位置
    /// @return マップチップの種類
    int getChipTypeAt(const Vec2& pos) const;

    /// 指定された位置が衝突判定のあるマップチップかどうかを判定
    /// @param pos 位置
    /// @return 衝突判定のあるマップチップならtrue
    bool isBlocked(const Vec2& pos) const;

    /// 指定された矩形がマップチップと交差しているかどうかを判定
    /// @param area 矩形
    /// @return マップチップと交差しているならtrue
    bool intersectsRect(const RectF& area) const;

private:
    /// マップデータを読み込む
    /// @param mapFilename マップデータのファイル名
    void loadMapData(const String& mapFilename);

    /// 指定された位置のマップチップを描画
    /// @param topLeftPos マップチップの左上位置
    /// @param chipType マップチップの種類
    void drawMapChip(const Vec2& topLeftPos, int chipType) const;
};

#endif /* ScrollingMap_hpp */

