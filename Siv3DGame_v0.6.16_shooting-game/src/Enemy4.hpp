//──────────────────────────────── Enemy4.hpp
#pragma once
#include <Siv3D.hpp>
#include "EnemyBase.hpp"
#include "EnemyBullet.hpp"
#include "BulletManager.hpp"

using namespace s3d;

/*------------------------------------------------------------
    BarrierEnemy  ─ バリア → 本体の 2 段階 HP
------------------------------------------------------------*/
class BarrierEnemy final : public EnemyBase
{
public:
    explicit BarrierEnemy(const Vec2& pos);

    RectF getAttackRect()  const override;
    RectF getDefenceRect() const override;
    void  onHitBullet()           override;
    void  update(BulletManager&)  override;
    void  draw()            const override;

private:
    /* 表示 */
    const Texture mCoreTex   = Texture{ Emoji(U"👾")  };   // コア
    const Texture mShieldTex = Texture{ Emoji(U"🛡️") };   // シールド

    /* 形状 */
    const double mCoreR   = 18.0;
    const double mShieldR = 26.0;

    /* HP */
    int mShieldHP = 2;       // バリア
    int mCoreHP   = 2;       // 本体

    /* 速度 */
    const Vec2 mV = Vec2(-120.0, 0.0);   // 左へ流れる
};

/*------------------------------------------------------------
    SpreaderEnemy ─ 定点で 360°ばら撒き弾
------------------------------------------------------------*/
class SpreaderEnemy final : public EnemyBase
{
public:
    explicit SpreaderEnemy(const Vec2& pos);

    RectF getAttackRect()  const override;
    RectF getDefenceRect() const override;
    void  onHitBullet()           override;
    void  update(BulletManager&)  override;
    void  draw()            const override;

private:
    const Texture mTex = Texture{ Emoji(U"💣") };

    /* ばら撒き制御 */
    double mInterval = 2.0;      // 発射間隔［秒］
    double mTimer    = 0.0;

    /* 定位置へ滑り込む */
    bool   mSettled  = false;
    double mTargetX  = 560.0;
    Vec2   mV        = Vec2(-180.0, 0.0);

    /* HP */
    int mHP = 3;
};

