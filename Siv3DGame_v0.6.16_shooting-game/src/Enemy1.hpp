//
//  Enemy1.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/29.
//

#ifndef Enemy1_hpp
#define Enemy1_hpp

#include "EnemyBase.hpp"


/// 敵キャラクターのクラス
class Enemy1 : public EnemyBase
{
private:
    Texture mTex;           //!< 描画のためのテクスチャ
    Vec2    mV;             //!< 速度ベクトル
    Vec2    mSize;          //!< サイズ
    int     mState;         //!< 現在の状態
    double  mLastShotTime;  //!< 最後に弾を撃った時間（0.0からスタート）

public:
    /// コンストラクタ
    /// @param pos 敵の初期位置
    Enemy1(const Vec2& pos);

    /// プレイヤー攻撃用の矩形を取得
    RectF getAttackRect() const override;

    /// プレイヤーからの攻撃を受けるための矩形を取得
    RectF getDefenceRect() const override;

    /// 敵の状態を更新し、必要に応じて弾を発射する
    /// @param bulletManager 弾情報の管理クラス
    void update(BulletManager& bulletManager) override;

    /// 敵を描画
    void draw() const override;
};

#endif /* Enemy1_hpp */
