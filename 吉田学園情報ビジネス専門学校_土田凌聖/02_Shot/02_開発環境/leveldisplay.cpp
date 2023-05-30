//==============================================================================================
//
// レベルの表示　　　leveldisplay.cpp
// 土田凌聖
//
//==============================================================================================
#include"leveldisplay.h"
#include"number.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CLevelDisplay::CLevelDisplay()
{
	m_nLevel = 0;
}

//==============================================================================================
// コンストラクタ
//==============================================================================================
CLevelDisplay::~CLevelDisplay()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CLevelDisplay::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < LEVEL_NUM; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create({ pos.x + (nCnt * m_fWidth), pos.y, pos.z }, m_fWidth, m_fHeight);
	}

	m_nLevel = 1;
	SetLevel(m_nLevel, 30, { 0.0f,0.0f,0.0f });

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CLevelDisplay::Uninit()
{
	for (int nCnt = 0; nCnt < LEVEL_NUM; nCnt++)
	{
		if (m_pNumber[nCnt] != nullptr)
		{
			m_pNumber[nCnt]->Uninit();
		}
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CLevelDisplay::Update()
{
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CLevelDisplay::Draw()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CLevelDisplay *CLevelDisplay::Create(D3DXVECTOR3 pos, float width, float height, D3DXCOLOR col)
{
	CLevelDisplay *pPlayerLevel = new CLevelDisplay;
	
	if (pPlayerLevel != nullptr)
	{
		pPlayerLevel->SetSize(width, height);
		pPlayerLevel->Init(pos);
		pPlayerLevel->SetCol(col);
	}

	return pPlayerLevel;
}

//==============================================================================================
// 加算処理
//==============================================================================================
void CLevelDisplay::AddLevel(int playerlevel, int maxlevel, D3DXVECTOR3 maxpos, EObjType type)
{
	m_nLevel += playerlevel;

	// スコアの設定
	SetLevel(m_nLevel, maxlevel, maxpos);

	if (type == OBJTYPE_PLAYER)
	{
		float SubCol = 1.0f / 15;

		B -= SubCol;

		if (B <= 0)
		{
			G -= SubCol;
		}
		if (G <= 0)
		{
			B -= SubCol;
		}

		SetCol({ R, G, B, 1.0f });
	}
}

//==============================================================================================
// サイズの設定
//==============================================================================================
void CLevelDisplay::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;
}

//==============================================================================================
// レベルの設定
//==============================================================================================
void CLevelDisplay::SetLevel(int statuslevel, int maxlevel, D3DXVECTOR3 maxpos)
{
	m_nLevel = statuslevel;

	int aPosTexU[LEVEL_NUM];		//各桁の数字を格納

	aPosTexU[0] = m_nLevel % 100 / 10;
	aPosTexU[1] = m_nLevel % 10 / 1;

	if (m_nLevel < maxlevel)
	{
		for (int nCnt = 0; nCnt < LEVEL_NUM; nCnt++)
		{
			m_pNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * aPosTexU[nCnt] + 0.1f, 0.1f * aPosTexU[nCnt]);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < LEVEL_NUM; nCnt++)
		{
			m_pTexture = CTexture::TEXTURE_LEVELMAX;

			// テクスチャの設定
			m_pNumber[nCnt]->Settexture(&m_pTexture);

			// テクスチャの位置の設定
			m_pNumber[nCnt]->SetTexPos(0.0f, 1.0f, 1.0f, 0.0f);

			// サイズの設定
			m_pNumber[nCnt]->SetSize(70.0f, 70.0f);

			m_pNumber[nCnt]->Setposition(maxpos);
		}

		m_pNumber[0]->Setcollar(&D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
	}
}

//==============================================================================================
// 色の設定
//==============================================================================================
void CLevelDisplay::SetCol(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < LEVEL_NUM; nCnt++)
	{
		m_pNumber[nCnt]->Setcollar(&col);
	}
}

//==============================================================================================
// 十の位のアルファ値の設定
//==============================================================================================
void CLevelDisplay::SetAlpha(float alpha)
{
	m_pNumber[0]->Setcollar(&D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha));
}