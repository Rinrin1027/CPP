// EnemyManager.cpp
#include "EnemyManager.hpp"
#include "PlayerBullet.hpp"

#pragma mark - コンストラクタ

EnemyManager::EnemyManager()
{
}

#pragma mark - 敵生成

void EnemyManager::add(unique_ptr<EnemyBase> enemy)
{
    mEnemies.push_back(std::move(enemy));
}

#pragma mark - プレイヤー衝突

void EnemyManager::checkPlayerCollision(Player& player)
{
    if (player.isDead()) return;
    RectF pr = player.getDefenceRect();
    for (auto it = mEnemies.begin(); it != mEnemies.end(); ++it)
    {
        if ((*it)->getAttackRect().intersects(pr))
        {
            player.onHitEnemy();
            (*it)->onHitPlayer();
            if ((*it)->isDead())
                mEnemies.erase(it);
            break;
        }
    }
}

#pragma mark - プレイヤー弾衝突

int EnemyManager::checkPlayerBulletCollisions(vector<unique_ptr<BulletBase>>& bullets)
{
    int hitCount = 0;
    for (auto bt = bullets.begin(); bt != bullets.end();)
    {
        BulletBase* base = bt->get();
        if (base->getType() != BulletType::Player)
        {
            ++bt;
            continue;
        }
        auto* pb = static_cast<PlayerBulletBase*>(base);
        bool consumed = false;
        RectF brect = pb->getCollisionRect();

        for (auto et = mEnemies.begin(); et != mEnemies.end(); ++et)
        {
            if ((*et)->isDead()) continue;
            if (brect.intersects((*et)->getDefenceRect()))
            {
                (*et)->onHitBullet();
                if ((*et)->isDead())
                    mEnemies.erase(et);

                if (!pb->handleHit())
                    consumed = true;

                ++hitCount;
                break;
            }
        }

        if (consumed)
            bt = bullets.erase(bt);
        else
            ++bt;
    }
    return hitCount;
}

#pragma mark - サポート弾衝突

int EnemyManager::checkSupportUnitBulletCollisions(vector<unique_ptr<BulletBase>>& bullets)
{
    int hitCount = 0;
    for (auto bt = bullets.begin(); bt != bullets.end();)
    {
        BulletBase* base = bt->get();
        if (base->getType() != BulletType::SupportUnit)
        {
            ++bt;
            continue;
        }
        bool hit = false;
        RectF brect = base->getCollisionRect();

        for (auto et = mEnemies.begin(); et != mEnemies.end(); ++et)
        {
            if ((*et)->isDead()) continue;
            if (brect.intersects((*et)->getDefenceRect()))
            {
                (*et)->onHitBullet();
                if ((*et)->isDead())
                    mEnemies.erase(et);
                hit = true;
                break;
            }
        }

        if (hit)
        {
            ++hitCount;
            bt = bullets.erase(bt);
        }
        else
        {
            ++bt;
        }
    }
    return hitCount;
}

#pragma mark - 更新・描画

void EnemyManager::update(BulletManager& bulletManager)
{
    for (auto it = mEnemies.begin(); it != mEnemies.end();)
    {
        (*it)->update(bulletManager);
        if ((*it)->isDead())
            it = mEnemies.erase(it);
        else
            ++it;
    }
}

void EnemyManager::draw() const
{
    for (const auto& e : mEnemies)
    {
        e->draw();
    }
}

