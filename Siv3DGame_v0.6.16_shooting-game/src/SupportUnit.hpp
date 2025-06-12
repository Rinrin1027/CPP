//
//  SupportUnit.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#ifndef SupportUnit_hpp
#define SupportUnit_hpp

#include <Siv3D.hpp>
#include <deque>


class BulletManager;

using namespace std;


/// サポート機を表すクラス
class SupportUnit
{
private:
    Texture mTex;           //!< テクスチャ
    Vec2    mPos;           //!< 現在位置（描画の中心位置）
    int     mFollowDelay;   //!< 何フレーム遅れてプレイヤーの現在位置に追従するか

public:
    /// コンストラクタ
    SupportUnit(int delayFrames);

    /// 現在位置の取得
    Vec2 getPos() const;
    
    /// サポート機の更新
    void update(const deque<Vec2>& playerTrail);
    
    /// 弾の発射
    void fireBullets(BulletManager& bulletManager) const;
    
    /// サポート機の描画
    void draw() const;
};

#endif /* SupportUnit_hpp */
