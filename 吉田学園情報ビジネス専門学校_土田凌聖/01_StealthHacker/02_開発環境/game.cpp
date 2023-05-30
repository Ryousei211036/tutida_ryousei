//==============================================================================================
//
// ゲーム　　　game.cpp
// 土田凌聖
//
//==============================================================================================
#include"game.h"
#include"inputkeyboard.h"
#include"application.h"
#include"fade.h"
#include"player.h"
#include"meshfield.h"
#include"camera.h"
#include"objectX.h"
#include"file.h"
#include"object3D.h"
#include"enemy.h"
#include"charmanager.h"
#include"stencilpolygon.h"
#include"setobject.h"
#include"score.h"
#include"sound.h"

CCharManager *CGame::m_pPlayer = nullptr;
CCharManager *CGame::m_pRobot = nullptr;
CCharManager **CGame::m_pEnemy = nullptr;
CObjectX **CGame::m_pObjectX = nullptr;
CCamera *CGame::m_pCamera = nullptr;
CFile *CGame::m_pFile = nullptr;
CStencilPolygon *CGame::m_pStencil = nullptr;
CSetObject *CGame::m_pSetObject = nullptr;
CScore *CGame::m_pScore = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGame::CGame(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGame::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAME);
	CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GAME, 0.5f);

	// 生成
	m_pCamera = new CCamera;	// カメラ
	m_pFile = new CFile;		// ファイル読み込み

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 150.0f);	// メッシュフィールド
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);	// プレイヤーの読み込み
	m_pEnemy = m_pFile->LoadCharFile(CFile::TXTNAME_ENEMY);		// 敵の読み込み
	m_pObjectX = m_pFile->LoadModel();							// モデルファイル読み込み

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
void CGame::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_GAME);

	// カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	m_pRobot = nullptr;

	Release();
}

//==============================================================================================
//更新処理
//==============================================================================================
void CGame::Update()
{
	// カメラの更新
	m_pCamera->Update();

#ifdef _DEBUG
	// ステージファイル読み込み
	m_pSetObject->Update();
#endif // _DEBUG

	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CGame::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CGame *CGame::Create()
{
	CGame *pGame = nullptr;

	pGame = new CGame;

	if (pGame != nullptr)
	{
		pGame->Init();
	}

	return pGame;
}