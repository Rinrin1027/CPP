//  Enemy3.cpp
#include "Enemy3.hpp"
#include "PlayScene.hpp"
#include "EnemyBullet.hpp"

using namespace s3d;

/* --------------------------------------------------------
   ctor
   --------------------------------------------------------*/
Enemy3::Enemy3(const Vec2& pos)
    : EnemyBase(pos)
    , mTexNormal{ Texture{ Emoji(U"💰") } }
    , mTexAngry { Texture{ Emoji(U"👿") } }
    , mTex      { mTexNormal }        // 最初は 💰
{
    mCenterY = mPos.y + 38.0;          // 単振動の中心を少し下に
}

/* --------------------------------------------------------
   判定矩形
   --------------------------------------------------------*/
RectF Enemy3::getAttackRect() const
{
    constexpr double pad = 5.0;
    return RectF{ mPos - (mSize / 2 + Vec2{ pad, pad }),
                  mSize.x + 2 * pad, mSize.y + 2 * pad };
}
RectF Enemy3::getDefenceRect() const
{
    return RectF{ mPos - mSize / 2, mSize };
}

/* --------------------------------------------------------
   update
   --------------------------------------------------------*/
void Enemy3::update(BulletManager& bm)
{
    const double dt = Scene::DeltaTime();

    /* ─── 1. 単振動でフワフワ ─── */
    constexpr double K = 13.5;            // バネ定数
    mA.y = (mCenterY - mPos.y) * K;
    mV  += mA * dt;
    mPos += mV * dt;

    /* ─── 2. 距離で表情切替 ─── */
    const Vec2 playerPos = PlayScene::instance->getPlayer().getPos();
    const double distSq  = (playerPos - mPos).lengthSq();
    const bool   angry   = (distSq < kAngryDistSq);

    if (angry != mIsAngry)
    {
        mIsAngry = angry;
        mTex     = mIsAngry ? mTexAngry : mTexNormal;
    }

    /* ─── 3. 射撃 ─── */
    mLastShot += dt;
    if (mLastShot >= 3.5)
    {
        Vec2 toPlayer = playerPos - mPos;
        if (toPlayer.lengthSq() >= Globals::kMinShotDistSq)
        {
            bm.add(std::make_unique<EnemyBullet1>(
                mPos, toPlayer.normalized() * 120));
        }
        mLastShot = 0.0;
    }

    /* ─── 4. 画面外チェック ─── */
    checkOutOfBounds();
}

/* --------------------------------------------------------
   draw
   --------------------------------------------------------*/
void Enemy3::draw() const
{
    mTex.resized(mSize).drawAt(mPos);

#if SHOW_DEFENCE_RECT
    getDefenceRect().drawFrame(1.0, Palette::Red);
#endif
#if SHOW_ATTACK_RECT
    getAttackRect().drawFrame(1.0, Palette::Yellow);
#endif
}

