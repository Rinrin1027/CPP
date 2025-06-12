//
//  EnemyBase.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include "EnemyBase.hpp"
#include "EnemyGenerator.hpp"
#include "PlayScene.hpp"
#include "EffectEnemyExplosion.hpp"


// コンストラクタ
EnemyBase::EnemyBase(const Vec2& pos)
    : mPos(pos), mIsDead(false)
{
    mSE_EnemyDamage = Audio(U"assets/enemy-burn.mp3");
}

// デストラクタ
EnemyBase::~EnemyBase()
{
    if (mGenerator->shouldDropItem()) {
        PlayScene::instance->addPowerUpItem(mPos);
    }
}

// この敵を生成した生成器の設定
void EnemyBase::setGenerator(const shared_ptr<EnemyGenerator>& generator)
{
    mGenerator = generator;
}

// 敵が死んでいるかどうかのチェック
bool EnemyBase::isDead() const
{
    return mIsDead;
}

// プレイヤーと衝突した時に呼ばれるコールバック関数
void EnemyBase::onHitPlayer()
{
    mIsDead = true;

    PlayAudio(mSE_EnemyDamage);

    PlayScene::instance->addScore(10);
    
    if (mGenerator) {
        mGenerator->onEnemyDefeated();
    }

    EffectSystem& effectSystem = PlayScene::instance->getEffectSystem();
    effectSystem.add(make_unique<EffectEnemyExplosion>(mPos));
}

// プレイヤーの弾と衝突した時に呼ばれるコールバック関数
void EnemyBase::onHitBullet()
{
    mIsDead = true;

    PlayAudio(mSE_EnemyDamage);

    PlayScene::instance->addScore(10);

    if (mGenerator) {
        mGenerator->onEnemyDefeated();
    }

    EffectSystem& effectSystem = PlayScene::instance->getEffectSystem();
    effectSystem.add(make_unique<EffectEnemyExplosion>(mPos));
}

// 画面外に出たことをチェックする
void EnemyBase::checkOutOfBounds()
{
    if ((mPos.x < -Globals::kSafeMargin) || (mPos.x > Scene::Width() + Globals::kSafeMargin) ||
        (mPos.y < -Globals::kSafeMargin) || (mPos.y > Scene::Height() + Globals::kSafeMargin)) {
        mIsDead = true;
    }
}

