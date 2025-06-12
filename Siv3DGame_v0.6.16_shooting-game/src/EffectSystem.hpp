//
//  EffectSystem.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/22.
//

#ifndef EffectSystem_hpp
#define EffectSystem_hpp

#include <Siv3D.hpp>
#include <memory>
#include <vector>


using namespace std;


/// 爆発などのエフェクトを表すための基本クラス
class EffectBase
{
public:
    /// デストラクタ
    virtual ~EffectBase() = default;

    /// エフェクトの更新
    virtual void update() = 0;

    /// エフェクトの描画
    virtual void draw() const = 0;

    /// エフェクトが終了したかどうかを判定
    virtual bool isFinished() const = 0;
};

/// エフェクトシステム
class EffectSystem
{
private:
    vector<unique_ptr<EffectBase>> mEffects;    //!< エフェクトを格納するコンテナ

public:
    /// すべてのエフェクトの更新
    void update();

    /// すべてのエフェクトの描画
    void draw() const;

    /// 新しいエフェクトを追加
    /// @param effect 追加するエフェクト
    void add(std::unique_ptr<EffectBase> effect);
};

#endif /* EffectSystem_hpp */
