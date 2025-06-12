//
//  EnemyBase.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#ifndef EnemyBase_hpp
#define EnemyBase_hpp

#include <Siv3D.hpp>
#include <memory>
#include "BulletManager.hpp"


class EnemyGenerator;

using namespace std;


// 敵の基本クラス。各種敵はこのクラスを継承して実装する
class EnemyBase
{
protected:
    Vec2    mPos;       //!< 現在位置（描画の中心位置）
    bool    mIsDead;    //!< 死んでいるかどうか
    shared_ptr<EnemyGenerator>  mGenerator; //<! この敵を生成したEnemyGeneratorのポインタ（アイテム化するかどうかの判定に使用）
    Audio   mSE_EnemyDamage;    //!< SE（敵がダメージを受けた）

public:
    /// コンストラクタ
    /// @param pos 敵の初期位置
    EnemyBase(const Vec2& pos);

    /// デストラクタ
    virtual ~EnemyBase();

    /// この敵を生成したEnemyGeneratorを設定する
    /// @param generator 敵を生成したEnemyGeneratorのポインタ
    void setGenerator(const shared_ptr<EnemyGenerator>& generator);
    
    /// この敵が死んでいるかどうかを判定
    bool isDead() const;

    /// 敵がプレイヤーを攻撃するための矩形を取得
    virtual RectF getAttackRect() const = 0;

    /// 敵がプレイヤーからの攻撃を受けるための矩形を取得
    virtual RectF getDefenceRect() const = 0;

    /// 敵の位置を更新し、必要に応じて弾を発射する
    /// @param bulletManager 弾情報の管理クラス
    virtual void update(BulletManager& bulletManager) = 0;

    /// 敵を描画する（現在位置を中心とした描画）
    virtual void draw() const = 0;
    
    /// 敵がプレイヤーからのダメージを受けた時に呼ばれるコールバック関数
    virtual void onHitPlayer();

    /// 敵が弾からのダメージを受けた時に呼ばれるコールバック関数
    virtual void onHitBullet();
    
protected:
    /// 敵が画面外に出たかどうかを判定
    void checkOutOfBounds();
};

#endif /* EnemyBase_hpp */
