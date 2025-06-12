//  Enemy3.hpp
#pragma once
#include <Siv3D.hpp>
#include "EnemyBase.hpp"
#include "BulletManager.hpp"

/// ──────────────────────────────────────────────
///   ぐるぐる回転しながらプレイヤーに弾を撃つ敵（顔変化付き）
/// ──────────────────────────────────────────────
class Enemy3 : public EnemyBase
{
public:
    explicit Enemy3(const s3d::Vec2& pos);

    // EnemyBase から継承した純仮想関数
    s3d::RectF getAttackRect()  const override;
    s3d::RectF getDefenceRect() const override;
    void       update(BulletManager& bm) override;
    void       draw() const override;

private:
    /* === 振る舞い用パラメータ === */
    s3d::Vec2  mV         { -167.0, 0.0 };   // 水平速度
    s3d::Vec2  mA         {   0.0, 0.0 };    // 加速度（ばね用）
    const s3d::Vec2 mSize { 30.0, 30.0 };    // 描画サイズ
    double     mCenterY   = 0.0;             // 単振動の中心 Y
    double     mLastShot  = 0.0;             // 前回発射からの経過秒

    /* === 表情管理 === */
    bool       mIsAngry   = false;
    s3d::Texture mTexNormal;   // 💰
    s3d::Texture mTexAngry;    // 👿
    const double kAngryDistSq = 120.0 * 120.0;   // 怒り状態しきい値²

    /* 現在描画中のテクスチャ参照（↑どちらかを保持） */
    s3d::Texture mTex;
};

