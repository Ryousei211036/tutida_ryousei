//==============================================================================================
//
// アプリケーション
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"application.h"
#include"inputkeyboard.h"
#include"title.h"
#include"game.h"
#include"result.h"
#include"sound.h"
#include"leveldisplay.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CTitle *CApplication::m_pTitle = nullptr;
CGame *CApplication::m_pGame = nullptr;
CResult *CApplication::m_pResult = nullptr;
CRenderer *CApplication::m_pRenderer = nullptr;
CInputkeyboard *CApplication::m_pInputkeyboard = nullptr;
CTexture *CApplication::m_pTexture = nullptr;
CSound *CApplication::m_pSound = nullptr;
CApplication::MODE CApplication::m_mode = MODE_NONE;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CApplication::CApplication()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CApplication::~CApplication()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// クラスの生成
	m_pInputkeyboard = new CInputkeyboard;		// キーボード
	m_pRenderer = new CRenderer;				// レンダリング
	m_pTexture = new CTexture;					// テクスチャ
	
	// 初期化処理
	m_pInputkeyboard->Init(hInstance, hWnd);	// キーボード
	m_pRenderer->Init(hWnd, false);				// レンダリング
	InitSound(hWnd);

	m_mode = MODE_TITLE;

	// モードの設定
	SetMode(m_mode);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CApplication::Uninit()
{
	// レンダリングの終了
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// キーボードの終了
	if (m_pInputkeyboard != nullptr)
	{
		m_pInputkeyboard->Uninit();
		delete m_pInputkeyboard;
		m_pInputkeyboard = nullptr;
	}

	//テクスチャの終了
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// サウンドの終了
	UninitSound();

	CObject::ReleaseAll();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CApplication::Update()
{
	m_pInputkeyboard->Update();

	m_pRenderer->Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}

//==============================================================================================
// モードの設定
//==============================================================================================
void CApplication::SetMode(MODE mode)
{
	if (m_pTitle != nullptr
		|| m_pGame != nullptr
		|| m_pResult != nullptr)
	{
		// 現在のモードの破棄
		switch (m_mode)
		{
		case CApplication::MODE_TITLE:
			m_pTitle->Uninit();
			break;

		case CApplication::MODE_GAME:
			m_pGame->Uninit();
			break;

		case CApplication::MODE_RESULT:
			m_pResult->Uninit();
			break;

		default:
			break;
		}

		// 全体の解放
		CObject::ReleaseAll();
	}

	m_mode = mode;

	// 指定のモードの設定
	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		m_pTitle = CTitle::Create();
		break;

	case CApplication::MODE_GAME:
		m_pGame = CGame::Create();
		break;

	case CApplication::MODE_RESULT:
		m_pResult = CResult::Create();
		break;

	default:
		break;
	}
}