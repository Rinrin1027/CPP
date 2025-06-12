//
//  Enemy1.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include "Enemy1.hpp"
#include "PlayScene.hpp"
#include "EffectSystem.hpp"
#include "EnemyBullet.hpp"


// コンストラクタ
Enemy1::Enemy1(const Vec2& pos)
    : EnemyBase(pos)
{
    mTex = Texture(Emoji(U"👻"));   // 絵文字からテクスチャを作る
    mV   = {-346.0, 0.0};
    mSize = {25, 25};
    mLastShotTime = 0.0;
    mState = 0;
}

// プレイヤー攻撃用の矩形を取得
RectF Enemy1::getAttackRect() const
{
    const double padding = 5.0; // 攻撃判定の拡大量
    return RectF(mPos - Vec2(mSize.x / 2 + padding, mSize.y / 2 + padding),
                 mSize.x + padding * 2,
                 mSize.y + padding * 2);
}

// プレイヤーからの攻撃を受けるための矩形を取得
RectF Enemy1::getDefenceRect() const
{
    return RectF(mPos - mSize / 2, mSize);
}

// 敵の状態を更新。必要に応じて弾を発射。
// @param bulletManager 弾情報の管理クラス
void Enemy1::update(BulletManager& bulletManager)
{
    if (mState == 0) {
        if (mPos.x < 160) {
            mState++;
            mV = {346.0, 173.0};
            if (mPos.y > Scene::Height() / 2) {
                mV.y *= -1;
            }
        }
    } else if (mState == 1) {
        if (mPos.x > 546) {
            mState++;
            mV = {-346.0, 0.0};
        }
    }
    mPos += mV * Scene::DeltaTime();
    mLastShotTime += Scene::DeltaTime();
    
    // 一定時間おきの弾の発射
    if (mLastShotTime >= 3.5) {
        Player& player = PlayScene::instance->getPlayer();
        Vec2 playerPos = player.getPos();
        Vec2 dir = playerPos - mPos;
        // 敵がプレイヤーに近すぎる場合は弾を発射しないようにする
        if (dir.lengthSq() >= Globals::kMinShotDistSq) {
            Vec2 speed = dir.normalized() * 120;
            bulletManager.add(make_unique<EnemyBullet1>(mPos, speed));
        }
        mLastShotTime = 0.0;
    }

    // 画面外に出たかどうかチェック
    checkOutOfBounds();
}

// 敵を描画（現在位置を中心とした描画）
void Enemy1::draw() const
{
    mTex.resized(mSize).drawAt(mPos);
    
#if SHOW_DEFENCE_RECT
    getDefenceRect().drawFrame(1.0, Palette::Red);
#endif  //#if SHOW_DEFENCE_RECT
    
#if SHOW_ATTACK_RECT
    getAttackRect().drawFrame(1.0, Palette::Yellow);
#endif  //#if SHOW_ATTACK_RECT
}

