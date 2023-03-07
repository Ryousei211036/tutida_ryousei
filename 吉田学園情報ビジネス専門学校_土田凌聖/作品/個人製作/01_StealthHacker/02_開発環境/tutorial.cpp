//==============================================================================================
//
// チュートリアル　　　tutorial.cpp
// 土田凌聖
//
//==============================================================================================
#include"tutorial.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"fade.h"
#include"camera.h"
#include"meshfield.h"
#include"charmanager.h"
#include"file.h"
#include"setobject.h"
#include"stencilpolygon.h"
#include"score.h"
#include"tutorial_text.h"
#include"sound.h"

CCharManager *CTutorial::m_pPlayer = nullptr;
CCharManager **CTutorial::m_pEnemy = nullptr;
CCharManager *CTutorial::m_pRobot = nullptr;
CCamera *CTutorial::m_pCamera = nullptr;
CFile *CTutorial::m_pFile = nullptr;
CObjectX **CTutorial::m_pObjectX = nullptr;
CSetObject *CTutorial::m_pSetObject = nullptr;
CStencilPolygon *CTutorial::m_pStencil = nullptr;
CScore *CTutorial::m_pScore = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTutorial::CTutorial(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTutorial::~CTutorial()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTutorial::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAME);

	CObject2D *pObject2D = CObject2D::Create({ 1150, 50, 0.0f }, { 150.0f,25.0f,0.0f });
	pObject2D->SetTexture(CTexture::TEXTURE_SKIP);

	// 生成
	m_pCamera = new CCamera;	// カメラ
	m_pFile = new CFile;		// ファイル読み込み

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 150.0f);			// メッシュフィールド
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);			// プレイヤーの読み込み
	m_pEnemy = m_pFile->LoadCharFile(CFile::TXTNAME_ENEMY_TUTORIAL);	// 敵の読み込み
	m_pObjectX = m_pFile->LoadModel();									// モデルファイル読み込み
	m_PTutorial_Text = CTutorial_Text::Create();						// チュートリアルテキスト

	// ステンシルバッファ用のポリゴン
	m_pStencil = CStencilPolygon::Create({ 1280 / 2,720 / 2,0.0f }, { 1280.0f, 720.0f, 0.0f });

	// スコア
	m_pScore = CScore::Create({ 1050.0f,680.0f,0.0f }, 20.0f, 20.0f, { 1.0f,1.0f,1.0f,1.0f });

	// カメラの初期化
	m_pCamera->Init();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTutorial::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_GAME);

	// カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	m_PTutorial_Text->Uninit();

	m_pRobot = nullptr;

	Release();
}

//==============================================================================================
//更新処理
//==============================================================================================
void CTutorial::Update()
{
	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// カメラの更新
	m_pCamera->Update();

	// チュートリアルテキストの更新
	m_PTutorial_Text->Update();

#ifdef _DEBUG
	// ステージファイル読み込み
	m_pSetObject->Update();
#endif // _DEBUG

	if (pKeyboard->GetKeyboardTrigger(DIK_P) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_BACK))
	{
		CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
	}
}
//==============================================================================================
// 描画処理
//==============================================================================================
void CTutorial::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = nullptr;

	pTutorial = new CTutorial;

	if (pTutorial != nullptr)
	{
		pTutorial->Init();
	}

	return pTutorial;
}