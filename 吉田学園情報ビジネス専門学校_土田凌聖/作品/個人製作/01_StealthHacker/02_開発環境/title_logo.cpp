//==============================================================================================
//
// �^�C�g�����S�@�@�@title_logo.cpp
// �y�c����
//
//==============================================================================================
#include"title_logo.h"
#include"inputkeyboard.h"
#include"inputJoypad.h"
#include"application.h"
#include"fade.h"
#include"sound.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CTitle_Logo::CTitle_Logo(int nPriority) : CObject2D(nPriority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTitle_Logo::~CTitle_Logo()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTitle_Logo::Init()
{
	CObject2D::Init();

	m_nSelect = SELECT_START;

	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		if (m_pObject2D[nCnt] == nullptr)
		{
			m_pObject2D[nCnt] = CObject2D::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		}
	}

	m_pObject2D[SELECT_START]->SetTexture(CTexture::TEXTURE_START);
	m_pObject2D[SELECT_START]->SetSize(300.0f, 100.0f);
	m_pObject2D[SELECT_START]->Setposition({ 760.0f,450.0f,0.0f });

	m_pObject2D[SELECT_TUTORIAL]->SetTexture(CTexture::TEXTURE_TUTORIAL);
	m_pObject2D[SELECT_TUTORIAL]->SetSize(500.0f, 100.0f);
	m_pObject2D[SELECT_TUTORIAL]->Setposition({ 900.0f,600.0f,0.0f });

	CObject2D *pTitle = CObject2D::Create({ 830.0f,180.0f,0.0f }, { 550.0f,200.0f,0.0f });
	pTitle->SetTexture(CTexture::TEXTURE_TITLELOGO);
	
	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTitle_Logo::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CTitle_Logo::Update()
{
	CObject2D::Update();
	
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		// �A���t�@�l��������
		m_pObject2D[nCnt]->SetCol({ 1.0f,1.0f,1.0f,0.3f });
	}

	// �I������
	CTitle_Logo::TitleSelect();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CTitle_Logo::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CTitle_Logo *CTitle_Logo::Create()
{
	CTitle_Logo *pTitle_Logo = nullptr;

	pTitle_Logo = new CTitle_Logo;

	if (pTitle_Logo != nullptr)
	{
		pTitle_Logo->Init();
	}

	return pTitle_Logo;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTitle_Logo::TitleSelect()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	if ((pKeyboard->GetKeyboardTrigger(DIK_S) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_DOWN) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y >= 0.3f)
		&& m_nSelect < SELECT_MAX - 1)
	{
		m_nSelect++;
		CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SELECT);
	}
	if ((pKeyboard->GetKeyboardTrigger(DIK_W) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_UP) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y <= -0.3f)
		&& m_nSelect > SELECT_START)
	{
		m_nSelect--;
		CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SELECT);
	}

	switch (m_nSelect)
	{
	case CTitle_Logo::SELECT_START:
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_A))
		{
			CFade::SetFade(CApplication::MODE_GAME, 0.05f);
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_START);
		}
		break;
	case CTitle_Logo::SELECT_TUTORIAL:
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_A))
		{
			CFade::SetFade(CApplication::MODE_TUTORIAL, 0.05f);
			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_START);
		}
		break;
	default:
		break;
	}

	// �I�𒆂̃|���S���̃A���t�@�l��������
	if (m_pObject2D[m_nSelect] != nullptr)
	{
		m_pObject2D[m_nSelect]->SetCol({ 1.0f,1.0f,1.0f,1.0f });
	}
}