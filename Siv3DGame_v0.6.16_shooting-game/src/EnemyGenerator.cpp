//
//  EnemyGenerator.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include "EnemyGenerator.hpp"
#include "Enemy1.hpp"
#include "Enemy2.hpp"
#include "Enemy3.hpp"
#include "Enemy4.hpp"


/// XML読み込みのためのヘルパー関数。XMLの要素からint型の属性を取得する。
/// 属性が存在しない場合は例外を投げる。
/// @param element XML要素
/// @param attrName 属性名
static int GetXMLAttributeInt(const XMLElement& element, const String& attrName)
{
    if (auto obj = element.attribute(attrName)) {
        return Parse<int>(obj.value());
    } else {
        throw Error(U"XML element <{0}> does not have an attribute `{1}`"_fmt(element.name(), attrName));
    }
}

/// XML読み込みのためのヘルパー関数。XMLの要素からdouble型の属性を取得する。
/// 属性が存在しない場合は例外を投げる。
/// @param element XML要素
/// @param attrName 属性名
static double GetXMLAttributeDouble(const XMLElement& element, const String& attrName)
{
    if (auto obj = element.attribute(attrName)) {
        return Parse<double>(obj.value());
    } else {
        throw Error(U"XML element <{0}> does not have an attribute `{1}`"_fmt(element.name(), attrName));
    }
}

/// XML読み込みのためのヘルパー関数。XMLの要素からint型の属性を取得し、属性が存在しない場合はデフォルト値を返す。
/// @param element XML要素
/// @param attrName 属性名
/// @param defaultValue デフォルト値
static int TryXMLAttributeInt(const XMLElement& element, const String& attrName, int defaultValue)
{
    if (auto obj = element.attribute(attrName)) {
        return Parse<int>(obj.value());
    } else {
        return defaultValue;
    }
}

/// XML読み込みのためのヘルパー関数。XMLの要素からdouble型の属性を取得し、属性が存在しない場合はデフォルト値を返す。
/// @param element XML要素
/// @param attrName 属性名
/// @param defaultValue デフォルト値
static double TryXMLAttributeDouble(const XMLElement& element, const String& attrName, double defaultValue)
{
    if (auto obj = element.attribute(attrName)) {
        return Parse<double>(obj.value());
    } else {
        return defaultValue;
    }
}

/// XML読み込みのためのヘルパー関数。XMLの要素から文字列型の属性を取得し、属性が存在しない場合はデフォルト値を返す。
/// @param element XML要素
/// @param attrName 属性名
/// @param defaultValue デフォルト値
static String TryXMLAttributeString(const XMLElement& element, const String& attrName, const String& defaultValue)
{
    if (auto obj = element.attribute(attrName)) {
        return obj.value();
    } else {
        return defaultValue;
    }
}


// コンストラクタ。XMLファイルから全てのenemy要素に対応するEnemyGeneratorを生成する
EnemyGenerator::EnemyGenerator(const XMLElement& element, int generatorID)
{
    mID = generatorID;
    
    mStartTime = GetXMLAttributeDouble(element, U"time");
    mType = GetXMLAttributeInt(element, U"type");
    mSpawnPos.x = GetXMLAttributeDouble(element, U"x");
    mSpawnPos.y = GetXMLAttributeDouble(element, U"y");
    mSpawnCount = TryXMLAttributeInt(element, U"count", 1);
    mSpawnDuration = TryXMLAttributeDouble(element, U"duration", 0.0);
    mSpawnPattern = TryXMLAttributeString(element, U"pattern", U"");
    mSpawnOffsetY = TryXMLAttributeDouble(element, U"offset_y", 0.0);
    mIsItemGroup = (TryXMLAttributeInt(element, U"item", 0) == 1);
    
    mActiveEnemyCount = 0;
}

// デストラクタ
EnemyGenerator::~EnemyGenerator()
{
    //Print << U"EnemyGenrator {0} is being deleted."_fmt(mID);
}

// 敵の生成器のIDを取得
int EnemyGenerator::getID() const
{
    return mID;
}

// deltaTimeを加算しながら、条件を満たしたらenemyManagerに敵を追加する
void EnemyGenerator::update(EnemyManager& enemyManager)
{
    mTimer += Scene::DeltaTime();
    
    // 出現開始前なら開始時刻まで待つ
    if (!mStarted) {
        if (mTimer >= mStartTime) {
            mStarted = true;
            mTimer -= mStartTime;
        } else {
            return;
        }
    }
    
    // spawn-durationごとに敵を生成（spawn-countに達するまで）
    while (mTimer >= mSpawnDuration && mSpawnedCount < mSpawnCount) {
        mTimer -= mSpawnDuration;
        mSpawnedCount++;
        
        // 特殊パターンの出現方法のサポート
        Vec2 pos = mSpawnPos;
        if (mSpawnPattern == U"center_wave") {
            int offsetSign = (mSpawnedCount % 2 == 1)? -1: 1;
            int offsetY = (mSpawnedCount / 2) * offsetSign;
            pos.y += offsetY * mSpawnOffsetY;
        }
        
        // type に応じた敵を生成
        unique_ptr<EnemyBase> enemy;
        if (mType == 1) {
            enemy = make_unique<Enemy1>(pos);
        } else if (mType == 2) {
            enemy = make_unique<Enemy2>(pos);
        } else if (mType == 3) {
            enemy = make_unique<Enemy3>(pos);
        }  else if (mType == 4) {
            enemy = make_unique<BarrierEnemy>(pos);
        } else if (mType == 5) {
            enemy = make_unique<SpreaderEnemy>(pos);
        } else {
            throw Error(U"Unknown enemy type: {0} in generator ID: {1}"_fmt(mType, mID));
        }

        enemy->setGenerator(shared_from_this());
        onEnemySpawned();
        enemyManager.add(std::move(enemy));
    }
}

// 全ての敵が生成済みかを判定
bool EnemyGenerator::isFinished() const
{
    return mSpawnedCount >= mSpawnCount;
}

// 敵が生成された時に呼ばれるコールバック関数
void EnemyGenerator::onEnemySpawned()
{
    mActiveEnemyCount++;
}

// この生成器から生成された敵が倒された時に呼ばれるコールバック関数
void EnemyGenerator::onEnemyDefeated()
{
    if (mActiveEnemyCount > 0) {
        mActiveEnemyCount--;
    }
}

// この生成器から生成された敵の一群を倒した場合にアイテムにするかどうかを取得
bool EnemyGenerator::shouldDropItem() const
{
    return mIsItemGroup && (mActiveEnemyCount == 0) && isFinished();
}

