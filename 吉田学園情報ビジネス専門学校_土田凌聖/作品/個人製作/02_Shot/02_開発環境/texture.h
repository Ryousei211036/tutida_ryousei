//**********************************************************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**********************************************************************************************
#ifndef _TEXTURE_H_	//このマクロ定義がされてなかったら
#define _TEXTURE_H_	//２重インクルード防止のマクロ定義

//==============================================================================================
// インクルード
//==============================================================================================
#include <d3dx9.h>

//==============================================================================================
// 定義
//==============================================================================================
class CTexture
{
public: /* 定義 */
	enum TEXTURE
	{	
		TEXTURE_BG = 0,				// 背景
		TEXTURE_TITLE,				// タイトル画面
		TEXTURE_RESULT,				// リザルト画面
		TEXTURE_PRESSENTER,			// PressEnter
		TEXTURE_GAMEUI,				// ゲーム画面のUI
		TEXTURE_RANKING,			// ランキングのUI
		TEXTURE_PLAYERBULLET,		// プレイヤーの弾
		TEXTURE_ENEMYBULLET,		// 敵の弾
		TEXTURE_PLAYER0,			// プレイヤー
		TEXTURE_PLAYER1,			// プレイヤー１
		TEXTURE_PLAYER2,			// プレイヤー２
		TEXTURE_PLAYER3,			// プレイヤー３
		TEXTURE_PLAYER4,			// プレイヤー４
		TEXTURE_PLAYER5,			// プレイヤー５
		TEXTURE_PLAYER6,			// プレイヤー６
		TEXTURE_PLAYER7,			// プレイヤー７
		TEXTURE_PLAYER8,			// プレイヤー８
		TEXTURE_PLAYER9,			// プレイヤー９
		TEXTURE_EVOLUTION1,			// 進化１
		TEXTURE_EVOLUTION2,			// 進化２
		TEXTURE_EVOLUTION3,			// 進化３
		TEXTURE_EVOLUTION4,			// 進化４
		TEXTURE_EVOLUTION5,			// 進化５
		TEXTURE_EVOLUTION6,			// 進化６
		TEXTURE_EVOLUTION7,			// 進化７
		TEXTURE_EVOLUTION8,			// 進化８
		TEXTURE_EVOLUTION9,			// 進化９
		TEXTURE_ENEMY0,				// 敵
		TEXTURE_ENEMY1,				// 敵１
		TEXTURE_ENEMY2,				// 敵２
		TEXTURE_ENEMY3,				// 敵３
		TEXTURE_ENEMY4,				// 敵４
		TEXTURE_ENEMY5,				// 敵５
		TEXTURE_ENEMY6,				// 敵６
		TEXTURE_ENEMY7,				// 敵７
		TEXTURE_ENEMY8,				// 敵８
		TEXTURE_ENEMY9,				// 敵９
		TEXTURE_EXPLOSION,			// 爆発
		TEXTURE_EXTRIANGLE,			// 餌１
		TEXTURE_EXBOX,				// 餌２
		TEXTURE_EXPENTAGON,			// 餌３
		TEXTURE_NUMBER,				// 数字
		TEXTURE_STATUS,				// ステータス
		TEXTURE_LEVELMAX,			// レベルMax
		TEXTURE_MAX,
		TEXTURE_NONE,
	};

	static const char* s_FileName[];	// ファイルパス

public:
	CTexture();		// デフォルトコンストラクタ
	~CTexture();	// デストラクタ

public: /* メンバ関数 */
	void LoadAll();										// 全ての読み込み
	void Load(TEXTURE inTexture);						// 指定の読み込み
	void ReleaseAll();									// 全ての破棄
	void Release(TEXTURE inTexture);					// 指定の破棄
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// 情報の取得

private: /* メンバ変数 */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// テクスチャの情報
};

#endif // !_TEXTURE_H_
