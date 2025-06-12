//
//  Player.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/20.
//

#include "Player.hpp"
#include "PlayScene.hpp"
#include "EffectSystem.hpp"
#include "EffectPlayerExplosion.hpp"
#include <algorithm>


#pragma mark - コンストラクタ

Player::Player()
    : mPos{100, 293}
    , mIsDead(false)
    , mIsInvincible(false)
    , mVerticalDir(0)
    , mSprintGauge(kMaxSprintGauge)  // フル充填スタート
{
    mTex        = Texture(U"assets/player_sprite.png");
    mSE_Damaged = Audio(U"assets/shot4.mp3");
}


#pragma mark - 情報取得

Vec2 Player::getPos() const
{
    return mPos;
}

RectF Player::getDefenceRect() const
{
    return RectF(mPos.x - 4, mPos.y - 8, {20, 20});
}

RectF Player::getPowerUpRect() const
{
    Vec2 size{50, 25};
    size += Vec2{4, 4};
    return RectF(mPos - size / 2, size);
}

bool Player::isDead() const
{
    return mIsDead;
}

bool Player::isInvincible() const
{
    return mIsInvincible;
}

const deque<Vec2>& Player::getTrail() const
{
    return mTrail;
}


#pragma mark - 移動・スプリント

void Player::move(const Vec2& input, double baseSpeed)
{
    if (mIsDead) {
        return;
    }

    // 縦移動方向だけ記憶
    if      (input.y <  0.0) mVerticalDir = -1;
    else if (input.y >  0.0) mVerticalDir = +1;
    else                     mVerticalDir =  0;

    // ΔTime
    double dt = Scene::DeltaTime();

    // スプリント判定・ゲージ処理
    bool sprintKey = KeyShift.pressed();
    bool sprinting = false;
    if (sprintKey && mSprintGauge > 0.0) {
        // ゲージがあるときだけ 2 倍速＆消費
        mSprintGauge = Max(0.0, mSprintGauge - kSprintConsumptionRate * dt);
        sprinting = true;
    }
    else if (!sprintKey) {
        // Shift を押していないときだけ回復
        mSprintGauge = Min(kMaxSprintGauge, mSprintGauge + kSprintRegenRate * dt);
    }

    // 速度倍率
    double speedMul = sprinting ? 2.0 : 1.0;

    // 正規化＋移動
    Vec2 dir = input.lengthSq() > 0.0 ? input.normalized() : Vec2{0,0};
    mPos += dir * baseSpeed * speedMul;

    // 横はクランプ
    mPos.x = clamp(mPos.x, 10.0, Scene::Width() - 10.0);

    // 縦はラップ
    constexpr double halfH = 12.5;
    const double topLimit    = -halfH;
    const double bottomLimit = Scene::Height() + halfH;

    if (mPos.y < topLimit) {
        mPos.y = bottomLimit + (mPos.y - topLimit);
    }
    else if (mPos.y > bottomLimit) {
        mPos.y = topLimit + (mPos.y - bottomLimit);
    }
}


#pragma mark - 軌跡更新

void Player::updateTrail()
{
    mTrail.push_back(mPos);
    if (mTrail.size() > Globals::kPlayerTrailBufferSize) {
        mTrail.pop_front();
    }
}


#pragma mark - 描画

void Player::draw() const
{
    if (mIsDead) return;

    const double interval   = 0.1;
    const int    komaCount  = 2;
    const int    komaWidth  = 95;
    const int    komaHeight = 74;
    int frame = (static_cast<int>(Scene::Time() / interval) % komaCount);
    int srcX  = frame * komaWidth;

    auto region = mTex(srcX, 0, komaWidth, komaHeight).resized(50, 25);
    
    if (mVerticalDir < 0)
    {
        int vPhase = (static_cast<int>(Scene::Time() / (interval * 2)) % 2);
        double angle = (vPhase == 0 ? 15.0_deg : 30.0_deg);
        region.rotated(-angle).drawAt(mPos);
    }
    else if (mVerticalDir > 0)
    {
        int vPhase = (static_cast<int>(Scene::Time() / (interval * 2)) % 2);
        double angle = (vPhase == 0 ? 15.0_deg : 30.0_deg);
        region.rotated(angle).drawAt(mPos);
    }
    else
    {
        region.drawAt(mPos);
    }

#if SHOW_DEFENCE_RECT
    getDefenceRect().drawFrame(1.0, Palette::Red);
#endif
}


#pragma mark - 衝突コールバック

void Player::onHitMap()
{
    if (mIsInvincible) return;
    mIsDead = true;
    PlayScene::instance->getEffectSystem()
        .add(make_unique<EffectPlayerExplosion>(mPos));
}

void Player::onHitEnemy()
{
    onHitMap();
}

void Player::onHitBullet()
{
    onHitMap();
}


#pragma mark - 無敵切り替え

void Player::toggleInvincible()
{
    mIsInvincible = !mIsInvincible;
}

