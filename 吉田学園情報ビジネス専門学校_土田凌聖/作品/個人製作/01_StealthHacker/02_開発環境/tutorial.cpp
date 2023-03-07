//==============================================================================================
//
// �`���[�g���A���@�@�@tutorial.cpp
// �y�c����
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
// �R���X�g���N�^
//==============================================================================================
CTutorial::CTutorial(int nPriority) : CObject(nPriority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTutorial::~CTutorial()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTutorial::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GAME);

	CObject2D *pObject2D = CObject2D::Create({ 1150, 50, 0.0f }, { 150.0f,25.0f,0.0f });
	pObject2D->SetTexture(CTexture::TEXTURE_SKIP);

	// ����
	m_pCamera = new CCamera;	// �J����
	m_pFile = new CFile;		// �t�@�C���ǂݍ���

#ifdef _DEBUG
	m_pSetObject = CSetObject::Create();
#endif // _DEBUG

	CMeshField::Create({ 0.0f, 0.0f,0.0f }, 20, 20, 150.0f);			// ���b�V���t�B�[���h
	m_pPlayer = *m_pFile->LoadCharFile(CFile::TXTNAME_PLAYER);			// �v���C���[�̓ǂݍ���
	m_pEnemy = m_pFile->LoadCharFile(CFile::TXTNAME_ENEMY_TUTORIAL);	// �G�̓ǂݍ���
	m_pObjectX = m_pFile->LoadModel();									// ���f���t�@�C���ǂݍ���
	m_PTutorial_Text = CTutorial_Text::Create();						// �`���[�g���A���e�L�X�g

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
void CTutorial::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_GAME);

	// �J�����̏I��
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
//�X�V����
//==============================================================================================
void CTutorial::Update()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// �J�����̍X�V
	m_pCamera->Update();

	// �`���[�g���A���e�L�X�g�̍X�V
	m_PTutorial_Text->Update();

#ifdef _DEBUG
	// �X�e�[�W�t�@�C���ǂݍ���
	m_pSetObject->Update();
#endif // _DEBUG

	if (pKeyboard->GetKeyboardTrigger(DIK_P) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_BACK))
	{
		CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
	}
}
//==============================================================================================
// �`�揈��
//==============================================================================================
void CTutorial::Draw()
{
	m_pCamera->SetCamera();
}

//==============================================================================================
// ��������
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