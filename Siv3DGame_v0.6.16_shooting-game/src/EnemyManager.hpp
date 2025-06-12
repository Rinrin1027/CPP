// EnemyManager.hpp
#ifndef EnemyManager_hpp
#define EnemyManager_hpp

#include <Siv3D.hpp>
#include <vector>
#include <memory>
#include "EnemyBase.hpp"
#include "Player.hpp"
#include "BulletManager.hpp"

using namespace std;

class EnemyManager
{
private:
    vector<unique_ptr<EnemyBase>> mEnemies;

public:
    EnemyManager();
    void add(unique_ptr<EnemyBase> enemy);
    void checkPlayerCollision(Player& player);
    int  checkPlayerBulletCollisions(vector<unique_ptr<BulletBase>>& bullets);
    int  checkSupportUnitBulletCollisions(vector<unique_ptr<BulletBase>>& bullets);
    void update(BulletManager& bulletManager);
    void draw() const;

    /// ホーミング用：現在の敵リストを返す
    const vector<unique_ptr<EnemyBase>>& getEnemies() const { return mEnemies; }
};

#endif /* EnemyManager_hpp */

