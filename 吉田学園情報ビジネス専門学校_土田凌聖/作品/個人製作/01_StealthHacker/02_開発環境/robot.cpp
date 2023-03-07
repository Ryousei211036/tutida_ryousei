//==============================================================================================
//
// ���{�b�g�@�@�@robot.cpp
// �y�c����
//
//==============================================================================================
#include"robot.h"
#include"game.h"
#include"camera.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"
#include"objectX.h"
#include"gage.h"
#include"tutorial.h"
#include"tutorial_text.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CRobot::CRobot()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CRobot::~CRobot()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CRobot::Init()
{
	CCharManager::Init();
	
	m_fSpeed = 2;				// �ړ����x
	m_fRotSpeed = 0.1f;			// ��]���x
	m_nConChange_CoolTime = 60;	// ����ύX�̃N�[���^�C��
	m_fRadius = 30.0f;			// �����蔻��̔��a
	m_nLife = 900;				// �̗�
	m_bDeath = false;
	m_bGageFull_Up = false;
	m_bResetGage = false;

	// �e�N���X�ɓn��
	CCharManager::SetRadius(m_fRadius);		// ���a
	CCharManager::SetLife(m_nLife);			// �̗�

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CRobot::Uninit()
{
	CCharManager::Uninit();
	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CRobot::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCharManager::Update();

		// �̗͂̎擾
		m_nLife = GetLife();

		// �^�C�v�̐ݒ�
		SetMotion(m_Motion);

		m_Motion = CCharManager::MOTION_NEUTRAL;	// �^�C�v�̐ݒ�

		Control_Type ContType = CCharManager::GetControlType();

		// ���{�b�g���쒆
		if (ContType == CONTROL_ROBOT && !m_bDeath)
		{
			// �ړ�����
			Move();

			// �A�N�V����
			Action();
		}

		// �{�^������������
		m_bPushButton = GetPushButton();

		if (m_bPushButton)
		{
			// �{�^��������
			m_Motion = MOTION_BUTTON;
		}

		Death();

		if (m_bGageFull_Up)
		{
			m_pGage->SetAlpha(0.1f);

			m_nLife = 60;
			SetLife(m_nLife);
			m_bResetGage = false;
			m_bDeath = false;
		}

		// �I�u�W�F�N�gX�̏��
		CObjectX **pObjectX = nullptr;

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pObjectX = CGame::GetObjectX();
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pObjectX = CTutorial::GetObjectX();
		}

		int nNumObject = (*pObjectX)->GetNumObject();

		for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
		{
			if (*pObjectX != nullptr)
			{
				// �����蔻��
				(*pObjectX)->CharCollision(CModel::MODEL_TYPE_ROBOT);
			}
		}
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CRobot::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CRobot *CRobot::Create(D3DXVECTOR3 pos)
{
	CRobot *pRobot = nullptr;

	pRobot = new CRobot;

	if (pRobot != nullptr)
	{
		pRobot->CCharManager::SetPos(pos);
	}

	return pRobot;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CRobot::Move()
{
	// �J�����̏��
	CCamera *pCamera = nullptr;
	// �J�����̊p�x
	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();
		CRot = pCamera->GetRot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();
		CRot = pCamera->GetRot();
	}

	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// �ʒu�̎擾
	m_Pos = CCharManager::GetPos();

	// �O��̈ʒu�̐ݒ�
	m_PosOld = m_Pos;

	// �ړI�̊p�x�̐ݒ�
	m_RotDest = CCharManager::GetRotDest();

	// �J�������S�[���̂ق��������Ă��邩
	bool bGoal = pCamera->GoalOpen();

	if (!bGoal)
	{
		// �v���C���[�̈ړ�
		if (pKeyboard->GetKeyboardPress(DIK_W) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y <= -0.3f)
		{
			if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
			{// ���O
				m_RotDest.y = CRot.y - D3DX_PI * 1 / 4;
				m_Pos.x -= sinf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
			{// �E�O
				m_RotDest.y = CRot.y + D3DX_PI * 1 / 4;
				m_Pos.x += sinf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
				m_Pos.z += cosf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
			}
			else
			{// �O
				m_RotDest.y = CRot.y;
				m_Pos.x += sinf(CRot.y) * m_fSpeed;
				m_Pos.z += cosf(CRot.y) * m_fSpeed;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_S) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y >= 0.3f)
		{
			if (pKeyboard->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
			{// ����
				m_RotDest.y = CRot.y - D3DX_PI * 3 / 4;
				m_Pos.x -= sinf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
			{// �E��
				m_RotDest.y = CRot.y + D3DX_PI * 3 / 4;
				m_Pos.x += sinf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
				m_Pos.z += cosf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
			}
			else
			{// ��
				m_RotDest.y = CRot.y + D3DX_PI;
				m_Pos.x -= sinf(CRot.y) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y) * m_fSpeed;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
		{// ��
			m_RotDest.y = CRot.y - D3DX_PI * 1 / 2;
			m_Pos.x -= sinf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
			m_Pos.z -= cosf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
		{// �E
			m_RotDest.y = CRot.y + D3DX_PI * 1 / 2;
			m_Pos.x += sinf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
			m_Pos.z += cosf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
		}

		if (pKeyboard->GetKeyboardPress(DIK_W)
			|| pKeyboard->GetKeyboardPress(DIK_A)
			|| pKeyboard->GetKeyboardPress(DIK_S)
			|| pKeyboard->GetKeyboardPress(DIK_D)
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y >= 0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y <= -0.3f)
		{
			// ����
			m_Motion = CCharManager::MOTION_WALK;

			// �{�^���������Ȃ�
			SetPushButton(false);
		}
	}

	// �p�x�̐��K��
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	CCharManager::SetPos(m_Pos);
	CCharManager::SetPosOld(m_PosOld);
	CCharManager::SetRotDest(m_RotDest);
}

//==============================================================================================
// �A�N�V����
//==============================================================================================
void CRobot::Action()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �v���C���[����ɐ؂�ւ���
	ChangeControl(CONTROL_PLAYER);
}

//==============================================================================================
// ���S
//==============================================================================================
void CRobot::Death()
{
	if (m_nLife <= 0)
	{
		m_Motion = CCharManager::MOTION_DEATH;
		m_bDeath = true;
		m_bGageFull_Up = false;

		if (!m_bResetGage && m_pGage != nullptr)
		{
			float fLength = m_pGage->GetEnd_Lenght();
			m_pGage->SubGage(fLength * 3);
			m_pGage->SetResetAlpha(1.0f);
			m_pGage->SetResetCount(30);
			m_bResetGage = true;
		}

		// �L�[�{�[�h�̏��
		CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

		// �v���C���[�̏��
		CCharManager *pPlayer = nullptr;

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pPlayer = CGame::GetPlayer();
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pPlayer = CTutorial::GetPlayer();
		}

		D3DXVECTOR3 PPos = { 1230.0f, 0.0f, 855.0f };
		float fPRadius = 0.0f;

		if (pPlayer != nullptr)
		{
			PPos = pPlayer->GetPos();
			fPRadius = pPlayer->GetRadius();
		}

		// �v���C���[�ƃ��{�b�g�̋���
		float Distance = CObject::Distance(m_Pos, PPos);

		if (m_pGage != nullptr)
		{
			if (m_fRadius + fPRadius > Distance)
			{
				// �������߂��Ƃ��̓Q�[�W��\������
				m_pGage->Display(true);

				if (pKeyboard->GetKeyboardPress(DIK_E))
				{
					// �Q�[�W�𑝂₷
					m_bGageFull_Up = m_pGage->AddGage(1.1f);
				}
			}
			else
			{
				m_pGage->Display(false);
			}
		}

		// ���[�V�����ύX
		ChangeMotion(m_Motion);

		// ���[�V�������I�������
		if (!PlayMotion(CCharManager::MOTION_DEATH))
		{
			// �����؂�ւ���
			CCharManager::SetControl(CONTROL_PLAYER);

			// ���{�b�g�̏ꏊ���ڂ�
			m_Pos = { 1220.0f,0.0f,840.0f };

			SetPos(m_Pos);
			SetPosOld(m_Pos);

			// �Q�[�W�̐���
			if (m_pGage == nullptr)
			{
				m_pGage = CGage::Create({ m_Pos.x ,m_Pos.y + 60.0f,m_Pos.z }, { 35.0f, 3.0f,0.0f }, { 0.0f,1.0f,0.0f,1.0f }, true);
			}

			m_pGage->SetPos({ m_Pos.x,m_Pos.y + 60.0f,m_Pos.z });
		}
		else
		{
			if (m_pGage != nullptr)
			{
				m_pGage->Display(false);
			}
		}
	}
}

//==============================================================================================
// ���[�V�������̏���
//==============================================================================================
void CRobot::Process_Motion()
{

}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CRobot::SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// ���[�V�����̐ݒ�
//==============================================================================================
void CRobot::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// ���[�V�����̕ύX
//==============================================================================================
void CRobot::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel *CRobot::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	char *Xfilename, CModel::EModel_Type type)
{
	CModel *pModel = CCharManager::SetModel(index, parent, pos, rot, Xfilename, type);

	// �T�C�Y�̎擾
	D3DXVECTOR3 fMax = pModel->GetMaxSize();
	D3DXVECTOR3 fMin = pModel->GetMinSize();

	// �e�̈ʒu
	D3DXVECTOR3 ParentPos = { 0.0f,0.0f,0.0f };

	// �e������ꍇ
	if (parent >= 0)
	{
		ParentPos = pModel->GetParent().GetPos();
	}

	// �I�t�Z�b�g�Ɛe�̈ʒu�𔽉f
	// �ő�l
	fMax.x += pModel->GetPos().x + ParentPos.x;
	fMax.y += pModel->GetPos().y + ParentPos.y;
	fMax.z += pModel->GetPos().z + ParentPos.z;

	// �ŏ��l
	fMin.x += pModel->GetPos().x + ParentPos.x;
	fMin.y += pModel->GetPos().y + ParentPos.y;
	fMin.z += pModel->GetPos().z + ParentPos.z;

	// �ő�T�C�Y�̐ݒ�
	if (m_SizeMax.x < fMax.x)
	{
		m_SizeMax.x = fMax.x;	// X
	}
	if (m_SizeMax.y < fMax.y)
	{
		m_SizeMax.y = fMax.y;	// Y
	}
	if (m_SizeMax.z < fMax.z)
	{
		m_SizeMax.z = fMax.z;	// Z
	}

	// �ŏ��T�C�Y�̐ݒ�
	if (m_SizeMin.x > fMin.x)
	{
		m_SizeMin.x = fMin.x;	// X
	}
	if (m_SizeMin.y > fMin.y)
	{
		m_SizeMin.y = fMin.y;	// Y
	}
	if (m_SizeMin.z > fMin.z)
	{
		m_SizeMin.z = fMin.z;	// Z
	}

	CCharManager::SetSizeMax(m_SizeMax);
	CCharManager::SetSizeMin(m_SizeMin);

	return nullptr;
}