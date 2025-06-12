#include "Globals.hpp"
#include "TitleScene.hpp"
#include "PlayScene.hpp"


/// 文字列を元にシーンを管理するクラス
using GameSceneManager = SceneManager<String>;


/// ゲームのエントリポイント
void Main()
{
    // ウィンドウの設定
    Window::SetTitle(GAME_TITLE);
    Window::Resize(800, 600);
    Graphics::SetVSyncEnabled(false);
    System::SetTerminationTriggers(UserAction::CloseButtonClicked); // ESCキーで終了しないようにする

    // シーンの作成と登録
    GameSceneManager sceneManager;
    sceneManager.add<TitleScene>(U"Title");
    sceneManager.add<PlayScene>(U"Play");
    
    // 開始シーンの指定
    //sceneMgr.init(U"Title", Duration{1.2});
    sceneManager.init(U"Play", Duration{1.2});

    // ゲームループ
    while (System::Update()) {
        if (!sceneManager.update()) {
            break;
        }
    }
}

