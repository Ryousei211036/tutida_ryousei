//==============================================================================================
//
// �^�C�g���@�@�@title.cpp
// �y�c����
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
// �R���X�g���N�^
//==============================================================================================
CTitle::CTitle(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTitle::~CTitle()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTitle::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_TITLE);

	// ����
	m_pCamera = new CCamera;	// �J����
	CFile *m_pFile = new CFile;		// �t�@�C���ǂݍ���

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	// ���b�V���t�B�[���h
	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 100.0f);
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);	// �v���C���[�̓ǂݍ���
	m_pObjectX = m_pFile->LoadModel();							// ���f���t�@�C���ǂݍ���

	CTitle_Logo::Create();

	// �J�����̏�����
	m_pCamera->Init();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTitle::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_TITLE);

	// �J�����̏I��
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	Release();
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CTitle::Update()
{
	// �J�����̍X�V
	m_pCamera->Update();

#ifdef _DEBUG
	// �X�e�[�W�t�@�C���ǂݍ���
	m_pSetObject->Update();
#endif // _DEBUG
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CTitle::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// ��������
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