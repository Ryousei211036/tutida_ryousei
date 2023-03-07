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
// �ÓI�����o�ϐ��̐錾
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
	m_pInputJoypad = new CInputJoypad;			// �W���C�p�b�h
	m_pRenderer = new CRenderer;				// �����_�����O
	m_pTexture = new CTexture;					// �e�N�X�`��
	m_pLight = new CLight;						// ���C�g
	m_pDebugProc = new CDebugProc;				// �f�o�b�O�\��
	m_pSound = new CSound;						// �T�E���h

	// ����������
	m_pInputkeyboard->Init(hInstance, hWnd);	// �L�[�{�[�h
	m_pInputJoypad->Init(hInstance, hWnd);		// �W���C�p�b�h
	m_pRenderer->Init(hWnd, TRUE);				// �����_�����O
	m_pLight->Init();							// ���C�g
	m_pDebugProc->Init();						// �f�o�b�O�\��
	m_pSound->Init(hWnd);						// �T�E���h

	// �Q�[�����[�h
	m_mode = MODE_TITLE;

	// ���[�h�̒��
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

	// �W���C�p�b�h�̏I��
	if (m_pInputJoypad != nullptr)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = nullptr;
	}

	//�e�N�X�`���̏I��
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ���C�g�̏I��
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// �f�o�b�O�\���̏I��
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	// �T�E���h�̏I��
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	CObject::ReleaseAll();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CApplication::Update()
{
	// �L�[�{�[�h�̍X�V
	m_pInputkeyboard->Update();

	// �W���C�p�b�h�̍X�V
	m_pInputJoypad->Update();

	m_pRenderer->Update();

	//if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F) == true)
	//{
	//	if (!m_bWireFrame)
	//	{
	//		//���C���[�t���[�����[�h�̐ݒ�
	//		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//		m_bWireFrame = true;
	//	}
	//	else
	//	{
	//		//���C���[�t���[�����[�h�̐ݒ�
	//		m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//		m_bWireFrame = false;
	//	}
	//}
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
	if (m_pGameMode != nullptr)
	{
		m_pGameMode->Uninit();
		m_pGameMode = nullptr;

		CObject::ReleaseAll();
	}

	m_mode = mode;

	// �w��̃��[�h�̐ݒ�
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