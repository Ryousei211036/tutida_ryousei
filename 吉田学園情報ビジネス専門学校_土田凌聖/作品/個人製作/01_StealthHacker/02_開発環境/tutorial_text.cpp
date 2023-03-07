//==============================================================================================
//
// チュートリアルテキスト　　　tuotrial_text.cpp
// 土田凌聖
//
//==============================================================================================
#include"tutorial_text.h"
#include"tutorial.h"
#include"charmanager.h"
#include"object2D.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"

bool CTutorial_Text::m_bDisplay_Tutorial = false;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTutorial_Text::CTutorial_Text()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTutorial_Text::~CTutorial_Text()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTutorial_Text::Init()
{
	m_nType = TYPE_NONE;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// チュートリアルの種類
		switch (nCnt)
		{
			// ドアの開け方
		case CTutorial_Text::TYPE_OPENDOOR:
			m_nTutorial[nCnt].Pos = { -30.0f, 0.0f, -600.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_DOOR;
			break;
			// 鍵のかかったドア
		case CTutorial_Text::TYPE_LOCKED_DOOR:
			m_nTutorial[nCnt].Pos = { -234.0f,  0.0f, -297.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_LOCKED_DOOR;
			break;
			// 鍵のかかったドア２
		case CTutorial_Text::TYPE_LOCKED_DOOR2:
			m_nTutorial[nCnt].Pos = { 33.0f, 0.0f, -144.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_LOCKED_DOOR;
			break;
			// 敵
		case CTutorial_Text::TYPE_FIND_ENEMY:
			m_nTutorial[nCnt].Pos = { 285.0f, 0.0f, -384.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_ENEMY;
			break;
			// ハッキング
		case CTutorial_Text::TYPE_HACKING:
			m_nTutorial[nCnt].Pos = { 290.0f, 0.0f, -384.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_HACKING;
			break;
			// 索敵
		case CTutorial_Text::TYPE_SCOUTING:
			m_nTutorial[nCnt].Pos = { -360.0f, 0.0f, -318.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_STEALTH;
			break;
			// 銃
		case CTutorial_Text::TYPE_GUN:
			m_nTutorial[nCnt].Pos = { -365.0f, 0.0f, -318.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_GUN;
			break;
			// ロボット
		case CTutorial_Text::TYPE_ROBOT:
			m_nTutorial[nCnt].Pos = { -405.0f, 0.0f, -471.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_ROBOT;
			break;
			// ボタン
		case CTutorial_Text::TYPE_BUTTON:
			m_nTutorial[nCnt].Pos = { 36.0f, 0.0f, 25.0f };
			m_nTutorial[nCnt].nRadius = 30.0f;
			m_nTutorial[nCnt].Texture = CTexture::TEXTURE_TUTORIAL_BUTTON;
			break;
		default:
			break;
		}
	}

	// 表示する位置
	TexturePos = { 640.0f,360.0f,0.0f };
	m_bDisplay_Tutorial = false;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		pObject2D[nCnt] = nullptr;
		m_nTutorial[nCnt].bDisplay = false;
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (pObject2D[nCnt] == nullptr)
		{
			pObject2D[nCnt] = CObject2D::Create(TexturePos, { 0.0f,0.0f,0.0f });	// 生成
			pObject2D[nCnt]->SetTexture(m_nTutorial[nCnt].Texture);					// テクスチャの設定
			pObject2D[nCnt]->SetSize(0.0f, 0.0f);									// サイズ(最初は0)
		}
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTutorial_Text::Uninit()
{
	m_bDisplay_Tutorial = false;
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTutorial_Text::Update()
{
	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	CCharManager *pPlayer = CTutorial::GetPlayer();
	D3DXVECTOR3 PPos = { 0.0f,0.0f,0.0f };
	float Radius = 0.0f;

	if (pPlayer != nullptr)
	{
		PPos = pPlayer->GetPos();
		Radius = pPlayer->GetRadius();
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		float Distance = sqrtf((PPos.x - m_nTutorial[nCnt].Pos.x) * (PPos.x - m_nTutorial[nCnt].Pos.x)
			+ (PPos.z - m_nTutorial[nCnt].Pos.z) * (PPos.z - m_nTutorial[nCnt].Pos.z));

		if (Radius + m_nTutorial[nCnt].nRadius > Distance
			&& !m_nTutorial[nCnt].bDisplay)
		{
			m_bDisplay_Tutorial = true;
			pObject2D[nCnt]->SetSize(650.0f, 420.0f);
		}

		if (m_bDisplay_Tutorial)
		{
			// キーボードの情報
			CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

			if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
			{
				pObject2D[nCnt]->SetSize(0.0f, 0.0f);
				m_bDisplay_Tutorial = false;

				if (nCnt == TYPE_LOCKED_DOOR
					|| nCnt == TYPE_LOCKED_DOOR2)
				{
					m_nTutorial[TYPE_LOCKED_DOOR].bDisplay = true;
					m_nTutorial[TYPE_LOCKED_DOOR2].bDisplay = true;
				}
				else
				{
					m_nTutorial[nCnt].bDisplay = true;
				}
			}
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTutorial_Text::Draw()
{

}

//==============================================================================================
// 生成処理
//==============================================================================================
CTutorial_Text *CTutorial_Text::Create()
{
	CTutorial_Text *pTutorial_Text = new CTutorial_Text;

	if (pTutorial_Text != nullptr)
	{
		pTutorial_Text->Init();
	}

	return pTutorial_Text;
}