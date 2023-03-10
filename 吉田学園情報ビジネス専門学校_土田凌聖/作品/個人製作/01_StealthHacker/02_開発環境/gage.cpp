//==============================================================================================
//
// ゲージ　　　gage.cpp
// 土田凌聖
//
//==============================================================================================
#include"gage.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGage::CGage(int nPriority) : CObject3D(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGage::~CGage()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGage::Init()
{
	CObject3D::Init();

	m_fAlpha = 1.0f;		// アルファ値
	m_nAlphaCount = 30;		// 消えるまでのカウント
	m_bDisplay = false;		// 表示するか

	// ゲージの長さの設定
	SetSizeX(m_fLength);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CGage::Uninit()
{
	CObject3D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CGage::Update()
{
	CObject3D::Update();

	pObject3D->SetPos(m_Pos);
	CObject3D::SetPos(m_Pos);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CGage::Draw()
{
	CObject3D::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CGage *CGage::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool billboard)
{
	CGage *pHackGage = nullptr;

	pHackGage = new CGage;

	if (pHackGage != nullptr)
	{
		pHackGage->SetPos(pos);
		pHackGage->SetLength(-size);
		pHackGage->SetEnd_Length(size);
		pHackGage->SetSize(size);
		pHackGage->SetCol(col);
		pHackGage->Setbillboard(billboard);
		pHackGage->SetObject3D(pos,size,billboard);
		pHackGage->Init();
	}

	return pHackGage;
}
//==============================================================================================
// Object3Dの設定
//==============================================================================================
void CGage::SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool billboard)
{
	pObject3D = CObject3D::Create(pos, size, { 0.0f,0.0f,0.0f,1.0f }, billboard);
}

//==============================================================================================
// ゲージを増やす
//==============================================================================================
bool CGage::AddGage(float add)
{
	if (m_fLength <= m_fEnd_Length)
	{
		// ゲージを伸ばす
		m_fLength += add;

		SetSizeX(m_fLength);
	}
	else
	{
		return true;
	}

	return false;
}

//==============================================================================================
// ゲージを減らす
//==============================================================================================
void CGage::SubGage(float sub)
{
	if (m_fLength >= -m_fEnd_Length)
	{
		// ゲージを伸ばす
		m_fLength -= sub;

		if (m_fLength <= -m_fEnd_Length)
		{
			m_fLength = -m_fEnd_Length;
		}

		SetSizeX(m_fLength);
	}
}

//==============================================================================================
// アルファ値の設定
//==============================================================================================
bool CGage::SetAlpha(float alpha)
{
	m_nAlphaCount--;

	if (m_nAlphaCount <= 0)
	{
		// アルファ値を下げる
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= alpha;
		}

		// 色の設定
		SetCol({ 0.0f, 1.0f, 0.0, m_fAlpha });

		if (pObject3D != nullptr)
		{
			pObject3D->SetCol({ 0.0f,0.0f,0.0f,m_fAlpha });
		}
		if (m_fAlpha < 0)
		{
			return true;
		}
	}

	return false;
}

//==============================================================================================
// ゲージの表示
//==============================================================================================
void CGage::Display(bool display)
{
	m_bDisplay = display;

	if (m_bDisplay)
	{
		SetCol({ 0.0f,1.0f,0.0f,1.0f });

		if (pObject3D != nullptr)
		{
			pObject3D->SetCol({ 0.0f,0.0f,0.0f,1.0f });
		}
	}
	else
	{
		SetCol({ 0.0f,1.0f,0.0f,0.0f });

		if (pObject3D != nullptr)
		{
			pObject3D->SetCol({ 0.0f,0.0f,0.0f,0.0f });
		}
	}
}