// PlayerBullet.cpp

#include "PlayerBullet.hpp"
#include "PlayScene.hpp"
#include "EnemyManager.hpp"
#include <cfloat>   // DBL_MAX
#include <algorithm>

#pragma mark - PlayerBulletBase

// １秒あたりの最大回転角（ホーミング用）
constexpr double kMaxTurnSpeed   = 180.0_deg;
// 何倍先まで読んで回避を試みるか
constexpr double lookaheadFactor = 100.0;

PlayerBulletBase::PlayerBulletBase(const Vec2& pos, const Vec2& v)
    : BulletBase(pos, v)
    , mSpeed(v.length())
{
}

BulletType PlayerBulletBase::getType() const
{
    return BulletType::Player;
}

bool PlayerBulletBase::handleHit()
{
    --mRemainingHits;
    return (mRemainingHits > 0);
}

void PlayerBulletBase::setRemainingHits(int n)
{
    mRemainingHits = n;
}

void PlayerBulletBase::update()
{
    const double dt = Scene::DeltaTime();

    // ——— ホーミング処理 ———
    if (mHoming)
    {
        const auto& enemies = PlayScene::instance->getEnemyManager().getEnemies();
        double bestDistSq = DBL_MAX;
        Vec2   bestDir;

        for (const auto& e : enemies)
        {
            Vec2 enemyPos = e->getDefenceRect().center();
            Vec2 diff     = enemyPos - mPos;
            double d2     = diff.lengthSq();
            if (d2 < bestDistSq)
            {
                bestDistSq = d2;
                bestDir    = diff.normalized();
            }
        }

        if (bestDistSq < DBL_MAX)
        {
            Vec2 currDir   = mV.normalized();
            Vec2 targetDir = bestDir;
            double dot     = Clamp(currDir.dot(targetDir), -1.0, 1.0);
            double cross   = currDir.x * targetDir.y - currDir.y * targetDir.x;
            double angleDiff = std::acos(dot);
            if (cross < 0) angleDiff = -angleDiff;

            double maxTurn = kMaxTurnSpeed * dt;
            double turn    = Clamp(angleDiff, -maxTurn, +maxTurn);

            Vec2 newDir = currDir.rotated(turn);
            mV = newDir * mSpeed;
        }
    }

    // ——— 障害物回避処理（ホーミング弾のみ） ———
    if (mHoming)
    {
        double lookDt = dt * lookaheadFactor;
        Vec2  aheadPos  = mPos + mV * lookDt;
        RectF aheadRect = getCollisionRect().movedBy(aheadPos - mPos);

        const auto& map = PlayScene::instance->getMap();
        if (map.intersectsRect(aheadRect))
        {
            Vec2 currDir  = mV.normalized();
            Vec2 leftDir  = currDir.rotated(15_deg);
            Vec2 rightDir = currDir.rotated(-15_deg);

            Vec2 posL = mPos + leftDir  * mSpeed * lookDt;
            Vec2 posR = mPos + rightDir * mSpeed * lookDt;

            RectF rectL = getCollisionRect().movedBy(posL - mPos);
            RectF rectR = getCollisionRect().movedBy(posR - mPos);

            if (!map.intersectsRect(rectL))
            {
                mV = leftDir  * mSpeed;
            }
            else if (!map.intersectsRect(rectR))
            {
                mV = rightDir * mSpeed;
            }
            else
            {
                // 両方塞がれたら少し減速して粘る
                mV = currDir * (mSpeed * 1);
            }
        }
    }

    // ——— 最後に移動 ———
    BulletBase::update();
}

#pragma mark - PlayerBullet1

PlayerBullet1::PlayerBullet1(const Vec2& pos, const Vec2& v)
    : PlayerBulletBase(pos, v * 0.7)  // 移動速度を70%
{
}

RectF PlayerBullet1::getCollisionRect() const
{
    Vec2 size{16, 16};
    return RectF{ mPos - size / 2, size };
}

void PlayerBullet1::draw() const
{
    // 進行方向の角度
    Vec2 dir = mV.lengthSq() > 0 ? mV.normalized() : Vec2{1, 0};
    double angle = std::atan2(dir.y, dir.x);

    // オリジナルに近い５つの円のオフセット・半径・色
    static const Array<Vec2> offsets = {
        {   0,  0},  // 大円
        {   0,  0},  // 小円
        {  -4,  0},  // ハイライト1
        {-12,  0},   // ハイライト2
        {-16,  0}    // 奥行き
    };
    static const Array<double> radii = {
        8,  // 大円（元4→8）
        4,  // 小円（元2→4）
        4,  // ハイライト1（元2→4）
        4,  // ハイライト2（元2→4）
        3   // 奥行き（元1.5→3）
    };
    static const Array<ColorF> colors = {
        ColorF{0.4, 0.8, 1.0}, // 大円
        Palette::White,        // 小円
        Palette::White,        // ハイライト1
        ColorF{0.8, 0.8, 0.8}, // ハイライト2
        ColorF{0.5, 0.5, 0.5}  // 奥行き
    };

    // 回転マトリクスを使って、それぞれを描画
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        Vec2 pos = mPos + offsets[i].rotated(angle);
        Circle(pos, radii[i]).draw(colors[i]);
    }

#if SHOW_BULLET_RECT
    getCollisionRect().drawFrame(1.0, Palette::Orange);
#endif
}


#pragma mark - SupportUnitBulletBase

SupportUnitBulletBase::SupportUnitBulletBase(const Vec2& pos, const Vec2& v)
    : BulletBase(pos, v * 0.7)
{
}

BulletType SupportUnitBulletBase::getType() const
{
    return BulletType::SupportUnit;
}

#pragma mark - SupportUnitBullet1

SupportUnitBullet1::SupportUnitBullet1(const Vec2& pos, const Vec2& v)
    : SupportUnitBulletBase(pos, v * 0.7)
{
}

RectF SupportUnitBullet1::getCollisionRect() const
{
    Vec2 size{16, 16};
    return RectF{ mPos - size / 2, size };
}


void SupportUnitBullet1::draw() const
{
    // 進行方向の角度を取得
    Vec2 dir = mV.lengthSq() > 0 ? mV.normalized() : Vec2{1, 0};
    double angle = std::atan2(dir.y, dir.x);

    // 元の見た目に合わせた５つの円のオフセット・半径・色
    static const Array<Vec2> offsets = {
        {   0,  0},  // 大円
        {   0,  0},  // 小円
        {  -4,  0},  // ハイライト1
        {-12,  0},   // ハイライト2
        {-16,  0}    // 奥行き
    };
    static const Array<double> radii = {
        8,  // 大円（元4→8）
        4,  // 小円（元2→4）
        4,  // ハイライト1（元2→4）
        4,  // ハイライト2（元2→4）
        3   // 奥行き（元1.5→3）
    };
    static const Array<ColorF> colors = {
        ColorF{0.4, 0.8, 1.0}, // 大円
        Palette::Black,        // 小円
        Palette::White,        // ハイライト1
        ColorF{0.8, 0.8, 0.8}, // ハイライト2
        ColorF{0.5, 0.5, 0.5}  // 奥行き
    };

    // 回転マトリクスを適用して描画
    for (size_t i = 0; i < offsets.size(); ++i)
    {
        Vec2 pos = mPos + offsets[i].rotated(angle);
        Circle(pos, radii[i]).draw(colors[i]);
    }

#if SHOW_BULLET_RECT
    getCollisionRect().drawFrame(1.0, Palette::Orange);
#endif
}



