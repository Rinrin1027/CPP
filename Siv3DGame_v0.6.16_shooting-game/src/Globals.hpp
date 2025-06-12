//
//  Globals.hpp
//  Siv3DGame
//
//  Created by numata on 2025/03/20.
//

#ifndef Globals_h
#define Globals_h

#include <Siv3D.hpp>

/// 文字列を元にシーンを管理するクラス
using GameSceneManager = SceneManager<String>;

// マクロ定数
#define GAME_TITLE          U"My Shooting Game"
#define SHOW_MAP_GRID       0
#define SHOW_ATTACK_RECT    0
#define SHOW_DEFENCE_RECT   0
#define SHOW_BULLET_RECT    0

// グローバル定数
namespace Globals
{
/// プレイヤーが一度に発射可能な（画面上に存在可能な）弾の数
inline constexpr int kMaxPlayerBulletCount = 5;

/// マップチップの基本サイズ
inline constexpr int kBlockSize = 25;

/// マップチップのグラフィックス素材上の基本サイズ
inline constexpr int kChipSize = 40;

/// サポート機がプレイヤーを追従するためのプレイヤー位置の記憶バッファ
inline constexpr int kPlayerTrailBufferSize = 10000;

/// プレイヤーに対して敵が弾を発射する際に最低限空いていないといけない距離
inline constexpr double kMinShotDist = 200.0;

/// プレイヤーに対して敵が弾を発射する際に最低限空いていないといけない距離（効率化のための2乗値）
inline constexpr double kMinShotDistSq = kMinShotDist * kMinShotDist;

/// 画面からはみ出したか判定するためのマージン（安全枠(＝ブロックサイズ)25 + スプライトのかすり余裕10 = 合計35）
inline constexpr int kSafeMargin = 35;
}

/// オーディオ再生のためのヘルパー関数。オーディオが再生中の場合は停止してから再生する。
/// @param audio 再生するオーディオ
/// @param startTime 再生開始時間（秒）
inline void PlayAudio(Audio& audio, double startTime = 0.0)
{
    if (audio.isPlaying()) {
        audio.stop();
    }
    audio.seekTime(startTime);
    audio.play();
}

#endif /* Globals_h */
