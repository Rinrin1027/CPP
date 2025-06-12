//
//  EnemyGeneratorManager.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#ifndef EnemyGeneratorManager_hpp
#define EnemyGeneratorManager_hpp

#include <Siv3D.hpp>
#include <memory>
#include <vector>
#include "EnemyGenerator.hpp"


using namespace std;


/// 敵の生成を行うEnemyGeneratorを管理するクラス
class EnemyGeneratorManager
{
private:
    vector<shared_ptr<EnemyGenerator>> mGenerators; //!< 敵の生成を行うEnemyGeneratorのリスト
    int mNextGeneratorID;   //!< 次に生成するEnemyGeneratorに付与するID

public:
    /// コンストラクタ。XMLファイルから全てのenemy要素に対応するEnemyGeneratorを生成する
    /// @param xmlPath XMLファイルのパス
    EnemyGeneratorManager(const FilePath& xmlPath);

    /// 各EnemyGeneratorを更新する
    /// @param enemyManager 敵の管理クラス
    void update(EnemyManager& enemyManager);
};

#endif /* EnemyGeneratorManager_hpp */

