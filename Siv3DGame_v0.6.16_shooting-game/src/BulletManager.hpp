//
//  BulletManager.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#ifndef BulletManager_hpp
#define BulletManager_hpp

#include <Siv3D.hpp>
#include <memory>
#include <vector>
#include "Globals.hpp"
#include "ScrollingMap.hpp"

class Player;
class EnemyManager;

using namespace std;


// cf. R-TYPEのレーザーやミサイル https://www.youtube.com/watch?v=WKErrM9LJXA


/// 弾の種類を表す列挙型
enum class BulletType
{
    Player,         //!< プレイヤーの弾
    SupportUnit,    //!< サポート機の弾
    Enemy,          //!< 敵の弾
};


/// 弾のベースクラス
class BulletBase
{
protected:
    Vec2    mPos;   //!< 現在位置（描画の中心位置）
    Vec2    mV;     //!< 弾の速度ベクトル
    
public:
    /// コンストラクタ
    /// @param pos 弾の初期位置
    /// @param v 弾の速度ベクトル
    BulletBase(const Vec2& pos, const Vec2& v);

    /// デストラクタ
    virtual ~BulletBase() = default;

    /// 弾が画面外に出たかどうかを判定
    virtual bool isOutOfScreen() const;

    /// プレイヤーの弾の種類を取得
    virtual BulletType getType() const = 0;

    /// 弾の衝突判定の矩形を取得
    virtual RectF getCollisionRect() const = 0;

    /// 弾の状態を更新
    virtual void update();

    /// 弾を描画
    virtual void draw() const = 0;
};

/// 弾の管理を行うクラス
class BulletManager
{
private:
    vector<unique_ptr<BulletBase>>  mBullets;   //!< 弾を格納するコンテナ
    int     mPlayerSideCount;       //!< プレイヤーの弾の数
    int     mSupportUnitSideCount;  //!< サポート機の弾の数
    int     mEnemySideCount;        //!< 敵の弾の数

public:
    /// コンストラクタ
    BulletManager();

    /// デストラクタ
    ~BulletManager() = default;
    
    /// 弾がマップ上の障害物に当たっているか判定する関数
    /// @param map マップのインスタンス参照
    void checkMapCollisions(const ScrollingMap& map);

    /// 敵の弾がプレイヤーに当たっているか判定する関数
    /// @param player プレイヤーのインスタンス参照
    void checkEnemyBulletCollisions(Player& player);

    /// プレイヤーかサポート機の弾が敵に当たっているか判定する関数
    /// @param enemyManager 敵の管理クラスのインスタンス参照
    void checkPlayerOrSupportUnitBulletCollisions(EnemyManager& enemyManager);

    /// プレイヤーの弾の数を取得
    int getPlayerSideCount() const;
    
    /// すべての弾の状態を更新
    void update();

    /// すべての弾を描画
    void draw() const;

    /// 新しい弾を追加
    /// @param bullet 追加する弾
    void add(unique_ptr<BulletBase> bullet);
};

#endif /* BulletManager_hpp */
