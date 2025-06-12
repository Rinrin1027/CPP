// PlayerBullet.hpp
#ifndef PlayerBullet_hpp
#define PlayerBullet_hpp

#include "BulletManager.hpp"
#include <Siv3D.hpp>

class EnemyBase;  // 前方宣言

/// プレイヤーの弾ベースクラス（貫通＆ホーミング対応）
class PlayerBulletBase : public BulletBase
{
private:
    int        mRemainingHits = 1;   // 残ヒット回数
    bool       mHoming        = false;
    double     mSpeed         = 0.0; // 常に維持する速度
    EnemyBase* mTarget        = nullptr; // （将来用）ロック中の敵

public:
    PlayerBulletBase(const Vec2& pos, const Vec2& v);

    BulletType getType() const override;

    /// 衝突時に残ヒット数をデクリメントし、まだ残っていれば true
    bool handleHit();

    /// 残ヒット数を設定（貫通弾用）
    void setRemainingHits(int n);

    /// ホーミング有効化フラグ
    void setHoming(bool on) { mHoming = on; }

    /// 毎フレーム更新：ホーミング → 障害物回避 → 移動
    void update() override;
};

/// PlayerBullet1 クラス（ホーミング＆貫通対応）
class PlayerBullet1 : public PlayerBulletBase
{
public:
    PlayerBullet1(const Vec2& pos, const Vec2& v);

    RectF getCollisionRect() const override;
    void  draw() const override;
};

/// サポート機の弾ベース
class SupportUnitBulletBase : public BulletBase
{
public:
    SupportUnitBulletBase(const Vec2& pos, const Vec2& v);
    BulletType getType() const override;
};

/// サポート機の弾1
class SupportUnitBullet1 : public SupportUnitBulletBase
{
public:
    SupportUnitBullet1(const Vec2& pos, const Vec2& v);

    RectF getCollisionRect() const override;
    void  draw() const override;
};

#endif /* PlayerBullet_hpp */

