//
//  Player.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/20.
//

#ifndef Player_hpp
#define Player_hpp

#include <Siv3D.hpp>
#include <deque>
#include "Globals.hpp"

using namespace std;


/// プレイヤーを表すクラス
class Player
{
private:
    Texture           mTex;           //!< テクスチャ
    Audio             mSE_Damaged;    //!< SE（ダメージを受けた）
    Vec2              mPos;           //!< 現在位置（描画の中心位置）
    deque<Vec2>       mTrail;         //!< 過去の現在位置のヒストリー
    bool              mIsDead;        //!< 死んでいるかどうか
    int               mVerticalDir;   //!< 縦移動方向（-1:上,0:横or停止,+1:下）
    bool              mIsInvincible;  //!< 無敵モードフラグ

    // ─── スプリント（ダッシュ）用ゲージ ───
    double            mSprintGauge;            //!< 現在のゲージ残量（秒）
    static constexpr double kMaxSprintGauge        = 3.0;  // 最大3秒分
    static constexpr double kSprintConsumptionRate = 1.0;  // 1秒で1ゲージ消費
    static constexpr double kSprintRegenRate       = 0.5;  // 1秒で0.5ゲージ回復

public:
    /// コンストラクタ
    Player();
    
    /// 現在位置の取得
    Vec2 getPos() const;
    
    /// 敵から攻撃を受ける際の交差判定用の矩形取得
    RectF getDefenceRect() const;

    /// パワーアップ・アイテムを取得する際の交差判定用の矩形取得
    RectF getPowerUpRect() const;
    
    /// 死んでいるかどうかの確認
    bool isDead() const;

    /// 無敵モードかどうかの確認
    bool isInvincible() const;

    /// 過去の現在位置のヒストリーの取得
    const deque<Vec2>& getTrail() const;
    
    /**
     * プレイヤーを移動させる。
     * @param direction 押下方向ベクトル (長さは1以下)
     * @param baseSpeed ΔTime込みの基本速度 (例: mPlayerSpeed * Scene::DeltaTime())
     */
    void move(const Vec2& direction, double baseSpeed);

    /// 軌跡ヒストリーの更新
    void updateTrail();

    /// プレイヤーを描画
    void draw() const;
    
    /// 衝突時コールバック
    void onHitMap();
    void onHitEnemy();
    void onHitBullet();
    
    /// 無敵モード切り替え
    void toggleInvincible();

    /// スプリントゲージ残量取得 (0.0～kMaxSprintGauge)
    double getSprintGauge() const { return mSprintGauge; }
    /// スプリントゲージ最大値取得
    static double getMaxSprintGauge() { return kMaxSprintGauge; }
};

#endif /* Player_hpp */

