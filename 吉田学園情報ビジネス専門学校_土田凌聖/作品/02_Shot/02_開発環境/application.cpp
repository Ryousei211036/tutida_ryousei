//==============================================================================================
//
// �A�v���P�[�V����
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"application.h"
#include"inputkeyboard.h"
#include"title.h"
#include"game.h"
#include"result.h"
#include"sound.h"
#include"leveldisplay.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
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
// �R���X�g���N�^
//==============================================================================================
CApplication::CApplication()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CApplication::~CApplication()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �N���X�̐���
	m_pInputkeyboard = new CInputkeyboard;		// �L�[�{�[�h
	m_pRenderer = new CRenderer;				// �����_�����O
	m_pTexture = new CTexture;					// �e�N�X�`��
	
	// ����������
	m_pInputkeyboard->Init(hInstance, hWnd);	// �L�[�{�[�h
	m_pRenderer->Init(hWnd, false);				// �����_�����O
	InitSound(hWnd);

	m_mode = MODE_TITLE;

	// ���[�h�̐ݒ�
	SetMode(m_mode);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CApplication::Uninit()
{
	// �����_�����O�̏I��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// �L�[�{�[�h�̏I��
	if (m_pInputkeyboard != nullptr)
	{
		m_pInputkeyboard->Uninit();
		delete m_pInputkeyboard;
		m_pInputkeyboard = nullptr;
	}

	//�e�N�X�`���̏I��
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// �T�E���h�̏I��
	UninitSound();

	CObject::ReleaseAll();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CApplication::Update()
{
	m_pInputkeyboard->Update();

	m_pRenderer->Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}

//==============================================================================================
// ���[�h�̐ݒ�
//==============================================================================================
void CApplication::SetMode(MODE mode)
{
	if (m_pTitle != nullptr
		|| m_pGame != nullptr
		|| m_pResult != nullptr)
	{
		// ���݂̃��[�h�̔j��
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

		// �S�̂̉��
		CObject::ReleaseAll();
	}

	m_mode = mode;

	// �w��̃��[�h�̐ݒ�
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