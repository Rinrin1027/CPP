//
//  EnemyGeneratorManager.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#include "EnemyGeneratorManager.hpp"


// コンストラクタ。XMLファイルから全てのenemy要素に対応するEnemyGeneratorを生成する
EnemyGeneratorManager::EnemyGeneratorManager(const FilePath& xmlPath)
{
    XMLReader xml(xmlPath);
    if (!xml) {
        throw Error(U"Failed to load XML file: " + xmlPath);
    }

    mNextGeneratorID = 0;
    for (auto e = xml.firstChild(); e; e = e.nextSibling()) {
        mGenerators.push_back(make_shared<EnemyGenerator>(e, mNextGeneratorID));
        mNextGeneratorID++;
    }
}

// すべてのEnemyGeneratorを更新し、敵を生成する
// @param enemyManager 敵の管理クラス
void EnemyGeneratorManager::update(EnemyManager& enemyManager)
{
    for (auto it = mGenerators.begin(); it != mGenerators.end(); ) {
        (*it)->update(enemyManager);
        if ((*it)->isFinished()) {
            //Print << U"EnemyGeneratorManager is removing generator {0}"_fmt((*it)->getID());
            it = mGenerators.erase(it);
        } else {
            it++;
        }
    }
}

