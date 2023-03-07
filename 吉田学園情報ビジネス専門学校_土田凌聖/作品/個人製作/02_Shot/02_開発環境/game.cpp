//==============================================================================================
//
// ゲーム画面　　　game.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include<time.h>
#include"game.h"
#include"bg.h"
#include"player.h"
#include"exp.h"
#include"enemy.h"
#include"inputkeyboard.h"
#include"score.h"
#include"fade.h"
#include"UI.h"
#include"leveldisplay.h"
#include"number.h"
#include"sound.h"

//**********************************************************************************************
// 静的メンバ変数宣言
//**********************************************************************************************
CBg *CGame::m_pBg = nullptr;
CPlayer *CGame::m_pPlayer = nullptr;
CEnemy *CGame::m_pEnemy = nullptr;
CExp *CGame::m_pExp = nullptr;
CScore *CGame::m_pScore = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGame::CGame(): CObject2D(4)
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_ExpSpawnPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 餌の生成する位置
	}
	m_nExpCollTime = 0;		// 餌生成のクールタイム
	m_nEnemyCollTime = 0;	// 敵生成のクールタイム
	m_nMaxNum = 0;			// 敵の最大数
	m_nCreateLevel = 0;		// 生成する敵のレベル
	m_nLevelCount = 0;	
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);
	//CEnemy::SetNumEnemy(0);

	//サウンドの再生
	PlaySound(SOUND_LABEL_GAME_BGM);

	// メンバ変数の初期化
	m_nMaxNum = MAX_ENEMY;		// 敵の最大数
	m_nCreateLevel = 1;

	// 生成するの位置
	m_fPlusPosX = SCREEN_WIDTH - SCREEN_WIDTH / 2 + PLAYER_STOPPOS;
	m_fPlusPosY = SCREEN_HEIGHT - SCREEN_HEIGHT / 2 + PLAYER_STOPPOS;
	m_fPlusInitPosX = SCREEN_WIDTH - SCREEN_WIDTH / 2 + PLAYER_STOPPOS;
	m_fPlusInitPosY = SCREEN_HEIGHT - SCREEN_HEIGHT / 2 + PLAYER_STOPPOS;
	m_fPlusZeroPosX = SCREEN_WIDTH / 2 - PLAYER_STOPPOS;
	m_fPlusZeroPosY = SCREEN_HEIGHT / 2 - PLAYER_STOPPOS;

	m_PlusPos = { m_fPlusPosX ,m_fPlusPosY ,0.0f };
	m_PlusInitPos = { m_fPlusInitPosX ,m_fPlusInitPosY ,0.0f };
	m_PlusZeroPos = { m_fPlusZeroPosX ,m_fPlusZeroPosY ,0.0f };

	// 生成処理
	m_pBg = CBg::Create();				// 背景
	m_pPlayer = CPlayer::Create();		// プレイヤー
	CUI::Create({ 1280 / 2, 720 / 2, 0.0f }, CTexture::TEXTURE_GAMEUI, 910, 510, 0, 0);				// UI
	m_pScore = CScore::Create({ 70.0f, 680, 0.0f }, 20.0f, 20.0f, { 1.0f,1.0f,0.0f,1.0f });		// スコア
	m_ELevel = CLevelDisplay::Create({ 1180, 65.0f, 0.0f }, 25.0f, 25.0f, { 1.0f, 0.0f, 0.0f, 1.0f });	// 敵のレベルの表示
	m_ELevel->SetAlpha(0.0f);

	// 敵の数のリセット
	CEnemy::SetNumEnemy(0);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CGame::Uninit()
{
	// サウンドの終了
	StopSound();

	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CGame::Update()
{
	CObject2D::Update();

	srand((unsigned int)time(0));

	MoveStop(m_PlusInitPos, &m_PlusPos);
	MoveStop({ 0.0f,0.0f,0.0f }, &m_PlusZeroPos);

	// 一定時間ごとに敵を生成
	CreateEnemy();

	// 一定時間ごとに餌を生成
	CreateExp();

#ifdef _DEBUG
	CFade::FADE FadeMode = CFade::GetFade();

	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN) == true && FadeMode == CFade::FADE_NONE)
	{
		CFade::SetFade(CApplication::MODE_RESULT, 0.02f);
	}
