//==============================================================================================
//
// ���U���g�@�@�@result.cpp
// �y�c����
//
//==============================================================================================
#include"result.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"
#include"fade.h"
#include"ranking.h"
#include"game.h"
#include"score.h"
#include"sound.h"

int CResult::m_nSaveScore = 0;
CRanking *CResult::m_pRanking = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CResult::CResult(int nPriority) : CObject(nPriority)
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
HRESULT CResult::Init()
{
	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_RESULT);

	CObject2D *pObject2D = CObject2D::Create({ 1280 / 2, 720 / 2, 0.0f }, { 910.0f, 510.0f, 0.0f });
	pObject2D->SetTexture(CTexture::TEXTURE_RESULT);

	m_pRanking = CRanking::Create({ 1280 / 2 - 200.0f, 720 / 2 - 100.0f, 0.0f }, 50.0f, 50.0f, { 1.0f, 1.0f, 1.0f, 1.0f });

	// �ŏI�X�R�A���擾
	CGame::GetScore()->SaveScore();

	// �X�R�A�̕ۑ�
	m_pRanking->CompareScore(m_nSaveScore);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CResult::Uninit()
{
	CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_RESULT);

	Release();
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CResult::Update()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
	{
		CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
	}
}
//==============================================================================================
// �`�揈��
//==============================================================================================
void CResult::Draw()
{

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
		pResult->Init();
	}

	return pResult;
}