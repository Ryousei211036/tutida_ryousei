//==============================================================================================
//
// �Q�[���@�@�@game.cpp
// �y�c����
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
// �R���X�g���N�^
//==============================================================================================
CGame::CGame(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CGame::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAME);
	CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GAME, 0.5f);

	// ����
	m_pCamera = new CCamera;	// �J����
	m_pFile = new CFile;		// �t�@�C���ǂݍ���

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 150.0f);	// ���b�V���t�B�[���h
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);	// �v���C���[�̓ǂݍ���
	m_pEnemy = m_pFile->LoadCharFile(CFile::TXTNAME_ENEMY);		// �G�̓ǂݍ���
	m_pObjectX = m_pFile->LoadModel();							// ���f���t�@�C���ǂݍ���

	// �X�e���V���o�b�t�@�p�̃|���S��
	m_pStencil = CStencilPolygon::Create({ 1280 / 2,720 / 2,0.0f }, { 1280.0f, 720.0f, 0.0f });

	// �X�R�A
	m_pScore = CScore::Create({ 1050.0f,680.0f,0.0f }, 20.0f, 20.0f, { 1.0f,1.0f,1.0f,1.0f });

	// �J�����̏�����
	m_pCamera->Init();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CGame::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_GAME);

	// �J�����̏I��
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
//�X�V����
//==============================================================================================
void CGame::Update()
{
	// �J�����̍X�V
	m_pCamera->Update();

#ifdef _DEBUG
	// �X�e�[�W�t�@�C���ǂݍ���
	m_pSetObject->Update();
#endif // _DEBUG

	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CGame::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// ��������
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