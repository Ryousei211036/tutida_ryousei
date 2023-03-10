//==============================================================================================
//
// 数字　　　number.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"number.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CNumber::CNumber() : CObject2D(4)
{}

//==============================================================================================
// デスラクタ
//==============================================================================================
CNumber::~CNumber()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CNumber::Init()
{
	CObject2D::Init();

	SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CNumber::Update()
{
	CObject2D::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CNumber::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float width, float height)
{
	CNumber *pNumber = nullptr;

	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		pNumber->Setposition(pos);
		pNumber->Init();
		pNumber->SetSize(width, height);
	}

	return pNumber;
}

//==============================================================================================
// テクスチャを変える
//==============================================================================================
void CNumber::Settexture(CTexture::TEXTURE *pTexture)
{
	m_pTexture = pTexture;
	SetTexture(*m_pTexture);
}

//==============================================================================================
// 色の設定
//==============================================================================================
void CNumber::Setcollar(D3DXCOLOR *pCol)
{
	SetCol(*pCol);
}
//
////==============================================================================================
//// サイズの設定
////==============================================================================================
//void CNumber::SetSize(float width, float height)
//{
//	CObject2D::SetSize(width, height);
//}