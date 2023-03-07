//==============================================================================================
//
// �Q�[����ʁ@�@�@game.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
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
// �ÓI�����o�ϐ��錾
//**********************************************************************************************
CBg *CGame::m_pBg = nullptr;
CPlayer *CGame::m_pPlayer = nullptr;
CEnemy *CGame::m_pEnemy = nullptr;
CExp *CGame::m_pExp = nullptr;
CScore *CGame::m_pScore = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CGame::CGame(): CObject2D(4)
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_ExpSpawnPos[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �a�̐�������ʒu
	}
	m_nExpCollTime = 0;		// �a�����̃N�[���^�C��
	m_nEnemyCollTime = 0;	// �G�����̃N�[���^�C��
	m_nMaxNum = 0;			// �G�̍ő吔
	m_nCreateLevel = 0;		// ��������G�̃��x��
	m_nLevelCount = 0;	
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
HRESULT CGame::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);
	//CEnemy::SetNumEnemy(0);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_GAME_BGM);

	// �����o�ϐ��̏�����
	m_nMaxNum = MAX_ENEMY;		// �G�̍ő吔
	m_nCreateLevel = 1;

	// ��������̈ʒu
	m_fPlusPosX = SCREEN_WIDTH - SCREEN_WIDTH / 2 + PLAYER_STOPPOS;
	m_fPlusPosY = SCREEN_HEIGHT - SCREEN_HEIGHT / 2 + PLAYER_STOPPOS;
	m_fPlusInitPosX = SCREEN_WIDTH - SCREEN_WIDTH / 2 + PLAYER_STOPPOS;
	m_fPlusInitPosY = SCREEN_HEIGHT - SCREEN_HEIGHT / 2 + PLAYER_STOPPOS;
	m_fPlusZeroPosX = SCREEN_WIDTH / 2 - PLAYER_STOPPOS;
	m_fPlusZeroPosY = SCREEN_HEIGHT / 2 - PLAYER_STOPPOS;

	m_PlusPos = { m_fPlusPosX ,m_fPlusPosY ,0.0f };
	m_PlusInitPos = { m_fPlusInitPosX ,m_fPlusInitPosY ,0.0f };
	m_PlusZeroPos = { m_fPlusZeroPosX ,m_fPlusZeroPosY ,0.0f };

	// ��������
	m_pBg = CBg::Create();				// �w�i
	m_pPlayer = CPlayer::Create();		// �v���C���[
	CUI::Create({ 1280 / 2, 720 / 2, 0.0f }, CTexture::TEXTURE_GAMEUI, 910, 510, 0, 0);				// UI
	m_pScore = CScore::Create({ 70.0f, 680, 0.0f }, 20.0f, 20.0f, { 1.0f,1.0f,0.0f,1.0f });		// �X�R�A
	m_ELevel = CLevelDisplay::Create({ 1180, 65.0f, 0.0f }, 25.0f, 25.0f, { 1.0f, 0.0f, 0.0f, 1.0f });	// �G�̃��x���̕\��
	m_ELevel->SetAlpha(0.0f);

	// �G�̐��̃��Z�b�g
	CEnemy::SetNumEnemy(0);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CGame::Uninit()
{
	// �T�E���h�̏I��
	StopSound();

	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CGame::Update()
{
	CObject2D::Update();

	srand((unsigned int)time(0));

	MoveStop(m_PlusInitPos, &m_PlusPos);
	MoveStop({ 0.0f,0.0f,0.0f }, &m_PlusZeroPos);

	// ��莞�Ԃ��ƂɓG�𐶐�
	CreateEnemy();

	// ��莞�Ԃ��Ƃɉa�𐶐�
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
// �`�揈��
//==============================================================================================
void CGame::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// �����ʒu�̈ړ�
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
// ��莞�Ԃ��Ƃɉa�𐶐����鏈��
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
		m_nExpCollTime = EXP_CREATETIME;	// �a�����̃N�[���^�C��
	}
}

//==============================================================================================
// ��莞�Ԃ��ƂɓG�𐶐����鏈��
//==============================================================================================
void CGame::CreateEnemy()
{
	// �G�̐��̎擾
	int nNumEnemy = CEnemy::GetNumEnemy();

	// �|������
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
		m_nEnemyCollTime = ENEMYCREATE_TIME;	// �G�����̃N�[���^�C��

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
// ��������
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