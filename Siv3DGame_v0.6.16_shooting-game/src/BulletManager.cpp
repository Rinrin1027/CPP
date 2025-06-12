//
//  BulletManager.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#include "BulletManager.hpp"
#include "PlayerBullet.hpp"
#include "Player.hpp"
#include "EnemyManager.hpp"

#pragma mark - 弾のベースクラス

BulletBase::BulletBase(const Vec2& pos, const Vec2& v)
    : mPos(pos), mV(v)
{
}

bool BulletBase::isOutOfScreen() const
{
    constexpr double kSafeMargin = 20.0;
    return (mPos.x < -kSafeMargin || mPos.x > Scene::Width() + kSafeMargin ||
            mPos.y < -kSafeMargin || mPos.y > Scene::Height() + kSafeMargin);
}

void BulletBase::update()
{
    mPos += mV * Scene::DeltaTime();
}

#pragma mark - 弾管理クラス

BulletManager::BulletManager()
    : mPlayerSideCount(0)
    , mSupportUnitSideCount(0)
    , mEnemySideCount(0)
{
}

void BulletManager::checkMapCollisions(const ScrollingMap& map)
{
    for (auto it = mBullets.begin(); it != mBullets.end();)
    {
        if (map.intersectsRect((*it)->getCollisionRect()))
        {
            switch ((*it)->getType())
            {
                case BulletType::Player:      --mPlayerSideCount;     break;
                case BulletType::SupportUnit: --mSupportUnitSideCount;break;
                case BulletType::Enemy:       --mEnemySideCount;      break;
            }
            it = mBullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void BulletManager::checkEnemyBulletCollisions(Player& player)
{
    for (auto it = mBullets.begin(); it != mBullets.end();)
    {
        if ((*it)->getType() == BulletType::Enemy
            && (*it)->getCollisionRect().intersects(player.getDefenceRect()))
        {
            player.onHitBullet();
            --mEnemySideCount;
            it = mBullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void BulletManager::checkPlayerOrSupportUnitBulletCollisions(EnemyManager& enemyManager)
{
    // オリジナルのまま：敵管理側に委譲
    int playerSideHitCount = enemyManager.checkPlayerBulletCollisions(mBullets);
    mPlayerSideCount -= playerSideHitCount;

    int supportUnitSideHitCount = enemyManager.checkSupportUnitBulletCollisions(mBullets);
    mSupportUnitSideCount -= supportUnitSideHitCount;
}

int BulletManager::getPlayerSideCount() const
{
    return mPlayerSideCount;
}

void BulletManager::update()
{
    for (auto it = mBullets.begin(); it != mBullets.end();)
    {
        (*it)->update();

        if ((*it)->isOutOfScreen())
        {
            switch ((*it)->getType())
            {
                case BulletType::Player:      --mPlayerSideCount;     break;
                case BulletType::SupportUnit: --mSupportUnitSideCount;break;
                case BulletType::Enemy:       --mEnemySideCount;      break;
            }
            it = mBullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void BulletManager::draw() const
{
    for (const auto& bullet : mBullets)
    {
        bullet->draw();
    }
}

void BulletManager::add(unique_ptr<BulletBase> bullet)
{
    switch (bullet->getType())
    {
        case BulletType::Player:      ++mPlayerSideCount;     break;
        case BulletType::SupportUnit: ++mSupportUnitSideCount;break;
        case BulletType::Enemy:       ++mEnemySideCount;      break;
        default:                      break;
    }
    mBullets.push_back(std::move(bullet));
}