#endif // _DEBUG
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CGame::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 生成位置の移動
//==============================================================================================
void CGame::MoveStop(D3DXVECTOR3 InitPos, D3DXVECTOR3 *Pos)
{
	D3DXVECTOR3 move = CPlayer::Move();

	D3DXVECTOR3 StopX = CPlayer::StopX(InitPos);
	D3DXVECTOR3 StopY = CPlayer::StopY(InitPos);

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		Pos->x = StopX.x;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		Pos->y = StopY.y;
	}

	Pos->x += move.x;
	Pos->y += move.y;
}

//==============================================================================================
// 一定時間ごとに餌を生成する処理
//==============================================================================================
void CGame::CreateExp()
{
	int nNumExp = CExp::GetNumExp();

	D3DXVECTOR3 ExpCreatePos;

	ExpCreatePos.x = rand() % ((int)m_PlusPos.x - (int)m_PlusZeroPos.x) + m_PlusZeroPos.x;
	ExpCreatePos.y = rand() % ((int)m_PlusPos.y - (int)m_PlusZeroPos.y) + m_PlusZeroPos.y;

	int ExpType = rand() % 3;
	int ExpRot = rand() % 4;

	m_nExpCollTime--;

	if (m_nExpCollTime <= 0
		&& nNumExp < MAX_EXP)
	{
		m_pExp = CExp::Create(D3DXVECTOR3(ExpCreatePos.x, ExpCreatePos.y, 0.0f), ExpType, (float)ExpRot);
		m_nExpCollTime = EXP_CREATETIME;	// 餌生成のクールタイム
	}
}

//==============================================================================================
// 一定時間ごとに敵を生成する処理
//==============================================================================================
void CGame::CreateEnemy()
{
	// 敵の数の取得
	int nNumEnemy = CEnemy::GetNumEnemy();

	// 倒した数
	int nNumDeath = CEnemy::GetNumDeathEnemy();

	D3DXVECTOR3 EnemyCreatePos;

	EnemyCreatePos.x = rand() % ((int)m_PlusPos.x - (int)m_PlusZeroPos.x) + m_PlusZeroPos.x;
	EnemyCreatePos.y = rand() % ((int)m_PlusPos.y - (int)m_PlusZeroPos.y) + m_PlusZeroPos.y;

	m_nEnemyCollTime--;
	m_nLevelCount++;

	if (m_nCreateLevel < 10)
	{
		m_ELevel->SetAlpha(0.0f);
	}
	else
	{
		m_ELevel->SetCol({ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	if (m_nEnemyCollTime <= 0
		&& nNumEnemy < m_nMaxNum)
	{
		m_pEnemy = CEnemy::Create(D3DXVECTOR3(EnemyCreatePos.x, EnemyCreatePos.y, 0.0f), m_nCreateLevel);
		m_nCreateCount++;
		m_nEnemyCollTime = ENEMYCREATE_TIME;	// 敵生成のクールタイム

		if (m_nCreateCount % 10 == 0)
		{
			m_nMaxNum++;
		}
		if (nNumDeath >= 1)
		{
			if (nNumDeath % 1 == 0 && m_nCreateLevel < ENEMY_MAXLEVEL)
			{
				m_nCreateLevel++;

				m_ELevel->AddLevel(1, ENEMY_MAXLEVEL, { 1200.0f,60.0f, 0.0f }, OBJTYPE_ENEMY);
			}
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CGame *CGame::Create()
{
	CGame *pGame = nullptr;

	pGame = new CGame;

	if (pGame != nullptr)
	{
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pGame;
}