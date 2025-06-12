//
//  EffectEnemyExplosion.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#ifndef EffectEnemyExplosion_hpp
#define EffectEnemyExplosion_hpp

#include "EffectSystem.hpp"


/// 敵が爆発した際のエフェクトを表すクラス
class EffectEnemyExplosion : public EffectBase
{
private:
    Vec2 mPos;          //!< 現在位置（描画の中心位置）
    double mTime = 0.0; //!< 経過時間

public:
    /// コンストラクタ
    /// @param pos 爆発の位置
    EffectEnemyExplosion(const Vec2& pos);

    /// エフェクトの更新
    void update() override;

    /// エフェクトの描画
    void draw() const override;

    /// エフェクトが終了したかどうかを判定
    bool isFinished() const override;
};


#endif /* EffectEnemyExplosion_hpp */
