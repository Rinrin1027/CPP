//──────────────────────────────── Enemy4.cpp
#include "Enemy4.hpp"
#include "PlayScene.hpp"

using namespace s3d;

/*============================================================
    BarrierEnemy
============================================================*/
BarrierEnemy::BarrierEnemy(const Vec2& pos)
    : EnemyBase(pos)
{}

RectF BarrierEnemy::getAttackRect() const
{
    return RectF(mPos - Vec2(mCoreR, mCoreR),
                 Vec2(mCoreR * 2.0, mCoreR * 2.0));
}

RectF BarrierEnemy::getDefenceRect() const
{
    const double r = (mShieldHP > 0) ? mShieldR : mCoreR;
    return RectF(mPos - Vec2(r, r),
                 Vec2(r * 2.0, r * 2.0));
}

void BarrierEnemy::onHitBullet()
{
    if (mShieldHP > 0) { --mShieldHP; return; }
    if (--mCoreHP <= 0) mIsDead = true;
}

void BarrierEnemy::update(BulletManager&)
{
    mPos += mV * Scene::DeltaTime();
    checkOutOfBounds();          // EnemyBase のユーティリティ
}

void BarrierEnemy::draw() const
{
    if (mShieldHP > 0)           // バリアが残っている時だけ描画
    {
        mShieldTex
            .resized(static_cast<int>(mShieldR * 2.0))
            .drawAt(mPos + Vec2(-mShieldR, 0.0));    // 左側に固定表示
    }
    mCoreTex
        .resized(static_cast<int>(mCoreR * 2.0))
        .drawAt(mPos);
}

/*============================================================
    SpreaderEnemy
============================================================*/
SpreaderEnemy::SpreaderEnemy(const Vec2& pos)
    : EnemyBase(pos)
{}

RectF SpreaderEnemy::getAttackRect() const
{
    return RectF(mPos - Vec2(18.0, 18.0), Vec2(36.0, 36.0));
}
RectF SpreaderEnemy::getDefenceRect() const { return getAttackRect(); }

void SpreaderEnemy::onHitBullet()
{
    if (--mHP <= 0) mIsDead = true;
}

void SpreaderEnemy::update(BulletManager& bm)
{
    const double dt = Scene::DeltaTime();

    /* ─ スタート位置へ移動 ─ */
    if (!mSettled)
    {
        mPos += mV * dt;
        if (mPos.x <= mTargetX)
        {
            mPos.x = mTargetX;
            mV     = Vec2(0.0, 0.0);
            mSettled = true;
        }
    }
    /* ─ 定点ばら撒き ─ */
    else
    {
        mTimer += dt;
        if (mTimer >= mInterval)
        {
            mTimer = 0.0;

            constexpr int N = 12;
            for (int i = 0; i < N; ++i)
            {
                const double ang = (360.0 / N) * i * 1_deg;
                Vec2 dir = Vec2(1.0, 0.0).rotated(ang);
                bm.add(std::make_unique<EnemyBullet1>(mPos, dir * 220.0));
            }
        }
    }

    /* 画面外チェック */
    if (mPos.y > Scene::Height() + 40.0) mIsDead = true;
}

void SpreaderEnemy::draw() const
{
    mTex.resized(36).drawAt(mPos);
}

