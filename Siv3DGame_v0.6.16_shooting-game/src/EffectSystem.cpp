//
//  EffectSystem.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#include "EffectSystem.hpp"


// すべてのエフェクトの更新
void EffectSystem::update()
{
    for (auto& effect : mEffects) {
        effect->update();
    }

    // 終了済みのエフェクトを削除
    erase_if(mEffects, [](const unique_ptr<EffectBase>& e) {
        return e->isFinished();
    });
}

// すべてのエフェクトの描画
void EffectSystem::draw() const
{
    for (const auto& effect : mEffects) {
        effect->draw();
    }
}

// 新しいエフェクトを追加
/// @param effect 追加するエフェクト
void EffectSystem::add(unique_ptr<EffectBase> effect)
{
    mEffects.push_back(std::move(effect));
}

