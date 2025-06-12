//
//  PlayScene.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include <Siv3D.hpp>
#include "PlayScene.hpp"
#include "PlayerBullet.hpp"
#include "EffectPlayerExplosion.hpp"

using namespace s3d;

#pragma mark - static変数

PlayScene* PlayScene::instance = nullptr;

#pragma mark - コンストラクタ・デストラクタ

PlayScene::PlayScene(const InitData& init)
    : IScene(init)
    , mIsPausing(false)
    , mTimeAfterPlayerDeath(0.0)
    , mPlayerSpeed(80.0)
    , mScore(0)
    , mFireInterval(0.5)        // 連射間隔 0.5秒
    , mTimeSinceLastShot(0.0)
{
    instance = this;

    try
    {
        mMap                   = make_unique<ScrollingMap>(U"assets/stage.csv");
        mEnemyGeneratorManager = make_unique<EnemyGeneratorManager>(U"assets/enemy.xml");
    }
    catch (const Error& e)
    {
        Logger << U"PlayScene 初期化失敗: " << e.what();
        std::exit(1);
    }

    mScoreFont         = Font(FontMethod::MSDF, 20, Typeface::Bold);
    mBGM               = Audio(U"assets/bgm2.mp3", Loop::Yes);
    mBGM.setVolume(0.7);
    mSE_Shot           = Audio(U"assets/shot4.mp3");
    mSE_Item           = Audio(U"assets/item.mp3");
    mSE_SpeedUp        = Audio(U"assets/speedup.mp3");
    mSE_AddSupportUnit = Audio(U"assets/unit_add.mp3");
    mSE_GameOver       = Audio(U"assets/gameover.mp3");
    mSE_GameOver.setVolume(1.3);

    mBGM.play();
}

PlayScene::~PlayScene()
{
    mBGM.stop();
    TextureAsset::UnregisterAll();
    instance = nullptr;
}

#pragma mark - update / draw

void PlayScene::update()
{
    // ポーズ切替
    if (KeyEscape.down()) mIsPausing = !mIsPausing;
    if (mIsPausing) return;

    // 無敵モード切替
    if (KeyEnter.down()) mPlayer.toggleInvincible();

    // 各種更新
    mMap->update();
    mEnemyGeneratorManager->update(mEnemyManager);
    mPowerUpManager.update();

    // プレイヤー死亡後の再スタート
    if (mPlayer.isDead())
    {
        mTimeAfterPlayerDeath += Scene::DeltaTime();
        if (mTimeAfterPlayerDeath >= 4.0)
        {
            changeScene(U"Play");
            return;
        }
    }
    else
    {
        updatePlayer();
        for (auto& unit : mSupportUnits)
        {
            unit->update(mPlayer.getTrail());
        }
    }

    mEnemyManager.update(mBulletManager);
    mBulletManager.update();
    checkCollisions();
    mEffectSystem.update();

    // テスト：左クリックでパワーアップ追加
    if (MouseL.down())
    {
        mPowerUpManager.add(Cursor::Pos());
    }
}

void PlayScene::draw() const
{
    // 背景・マップ・敵・アイテム
    Scene::SetBackground(ColorF(0,0,0));
    mMap->draw();
    mEnemyManager.draw();
    mPowerUpManager.draw();

    // プレイヤー＆サポートユニット
    if (!mPlayer.isDead())
    {
        for (auto& unit : mSupportUnits)
        {
            unit->draw();
        }
        mPlayer.draw();
    }

    // 弾・エフェクト
    mBulletManager.draw();
    mEffectSystem.draw();

    #pragma mark - UI描画

    // スコア
    {
        const String scoreStr = U"SCORE {0}"_fmt(mScore);
        mScoreFont(scoreStr).draw(12, 6, Palette::Gray);
        mScoreFont(scoreStr).draw(10, 4, Palette::White);
    }

    // 無敵モード表示
    {
        const bool inv = mPlayer.isInvincible();
        const ColorF col = inv ? Palette::Yellow : Palette::Gray;
        mScoreFont(U"無敵モード: {0}"_fmt(inv ? U"ON" : U"OFF"))
            .draw(Scene::Width() - 200, 50, col);
    }

    // スプリントゲージ（最前面）
    {
        const double rate = mPlayer.getSprintGauge() / Player::getMaxSprintGauge();
        const double w = 120.0, h = 8.0;
        const double x = 10.0, y = 60.0;
        RectF(x, y, w, h).drawFrame(2.0, Palette::White);
        RectF(x, y, w * rate, h).draw(Palette::Green);
        RectF(x, y, w, h).drawFrame(1.0, Palette::White);
    }
}

#pragma mark - public getters

