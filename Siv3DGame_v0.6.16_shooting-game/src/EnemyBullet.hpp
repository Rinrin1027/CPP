//
//  EnemyBullet.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#ifndef EnemyBullet_hpp
#define EnemyBullet_hpp

#include "BulletManager.hpp"


/// 敵の弾を表すクラス
class EnemyBulletBase : public BulletBase
{
public:
    /// コンストラクタ
    /// @param pos 初期位置
    /// @param v 初期速度
    EnemyBulletBase(const Vec2& pos, const Vec2& v);

public:
    /// 弾の種類を取得
    BulletType getType() const override;
};


/// 敵の弾1を表すクラス
class EnemyBullet1 : public EnemyBulletBase
{
public:
    /// コンストラクタ
    /// @param pos 初期位置
    /// @param v 初期速度
    EnemyBullet1(const Vec2& pos, const Vec2& v);
    
public:
    /// 弾の当たり判定の矩形を取得
    RectF getCollisionRect() const override;

    /// 弾を描画
    void draw() const override;
};

#endif /* EnemyBullet_hpp */
