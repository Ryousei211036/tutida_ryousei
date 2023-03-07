//==============================================================================================
//
// ���U���g��ʁ@�@�@result.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include<time.h>
#include"result.h"
#include"UI.h"
#include"bg.h"
#include"enemy.h"
#include"exp.h"
#include"inputkeyboard.h"
#include"fade.h"
#include"ranking.h"
#include"sound.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
CExp *CResult::m_pExp = nullptr;
CRanking *CResult::m_pRanking = nullptr;
CEnemy *CResult::m_pEnemy = nullptr;
int CResult::m_nSaveScore = 0;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CResult::CResult() : CObject2D(4)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CResult::~CResult()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_RESULT_BGM);

	m_Pos = pos;				// �����ʒu
	m_fWidth = RESULT_WIDTH;	// ��
	m_fHeight = RESULT_HEIGHT;	// ����

	CObject2D::SetSize(m_fWidth, m_fHeight);			// �T�C�Y�ƕ��̐ݒ�

	srand((unsigned int)time(0));

	int nEPosX = rand() % 1280;
	int nEPosY = rand() % 720;

	CObject2D::SetCol({ 0.0f, 0.0f, 0.0f, 0.5f });
	CBg::Create();
	m_pEnemy = CEnemy::Create({ (float)nEPosX , (float)nEPosY ,0.0f }, 11);
	m_pRanking = CRanking::Create({ 1280 / 2 - 120.0f, 720 / 2 - 100.0f, 0.0f }, 30.0f, 30.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	CUI::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200.0f, 0.0f }, CTexture::TEXTURE_RANKING, 150.0f, 150.0f, 0, 0);

	// �X�R�A�̕ۑ�
	m_pRanking->CompareScore(m_nSaveScore);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CResult::Uninit()
{
	// �T�E���h�̏I��
	StopSound();

	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CResult::Update()
{
	CObject2D::Update();

	CFade::FADE FadeMode = CFade::GetFade();
	CreateExp();

	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN) == true && FadeMode == CFade::FADE_NONE)
	{
		CFade::SetFade(CApplication::MODE_TITLE, 0.04f);
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CResult::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CResult *CResult::Create()
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init(D3DXVECTOR3(1280 / 2, 720 / 2, 0.0f));
	}

	return pResult;
}

//==============================================================================================
// �a�̐���
//==============================================================================================
void CResult::CreateExp()
{
	int nNumExp = CExp::GetNumExp();

	int nExpCreatePosX = rand() % 1280;
	int nExpCreatePosY = rand() % 720;
	int ExpType = rand() % 3;
	int ExpRot = rand() % 4;

	m_nExpCollTime--;

	if (m_nExpCollTime <= 0
		&& nNumExp < 10)
	{
		m_pExp = CExp::Create(D3DXVECTOR3((float)nExpCreatePosX, (float)nExpCreatePosY, 0.0f), ExpType, (float)ExpRot);
		m_nExpCollTime = EXP_CREATETIME;	// �a�����̃N�[���^�C��
	}
}