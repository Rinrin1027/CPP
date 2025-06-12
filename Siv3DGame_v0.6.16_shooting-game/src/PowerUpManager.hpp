//
//  PowerUpManager.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#ifndef PowerUpManager_hpp
#define PowerUpManager_hpp

#include <Siv3D.hpp>
#include <memory>
#include <vector>
#include "PowerUp.hpp"


using namespace std;


/// パワーアップアイテムを管理するクラス
class PowerUpManager
{
private:
    vector<unique_ptr<PowerUp>> mPowerUps;  //!< パワーアップ・アイテムのリスト
    int mCollectedCount;    //!< 収集したパワーアップ・アイテムの数

public:
    /// コンストラクタ
    PowerUpManager();
    
    /// 指定された位置にパワーアップ・アイテムを追加
    /// @param pos 追加する位置
    void add(const Vec2& pos);

    /// これまでにパワーアップ・アイテムを収集した数を取得
    int getCollectedCount() const;

    /// パワーアップ・アイテムとプレイヤーが交差したかどうかを判定
    /// @param playerRect プレイヤーの矩形
    /// @return 交差したならtrue
    bool checkPlayerCollision(const RectF& playerRect);

    /// すべてのパワーアップ・アイテムの位置を更新
    void update();

    /// すべてのパワーアップ・アイテムを描画
    void draw() const;
};


#endif /* PowerUpManager_hpp */
