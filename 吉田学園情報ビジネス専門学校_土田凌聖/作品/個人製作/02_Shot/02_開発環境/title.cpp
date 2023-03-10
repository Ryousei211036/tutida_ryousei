//==============================================================================================
//
// タイトル画面　　　title.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"title.h"
#include"inputkeyboard.h"
#include"fade.h"
#include"UI.h"
#include"sound.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTitle::CTitle() : CObject2D(4)
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
HRESULT CTitle::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);
	
	//サウンドの再生
	PlaySound(SOUND_LABEL_TITLE_BGM);

	m_Pos = pos;				// 初期位置
	m_fWidth = TITLE_WIDTH;		// 幅
	m_fHeight = TITLE_HEIGHT;	// 高さ

	CObject2D::SetSize(m_fWidth, m_fHeight);		// サイズと幅の設定

	CObject2D::SetTexture(CTexture::TEXTURE_TITLE);	//テクスチャの設定
	CUI::Create({ 860.0f, 560.0f, 0.0f }, CTexture::TEXTURE_PRESSENTER, 400, 400, 30, 3);		// UI

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTitle::Uninit()
{
	// サウンドの終了
	StopSound();

	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTitle::Update()
{
	CObject2D::Update();

	CFade::FADE FadeMode = CFade::GetFade();

	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN) == true && FadeMode == CFade::FADE_NONE)
	{
		CFade::SetFade(CApplication::MODE_GAME, 0.04f);

		//サウンドの再生
		PlaySound(SOUND_LABEL_START_SE);
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CTitle::Draw()
{
	CObject2D::Draw();
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
		pTitle->Init(D3DXVECTOR3(1280 / 2, 720 / 2, 0.0f));
	}

	return pTitle;
}