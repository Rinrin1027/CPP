//
//  EnemyGenerator.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#ifndef EnemyGenerator_hpp
#define EnemyGenerator_hpp

#include <Siv3D.hpp>
#include "EnemyManager.hpp"


using namespace std;


/// 敵の生成を行うクラス
/// @note 1つのEnemyGeneratorは複数の敵を生成することができます。
class EnemyGenerator : public enable_shared_from_this<EnemyGenerator>
{
private:
    int mID;
    double mStartTime;      //!< 出現開始までの待機時間
    int mType;              //!< 敵の種類（例：1ならEnemy1）
    Vec2 mSpawnPos;         //!< 敵の生成位置X座標
    int mSpawnCount;        //!< 生成する敵の総数
    double mSpawnDuration;  //!< 敵を生成する間隔（秒）
    String mSpawnPattern;   //!< 敵を生成する際の特殊パターン
    double mSpawnOffsetY;   //!< 敵を生成する際の特殊パターンのためのY方向のオフセット
    bool mIsItemGroup;      //!< 敵の一群を倒した場合にアイテムにするかどうか

    double mTimer;          //!< 内部で経過時間を蓄積する変数
    int mSpawnedCount;      //!< 既に生成した敵の数
    bool mStarted;          //!< 出現開始済みか
    int mActiveEnemyCount;  //!< 現在出現している敵の数

public:
    /// コンストラクタ
    /// @param element XML要素
    /// @param generatorID 敵生成器のID
    EnemyGenerator(const XMLElement& element, int generatorID);

    /// デストラクタ
    ~EnemyGenerator();

public:
    /// 敵の生成器のIDを取得
    int getID() const;
    
    /// 条件を満たしたらenemyManagerに敵を追加する
    /// @param enemyManager 敵の管理クラス
    void update(EnemyManager& enemyManager);

    /// 全ての敵が生成済みかを判定
    bool isFinished() const;
    
    /// 敵がされた時に呼ばれるコールバック関数
    void onEnemySpawned();

    /// この生成器から生成された敵が倒された時に呼ばれるコールバック関数
    void onEnemyDefeated();

    /// この生成器から生成された敵の一群を倒した場合にアイテムにするかどうかを取得
    bool shouldDropItem() const;
};

#endif /* EnemyGenerator_hpp */
