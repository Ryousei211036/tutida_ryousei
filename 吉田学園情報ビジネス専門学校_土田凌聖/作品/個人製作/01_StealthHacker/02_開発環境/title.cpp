//==============================================================================================
//
// タイトル　　　title.cpp
// 土田凌聖
//
//==============================================================================================
#include"title.h"
#include"inputkeyboard.h"
#include"application.h"
#include"fade.h"
#include"camera.h"
#include"meshfield.h"
#include"file.h"
#include"setobject.h"
#include"objectX.h"
#include"title_logo.h"
#include"sound.h"

CCamera *CTitle::m_pCamera = nullptr;
CCharManager *CTitle::m_pPlayer = nullptr;
CObjectX **CTitle::m_pObjectX = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTitle::CTitle(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTitle::~CTitle()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTitle::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_TITLE);

	// 生成
	m_pCamera = new CCamera;	// カメラ
	CFile *m_pFile = new CFile;		// ファイル読み込み

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	// メッシュフィールド
	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 100.0f);
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);	// プレイヤーの読み込み
	m_pObjectX = m_pFile->LoadModel();							// モデルファイル読み込み

	CTitle_Logo::Create();

	// カメラの初期化
	m_pCamera->Init();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTitle::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_TITLE);

	// カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	Release();
}

//==============================================================================================
//更新処理
//==============================================================================================
void CTitle::Update()
{
	// カメラの更新
	m_pCamera->Update();

#ifdef _DEBUG
	// ステージファイル読み込み
	m_pSetObject->Update();
#endif // _DEBUG
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTitle::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = nullptr;

	pTitle = new CTitle;

	if (pTitle != nullptr)
	{
		pTitle->Init();
	}

	return pTitle;
}