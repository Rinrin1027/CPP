//
//  PowerUp.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#ifndef PowerUp_hpp
#define PowerUp_hpp

#include <Siv3D.hpp>


/// パワーアップアイテムを表すクラス
class PowerUp
{
private:
    Vec2 mPos;      //!< 現在位置（描画の中心位置）
    Vec2 mSize;     //!< サイズ
    Vec2 mV;        //!< 移動ベクトル
    Texture mTex;   //!< 描画のためのテクスチャ

public:
    /// コンストラクタ
    /// @param pos 初期位置
    PowerUp(const Vec2& pos);

public:
    /// プレイヤーとの交差判定用の矩形取得
    RectF getRect() const;

    /// パワーアップアイテムの位置を更新
    void update();

    /// パワーアップアイテムを描画
    void draw() const;
};

#endif /* PowerUp_hpp */

