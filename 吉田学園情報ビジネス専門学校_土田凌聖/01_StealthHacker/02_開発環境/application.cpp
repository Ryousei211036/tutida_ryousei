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
#include"inputjoypad.h"
#include"object.h"
#include"Object3D.h"
#include"Object2D.h"
#include"camera.h"
#include"light.h"
#include"player.h"
#include"debugproc.h"
#include"meshfield.h"
#include"title.h"
#include"game.h"
#include"result.h"
#include"sound.h"
#include"tutorial.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CTitle *CApplication::m_pTitle = nullptr;
CGame *CApplication::m_pGame = nullptr;
CResult *CApplication::m_pResult = nullptr;
CRenderer *CApplication::m_pRenderer = nullptr;
CInputkeyboard *CApplication::m_pInputkeyboard = nullptr;
CInputJoypad *CApplication::m_pInputJoypad = nullptr;
CTexture *CApplication::m_pTexture = nullptr;
CSound *CApplication::m_pSound = nullptr;
CLight *CApplication::m_pLight = nullptr;
CApplication::MODE CApplication::m_mode = MODE_NONE;
CObject *CApplication::m_pGameMode = nullptr;

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
	m_pInputJoypad = new CInputJoypad;			// ジョイパッド
	m_pRenderer = new CRenderer;				// レンダリング
	m_pTexture = new CTexture;					// テクスチャ
	m_pLight = new CLight;						// ライト
	m_pDebugProc = new CDebugProc;				// デバッグ表示
	m_pSound = new CSound;						// サウンド

	// 初期化処理
	m_pInputkeyboard->Init(hInstance, hWnd);	// キーボード
	m_pInputJoypad->Init(hInstance, hWnd);		// ジョイパッド
	m_pRenderer->Init(hWnd, TRUE);				// レンダリング
	m_pLight->Init();							// ライト
	m_pDebugProc->Init();						// デバッグ表示
	m_pSound->Init(hWnd);						// サウンド

	// ゲームモード
	m_mode = MODE_TITLE;

	// モードの定設
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

	// ジョイパッドの終了
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	//テクスチャの終了
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ライトの終了
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// デバッグ表示の終了
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	// サウンドの終了
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	CObject::ReleaseAll();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CApplication::Update()
{
	// キーボードの更新
	m_pInputkeyboard->Update();

	// ジョイパッドの更新
	m_pInputJoypad->Update();

	m_pRenderer->Update();

	//if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F) == true)
	//{
	//	if (!m_bWireFrame)
	//	{
	//		//ワイヤーフレームモードの設定
	//		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//		m_bWireFrame = true;
	//	}
	//	else
	//	{
	//		//ワイヤーフレームモードの設定
	//		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//		m_bWireFrame = false;
	//	}
	//}
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
	if (m_pGameMode != nullptr)
	{
		m_pGameMode->Uninit();
		m_pGameMode = nullptr;

		CObject::ReleaseAll();
	}

	m_mode = mode;

	// 指定のモードの設定
	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		m_pGameMode = CTitle::Create();
		break;

	case CApplication::MODE_GAME:
		m_pGameMode = CGame::Create();
		break;

	case CApplication::MODE_RESULT:
		m_pGameMode = CResult::Create();
		break;

	case CApplication::MODE_TUTORIAL:
		m_pGameMode = CTutorial::Create();
		break;

	default:
		break;
	}
}