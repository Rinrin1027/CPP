//
//  PowerUpManager.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#include "PowerUpManager.hpp"


// コンストラクタ
PowerUpManager::PowerUpManager()
{
    mCollectedCount = 0;
}

// 新しいパワーアップ・アイテムを追加する
// @param pos アイテムを追加する座標（中心位置）
void PowerUpManager::add(const Vec2& pos)
{
    mPowerUps.push_back(make_unique<PowerUp>(pos));
}

// 現在までに獲得したパワーアップ・アイテムの個数を取得
int PowerUpManager::getCollectedCount() const
{
    return mCollectedCount;
}

// プレイヤーとパワーアップ・アイテムの衝突判定
bool PowerUpManager::checkPlayerCollision(const RectF& playerRect)
{
    bool collected = false;
    for (auto it = mPowerUps.begin(); it != mPowerUps.end();) {
        if ((*it)->getRect().intersects(playerRect)) {
            collected = true;
            mCollectedCount++;
            it = mPowerUps.erase(it);
        } else {
            it++;
        }
    }
    return collected;
}

// すべてのパワーアップ・アイテムの更新
void PowerUpManager::update()
{
    for (auto& powerUp : mPowerUps) {
        powerUp->update();
    }
}

// すべてのパワーアップ・アイテムの描画
void PowerUpManager::draw() const
{
    for (auto& powerUp : mPowerUps) {
        powerUp->draw();
    }
}

