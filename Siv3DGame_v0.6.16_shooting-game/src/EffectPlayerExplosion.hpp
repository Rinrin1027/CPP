//
//  EffectPlayerExplosion.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#ifndef EffectPlayerExplosion_hpp
#define EffectPlayerExplosion_hpp

#include "EffectSystem.hpp"


/// プレイヤーが爆発した際のエフェクトを表すクラス
class EffectPlayerExplosion : public EffectBase
{
private:
    Vec2 mPos;          //!< 現在位置（描画の中心位置）
    double mTime = 0.0; //!< 経過時間

public:
    /// コンストラクタ
    /// @param pos 爆発の位置
    EffectPlayerExplosion(const Vec2& pos);

    /// エフェクトの更新
    void update() override;

    /// エフェクトの描画
    void draw() const override;

    /// エフェクトが終了したかどうかを判定
    bool isFinished() const override;
};


#endif /* EffectPlayerExplosion_hpp */
