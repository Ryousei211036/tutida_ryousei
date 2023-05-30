//==============================================================================================
//
// タイトルロゴ　　　title_logo.cpp
// 土田凌聖
//
//==============================================================================================
#include"title_logo.h"
#include"inputkeyboard.h"
#include"inputJoypad.h"
#include"application.h"
#include"fade.h"
#include"sound.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTitle_Logo::CTitle_Logo(int nPriority) : CObject2D(nPriority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTitle_Logo::~CTitle_Logo()
{
}

//==============================================================================================
// 初期化処理
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
// 終了処理
//==============================================================================================
void CTitle_Logo::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTitle_Logo::Update()
{
	CObject2D::Update();
	
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		// アルファ値を下げる
		m_pObject2D[nCnt]->SetCol({ 1.0f,1.0f,1.0f,0.3f });
	}

	// 選択処理
	CTitle_Logo::TitleSelect();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTitle_Logo::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 生成処理
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
// 選択処理
//==============================================================================================
void CTitle_Logo::TitleSelect()
{
	// キーボードの情報
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

	// 選択中のポリゴンのアルファ値をあげる
	if (m_pObject2D[m_nSelect] != nullptr)
	{
		m_pObject2D[m_nSelect]->SetCol({ 1.0f,1.0f,1.0f,1.0f });
	}
}