//
//  EnemyBullet.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#include "EnemyBullet.hpp"


#pragma mark - 敵の弾の基底クラス

// コンストラクタ
EnemyBulletBase::EnemyBulletBase(const Vec2& pos, const Vec2& v)
    : BulletBase(pos, v)
{
    // Do nothing
}

// 弾の種類を取得
BulletType EnemyBulletBase::getType() const
{
    return BulletType::Enemy;
}


#pragma mark - 敵の弾1

// コンストラクタ
EnemyBullet1::EnemyBullet1(const Vec2& pos, const Vec2& v)
    : EnemyBulletBase(pos, v)
{
    // Do nothing
}

// 弾の当たり判定の矩形を取得
RectF EnemyBullet1::getCollisionRect() const
{
    Vec2 size{16, 16};
    return RectF{mPos - size / 2, size};
}

// 弾を描画（現在位置を中心とした描画）
void EnemyBullet1::draw() const
{
    Circle(mPos, 8).draw(ColorF{1.0, 1.0, 1.0});
    
#if SHOW_BULLET_RECT
    getCollisionRect().drawFrame(1.0, Palette::Orange);
#endif  //#if SHOW_BULLET_RECT
}

