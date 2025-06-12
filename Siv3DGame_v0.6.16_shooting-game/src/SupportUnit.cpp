//
//  SupportUnit.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#include "SupportUnit.hpp"
#include "PlayerBullet.hpp"


/// コンストラクタ
SupportUnit::SupportUnit(int delayFrames)
    : mPos(100, 300), mFollowDelay(delayFrames)
{
    mTex = Texture(Emoji(U"🖲️"));   // 絵文字からテクスチャを作る
}

// 現在位置の取得
Vec2 SupportUnit::getPos() const
{
    return mPos;
}

// サポート機の更新
void SupportUnit::update(const deque<Vec2>& trail)
{
    if (trail.size() > static_cast<size_t>(mFollowDelay)) {
        mPos = trail[trail.size() - mFollowDelay - 1];
    }
}

// 弾の発射
void SupportUnit::fireBullets(BulletManager& bulletManager) const
{
    bulletManager.add(make_unique<SupportUnitBullet1>(mPos + Vec2{20, 0}, Vec2{800, 0}));
}

// サポート機の描画
void SupportUnit::draw() const
{
    mTex.resized(25, 25).drawAt(mPos);
}

