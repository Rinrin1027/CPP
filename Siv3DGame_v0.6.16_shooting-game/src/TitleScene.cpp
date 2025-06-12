#include "TitleScene.hpp"


/// タイトル表示用のフォントの名前
static const String sTitleFontName = U"TitleFont";


// コンストラクタ
TitleScene::TitleScene(const InitData& init)
    : IScene(init)
{
    Print << U"Title::Title()";
    
    // フォントの読み込み
    FontAsset::Register(sTitleFontName, FontMethod::MSDF, 60, Typeface::Heavy);

    mTexCursor = Texture(Emoji(U"🚀"));

    // その他の変数の初期化
    mTime = 0.0;
}

// デストラクタ
TitleScene::~TitleScene()
{
    FontAsset::UnregisterAll();
    Print << U"Title::~Title()";
}

// シーンの更新
void TitleScene::update()
{
    mTime += Scene::DeltaTime();
    if (MouseL.down()) {
        changeScene(U"Play", Duration{1.2});
    }
}

// シーンの描画
void TitleScene::draw() const
{
    Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

    FontAsset(sTitleFontName)(U"My Game").drawAt(400, 100, Palette::White);
    
    Circle(Cursor::Pos(), 50).draw(Palette::Orange);

    mTexCursor.resized(64, 64).rotated(mTime).drawAt(Cursor::Pos());
}

