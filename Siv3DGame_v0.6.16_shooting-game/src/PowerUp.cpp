//
//  PowerUp.cpp
//  Siv3DGame
//
//  Created by numata on 2025/03/30.
//

#include "PowerUp.hpp"


// コンストラクタ
PowerUp::PowerUp(const Vec2& pos)
    : mPos(pos)
{
    mTex = Texture(Emoji(U"🔮"));
    mV = {-100, 0};
    mSize = {25, 25};
}

// パワーアップ・アイテムの位置を更新
void PowerUp::update()
{
    mPos += mV * Scene::DeltaTime();
}

// パワーアップ・アイテムを描画（現在位置を中心とした描画）
void PowerUp::draw() const
{
    mTex.resized(mSize).drawAt(mPos);
}

// プレイヤーとの交差判定用の矩形取得
RectF PowerUp::getRect() const
{
    return RectF(mPos - mSize / 2, mSize);
}