EffectSystem& PlayScene::getEffectSystem() { return mEffectSystem; }
Player&       PlayScene::getPlayer()       { return mPlayer; }
EnemyManager& PlayScene::getEnemyManager() { return mEnemyManager; }
void          PlayScene::addPowerUpItem(const Vec2& pos) { mPowerUpManager.add(pos); }
void          PlayScene::addScore(int score)            { mScore += score; }

#pragma mark - private functions

void PlayScene::updatePlayer()
{
    // ΔTime込みの基本速度
    const double baseSpeed = mPlayerSpeed * Scene::DeltaTime();

    // 入力方向取得
    Vec2 inputVec{0,0};
    if (KeyUp.pressed())    inputVec.y -= 1;
    if (KeyDown.pressed())  inputVec.y += 1;
    if (KeyLeft.pressed())  inputVec.x -= 1;
    if (KeyRight.pressed()) inputVec.x += 1;

    // 移動
    mPlayer.move(inputVec, baseSpeed);
    if (inputVec.lengthSq() > 0)
    {
        mPlayer.updateTrail();
    }

    // 発射オフセット（右20, 上10）
    constexpr Vec2 spawnOffset{ 60, -2 };

    // 発射方向：移動方向 or 右向き
    Vec2 bulletDir = inputVec.lengthSq() > 0 ? inputVec.normalized() : Vec2{1, 0};

    // 発射用ラムダ
    auto fireOne = [&](const Vec2& dir, int hits = 1, bool homing = false)
    {
        auto pb = make_unique<PlayerBullet1>(
            mPlayer.getPos() + spawnOffset,
            dir * 800.0);
        pb->setRemainingHits(hits);
        if (homing) pb->setHoming(true);
        mBulletManager.add(std::move(pb));
    };

    // Gキー：3方向弾（正面＋±30°）
    if (KeyG.down() &&
        mBulletManager.getPlayerSideCount() + 3 <= Globals::kMaxPlayerBulletCount)
    {
        fireOne(bulletDir);
        fireOne(bulletDir.rotated(30.0_deg));
        fireOne(bulletDir.rotated(-30.0_deg));
    }

    // Kキー：貫通ホーミング弾（残ヒット2回＋ホーミング）
    if (KeyK.down() &&
        mBulletManager.getPlayerSideCount() < Globals::kMaxPlayerBulletCount)
    {
        fireOne(bulletDir, 2, true);
    }

    // スペース連射制御
    bool doFire = false;
    if (KeySpace.down())
    {
        doFire = true;
        mTimeSinceLastShot = 0.0;
    }
    else if (KeySpace.pressed())
    {
        mTimeSinceLastShot += Scene::DeltaTime();
        if (mTimeSinceLastShot >= mFireInterval)
        {
            doFire = true;
            mTimeSinceLastShot = 0.0;
        }
    }

    if (doFire &&
        mBulletManager.getPlayerSideCount() < Globals::kMaxPlayerBulletCount)
    {
        fireOne(bulletDir);

        // オプション機からも発射
        for (auto& unit : mSupportUnits)
        {
            unit->fireBullets(mBulletManager);
        }
    }
}

void PlayScene::checkCollisions()
{
    // パワーアップ取得
    if (mPowerUpManager.checkPlayerCollision(mPlayer.getPowerUpRect()))
    {
        PlayAudio(mSE_Item);
        int count = mPowerUpManager.getCollectedCount();
        if (count == 1)
        {
            PlayAudio(mSE_SpeedUp);
            mPlayerSpeed = 150.0;
        }
        else
        {
            PlayAudio(mSE_AddSupportUnit);
            int delay = 600 * (static_cast<int>(mSupportUnits.size()) + 1);
            mSupportUnits.push_back(make_unique<SupportUnit>(delay));
        }
    }

    // 障害物との衝突
    if (!mPlayer.isDead() && mMap->intersectsRect(mPlayer.getDefenceRect()))
    {
        mPlayer.onHitMap();
        if (mPlayer.isDead())
        {
            PlayAudio(mSE_GameOver);
            mTimeAfterPlayerDeath = 0.0;
        }
    }

    // 敵弾との衝突
    if (!mPlayer.isDead())
    {
        mBulletManager.checkEnemyBulletCollisions(mPlayer);
        if (mPlayer.isDead())
        {
            PlayAudio(mSE_GameOver);
            mTimeAfterPlayerDeath = 0.0;
        }
    }

    // 敵との衝突
    if (!mPlayer.isDead())
    {
        mEnemyManager.checkPlayerCollision(mPlayer);
        if (mPlayer.isDead())
        {
            PlayAudio(mSE_GameOver);
            mTimeAfterPlayerDeath = 0.0;
        }
    }

    // プレイヤー＆サポート弾 vs 敵
    mBulletManager.checkPlayerOrSupportUnitBulletCollisions(mEnemyManager);

    // 弾 vs 地形
    mBulletManager.checkMapCollisions(*mMap);
}

