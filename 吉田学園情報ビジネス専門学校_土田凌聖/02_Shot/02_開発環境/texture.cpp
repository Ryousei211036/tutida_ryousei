//**********************************************************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**********************************************************************************************

//==============================================================================================
// インクルード
//==============================================================================================
#include "application.h"
#include "texture.h"
#include "renderer.h"

#include <assert.h>

//==============================================================================================
// 定義
//==============================================================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"Data/Texture/bg.jpg",				// 背景
	"Data/Texture/title001.jpg",		// タイトル画面
	"Data/Texture/result.jpg",			// リザルト画面
	"Data/Texture/PressEnter.png",		// PressEnter
	"Data/Texture/gameUI.png",			// ゲーム画面のUI
	"Data/Texture/RankingUI.png",		// ランキングのUI
	"Data/Texture/bullet000.png",		// 弾
	"Data/Texture/enemybullet000.png",	// 弾
	"Data/Texture/tank000.png",			// プレイヤー０
	"Data/Texture/tank001.png",			// プレイヤー１
	"Data/Texture/tank002.png",			// プレイヤー２
	"Data/Texture/tank003.png",			// プレイヤー３
	"Data/Texture/tank004.png",			// プレイヤー４
	"Data/Texture/tank005.png",			// プレイヤー５
	"Data/Texture/tank006.png",			// プレイヤー６
	"Data/Texture/tank007.png",			// プレイヤー７
	"Data/Texture/tank008.png",			// プレイヤー８
	"Data/Texture/tank009.png",			// プレイヤー９
	"Data/Texture/evolution000.png",	// 進化１
	"Data/Texture/evolution001.png",	// 進化２
	"Data/Texture/evolution002.png",	// 進化３
	"Data/Texture/evolution003.png",	// 進化４
	"Data/Texture/evolution004.png",	// 進化５
	"Data/Texture/evolution005.png",	// 進化６
	"Data/Texture/evolution006.png",	// 進化７
	"Data/Texture/evolution007.png",	// 進化８
	"Data/Texture/evolution008.png",	// 進化９
	"Data/Texture/enemy000.png",		// 敵
	"Data/Texture/enemy001.png",		// 敵１
	"Data/Texture/enemy002.png",		// 敵２
	"Data/Texture/enemy003.png",		// 敵３
	"Data/Texture/enemy004.png",		// 敵４
	"Data/Texture/enemy005.png",		// 敵５
	"Data/Texture/enemy006.png",		// 敵６
	"Data/Texture/enemy007.png",		// 敵７
	"Data/Texture/enemy008.png",		// 敵８
	"Data/Texture/enemy009.png",		// 敵９
	"Data/Texture/explosion000.png",	// 爆発
	"Data/Texture/Extriangle0.png",		// 餌1
	"Data/Texture/Exbox0.png",			// 餌2
	"Data/Texture/Expentagon0.png",		// 餌3
	"Data/Texture/number000.png",		// 数字
	"Data/Texture/status.png",			// ステータス
	"Data/Texture/max.png",				// レベルMAX
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//----------------------------------------------------------------------------------------------
// デフォルトコンストラクタ
//----------------------------------------------------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//----------------------------------------------------------------------------------------------
// デストラクタ
//----------------------------------------------------------------------------------------------
CTexture::~CTexture()
{
}

//----------------------------------------------------------------------------------------------
// 全ての読み込み
//----------------------------------------------------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//----------------------------------------------------------------------------------------------
// 読み込み
//----------------------------------------------------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//----------------------------------------------------------------------------------------------
// 全ての解放
//----------------------------------------------------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//----------------------------------------------------------------------------------------------
// 解放
//----------------------------------------------------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//----------------------------------------------------------------------------------------------
// 取得
//----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
