//
//  EffectEnemyExplosion.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include "EffectEnemyExplosion.hpp"


/// 敵の爆発エフェクトの持続時間
static const double kDuration = 0.6;


// コンストラクタ
EffectEnemyExplosion::EffectEnemyExplosion(const Vec2& pos)
    : mPos(pos)
{
    // Do nothing
}

// エフェクトの更新
void EffectEnemyExplosion::update()
{
    mTime += Scene::DeltaTime();
}

// エフェクトの描画
void EffectEnemyExplosion::draw() const
{
    double t = mTime / kDuration;

    // [0.0-0.5]:拡大, [0.5-1.0]:縮小
    double scale = (t < 0.5)? (t * 2.0): (1.0 - t) * 2.0;
    double radius = 50.0 * scale;

    Circle{ mPos, radius }.draw(ColorF{ 1.0, 0.8, 0.2, 0.7 });
}

// エフェクトが終了したかどうかを判定
bool EffectEnemyExplosion::isFinished() const
{
    return mTime >= kDuration;
}

