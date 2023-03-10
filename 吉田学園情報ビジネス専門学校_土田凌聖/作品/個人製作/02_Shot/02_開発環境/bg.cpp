//==============================================================================================
//
// 背景　　　bg.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"bg.h"
#include"player.h"
#include"application.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CBg::CBg() : CObject2D(0)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_fWidth = 0.0f;							// 幅
	m_fHeight = 0.0f;							// 高さ
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CBg::~CBg()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	m_Pos = pos;				// 位置
	m_InitPos = pos;			// 初期位置
	m_fWidth = BG_WIDTH;		// 幅
	m_fHeight = BG_HEIGHT;		// 高さ
												
	CObject2D::SetSize(m_fWidth,  m_fHeight);		// サイズと幅の設定

	CObject2D::SetTexture(CTexture::TEXTURE_BG);	//テクスチャの設定

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CBg::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CBg::Update()
{
	CObject2D::Update();

	CApplication::MODE GameMode = CApplication::GetMode();

	if (GameMode == CApplication::MODE_GAME)
	{
		D3DXVECTOR3 StopX = CPlayer::StopX(m_InitPos);
		D3DXVECTOR3 StopY = CPlayer::StopY(m_InitPos);

		if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			m_Pos.x = StopX.x;
		}
		if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			m_Pos.y = StopY.y;
		}

		m_Move = CPlayer::Move();

		m_Pos += m_Move;

		CObject2D::Setposition(m_Pos);
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CBg::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 背景の生成
//==============================================================================================
CBg *CBg::Create()
{
	CBg *pBg = nullptr;

	pBg = new CBg;

	if (pBg != nullptr)
	{
		pBg->Init(D3DXVECTOR3(1280 / 2, 720 / 2, 0.0f));
	}

	return pBg;
}