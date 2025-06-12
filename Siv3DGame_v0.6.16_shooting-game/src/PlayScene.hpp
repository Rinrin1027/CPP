// PlayScene.hpp
#pragma once
#include <Siv3D.hpp>
#include "Player.hpp"
#include "ScrollingMap.hpp"
#include "EnemyGeneratorManager.hpp"
#include "PowerUpManager.hpp"
#include "EnemyManager.hpp"         // ← 追加済み
#include "BulletManager.hpp"
#include "EffectSystem.hpp"
#include "SupportUnit.hpp"

using namespace s3d;
using App = SceneManager<String>;

class PlayScene : public App::Scene
{
public:
    const ScrollingMap& getMap() const { return *mMap; }
    PlayScene(const InitData& init);
    ~PlayScene() override;

    void update() override;
    void draw() const override;

    EffectSystem& getEffectSystem();
    Player&       getPlayer();
    void          addPowerUpItem(const Vec2& pos);
    void          addScore(int score);

    /// 追加：EnemyManager へのアクセス
    EnemyManager& getEnemyManager();

    static PlayScene* instance;

private:
    void updatePlayer();
    void checkCollisions();

    std::unique_ptr<ScrollingMap>           mMap;
    std::unique_ptr<EnemyGeneratorManager>  mEnemyGeneratorManager;
    PowerUpManager                         mPowerUpManager;
    EnemyManager                           mEnemyManager;       // 実際に敵を保持する
    BulletManager                          mBulletManager;
    EffectSystem                           mEffectSystem;
    Player                                 mPlayer;
    std::vector<std::unique_ptr<SupportUnit>> mSupportUnits;

    Font   mScoreFont;
    Audio  mBGM, mSE_Shot, mSE_Item, mSE_SpeedUp, mSE_AddSupportUnit, mSE_GameOver;
    bool   mIsPausing;
    double mTimeAfterPlayerDeath;
    double mPlayerSpeed;
    int    mScore;

    // 連射制御用
    double mFireInterval;
    double mTimeSinceLastShot;
};

