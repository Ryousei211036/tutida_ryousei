//==============================================================================================
//
// フェード　　　fade.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"fade.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CFade *CFade::m_pFade = nullptr;
bool CFade::m_bCreate = false;
float CFade::m_fFadeSpeed = 0.0f;
CApplication::MODE CFade::NextMode = CApplication::MODE_NONE;
CFade::FADE CFade::FadeMode = FADE_NONE;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CFade::CFade(int nPriority) : CObject2D(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CFade::~CFade()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CFade::Init()
{
	CObject2D::Init();

	m_pFade = nullptr;
	m_fWidth = 910.0f;		// 幅
	m_fHeight = 510.0f;		// 高さ
	m_fAlpha = 0.0f;		// アルファ値

	// サイズと幅の設定
	CObject2D::SetSize(m_fWidth, m_fHeight);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CFade::Uninit()
{
	CObject2D::Uninit();

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CFade::Update()
{
	CObject2D::Update();

	// 色の設定
	CObject2D::SetCol({ 0.0f,0.0f,0.0f,m_fAlpha });

	if (FadeMode != FADE_NONE)
	{
		if (FadeMode == FADE_IN)
		{
			// ポリゴンを透明にしていく
			m_fAlpha -= m_fFadeSpeed;

			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;

				//何もしてない状態にする
				FadeMode = FADE_NONE;

				m_pFade = nullptr;

				m_bCreate = false;

				// 終了処理
				Uninit();
			}
		}
		else if (FadeMode == FADE_OUT)
		{
			//ポリゴンを不透明にしていく
			m_fAlpha += m_fFadeSpeed;

			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;

				// 画面遷移
				CApplication::SetMode(NextMode);

				//フェードイン状態にする
				FadeMode = FADE_IN;
			}
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CFade::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CFade *CFade::Create()
{
	CFade *pFade = nullptr;

	pFade = new CFade;

	if (pFade != nullptr)
	{
		pFade->Init();
		pFade->Setposition(D3DXVECTOR3(1280 / 2, 720 / 2, 0.0f));
		pFade->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	}

	return pFade;
}

//==============================================================================================
// フェード処理
//==============================================================================================
void CFade::SetFade(CApplication::MODE mode, float fadespeed)
{
	// フェード用ポリゴンの生成
	if (m_pFade == nullptr)
	{
		m_pFade = Create();
		m_bCreate = true;

		m_fFadeSpeed = fadespeed;

		// 次のモードの設定
		NextMode = mode;

		// フェードアウト状態にする
		FadeMode = FADE_OUT;
	}
}