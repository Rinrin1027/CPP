#ifndef TITLE_SCENE
#define TITLE_SCENE

#include "Globals.hpp"


/// タイトルシーンを表すクラス
class TitleScene : public GameSceneManager::Scene
{
private:
    Texture mTexCursor; //!< テクスチャ
    double mTime;       //!< 時間
    
public:
    /// コンストラクタ
    /// @param init 初期化データ
    TitleScene(const InitData& init);

    /// デストラクタ
    virtual ~TitleScene();
    
    /// シーンの更新
    void update() override;

    /// シーンの描画
    void draw() const override;
};

#endif  //#ifndef TITLE_SCENE
