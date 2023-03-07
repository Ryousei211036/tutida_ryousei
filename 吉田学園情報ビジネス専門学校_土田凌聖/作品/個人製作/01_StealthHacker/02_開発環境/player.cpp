//==============================================================================================
//
// �v���C���[�@�@�@player.cpp
// �y�c����
//
//==============================================================================================
#include"player.h"
#include"application.h"
#include"inputkeyboard.h"
#include"inputJoypad.h"
#include"camera.h"
#include"debugproc.h"
#include"model.h"
#include"game.h"
#include"file.h"
#include"sound.h"
#include"stencilpolygon.h"
#include"objectX.h"
#include"bullet.h"
#include"fade.h"
#include"score.h"
#include"enemy.h"
#include"tutorial.h"
#include"tutorial_text.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CPlayer::CPlayer()
{
	m_Pos = { 0.0f,0.0f,0.0f };
	m_Rot = { 0.0f,0.0f,0.0f };
	m_RotDest = { 0.0f,0.0f,0.0f };
	m_SizeMax = { -1000.0f,-1000.0f,-1000.0f };
	m_SizeMin = { 1000.0f, 1000.0f, 1000.0f };
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CPlayer::~CPlayer()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CPlayer::Init()
{	
	CCharManager::Init();

	m_fRadius = 30.0f;		// �����蔻��̔��a
	m_fCameraDis = 80.0f;	// �J�����Ƃ̋���
	m_nLife = 90;			// �̗�
	m_Motion = CCharManager::MOTION_NEUTRAL;	// ���[�V����
	m_bFind = false;
	m_bPlay_Shot = false;
	m_bPlay_StealthKill = false;
	m_bDeath = false;
	m_bPushButton = false;

	m_Pos = CCharManager::GetPos();
	m_InitPos = m_Pos;

	CCharManager::SetRadius(m_fRadius);
	CCharManager::SetLife(m_nLife);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CPlayer::Uninit()
{
	CCharManager::Uninit();

	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CPlayer::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCamera *pCamera = nullptr;

		// �I�u�W�F�N�gX�̏��
		CObjectX **pObjectX = nullptr;

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pCamera = CGame::GetCamera();
			pObjectX = CGame::GetObjectX();
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pCamera = CTutorial::GetCamera();
			pObjectX = CTutorial::GetObjectX();
		}

		if (CApplication::GetMode() == CApplication::MODE_GAME
			|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// ����̃^�C�v
			Control_Type ContType = GetControlType();

			// �̗͂̎擾
			m_nLife = GetLife();

			// �X�e���X�L������
			m_bPlay_StealthKill = GetPlayStealthKill();

			bool bGoal = pCamera->GoalOpen();

			if (m_bPlay_StealthKill)
			{
				m_Motion = CCharManager::MOTION_KILL;
			}

			// �v���C���[���쒆
			if (ContType == CONTROL_PLAYER && !m_bDeath)
			{
				if (!m_bPlay_StealthKill && !bGoal)
				{
					// �A�N�V����
					Action();
				}

				// �ړ�����
				Move();

				// ������炷
				Sound_Footstep();
			}

			// �{�^������������
			m_bPushButton = GetPushButton();

			if (m_bPushButton)
			{
				// �{�^��������
				m_Motion = MOTION_BUTTON;
			}

			// ���S
			Death();
		}

		// �^�C�v�̐ݒ�
		SetMotion(m_Motion);

		// �e�̍X�V����
		CCharManager::Update();

		if (CApplication::GetMode() == CApplication::MODE_GAME
			|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// ��]���x
			m_fRotSpeed = 0.1f;

			// ���[�V�������̍Đ����̏���
			Process_Motion();

			// �I�u�W�F�N�g�̐�
			int nNumObject = (*pObjectX)->GetNumObject();

			for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
			{
				if (*pObjectX != nullptr)
				{
					// �I�u�W�F�N�g�̓����蔻��
					(*pObjectX)->CharCollision(CModel::MODEL_TYPE_PLAYER);
				}
			}
		}
	}
	else
	{
		CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_WARK);

		m_bFootstep = false;
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CPlayer::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->CCharManager::SetPos(pos);
	}

	return pPlayer;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CPlayer::Move()
{
	CCamera *pCamera = nullptr;
	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();

		// �J�����̊p�x
		CRot = pCamera->GetRot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();

		// �J�����̊p�x
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

	// �X�e���X�L�����[�V����������Ȃ���
	if (!m_bPlay_StealthKill && !bGoal)
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
			// �ړ����x(����)
			m_fSpeed = 3;

			// ����
			m_Motion = CCharManager::MOTION_WALK;

			// �e�����낷
			m_bHoldGun = false;

			// �X�e���V���o�b�t�@�p�̃|���S���𓧖��ɂ���
			m_bFind = false;

			// �X�e���V���|���S���̏��
			CStencilPolygon *pStencil = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pStencil = CGame::GetStencil();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pStencil = CTutorial::GetStencil();
			}

			if (pStencil != nullptr)
			{
				pStencil->SetUse(false);
			}

			// �{�^���������Ȃ�
			SetPushButton(false);
		}
	}

	// �p�x�̐��K��
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	// �e�ɏ���n��
	CCharManager::SetPos(m_Pos);		// �ʒu
	CCharManager::SetPosOld(m_PosOld);	// �O��̈ʒu
}

//==============================================================================================
// �v���C���[�̃A�N�V����
//==============================================================================================
void CPlayer::Action()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// �X�e���V���|���S���̏��
	CStencilPolygon *pStencil = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pStencil = CGame::GetStencil();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pStencil = CTutorial::GetStencil();
	}

	// �e���\����
	if ((pKeyboard->GetKeyboardTrigger(DIK_UP) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_LEFT_BUTTON))
		&& !m_bHoldGun && !m_bPushButton)
	{
		m_bHoldGun = true;
	}
	//// �e�����낷
	//else if ((pKeyboard->GetKeyboardTrigger(DIK_UP) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_LEFT_BUTTON)) && m_bHoldGun)
	//{
	//	m_bHoldGun = false;
	//}

	// �e������
	if ((pKeyboard->GetKeyboardTrigger(DIK_SPACE) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_RIGHT_BUTTON)) && m_bHoldGun)
	{
		m_Motion = CCharManager::MOTION_SHOT;

		if (!m_bPlay_Shot)
		{
			D3DXVECTOR3 m_GunPos = m_Pos;
			m_GunPos.y = 80.0f;

			m_GunPos.x += sinf(m_Rot.y) * 50;
			m_GunPos.z += cosf(m_Rot.y) * 50;

			m_GunPos.x += sinf(m_Rot.y + D3DX_PI / 2) * 10.0f;
			m_GunPos.z += cosf(m_Rot.y + D3DX_PI / 2) * 10.0f;

			// �e������
			CBullet::Create(m_GunPos, m_Rot, { 0.2f,0.2f,0.2f }, "Data/XFILE/Player/01_head.x", CObjectX::OBJECTX_BULLET, CModel::MODEL_TYPE_PLAYER);
		}
	}

	// ���G����
	if ((pKeyboard->GetKeyboardTrigger(DIK_Q) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_Y))
		&& !m_bFind && pStencil != nullptr && !m_bPushButton && !m_bHoldGun)
	{
		m_bFind = true;				// ���G����
		m_Motion = CCharManager::MOTION_FIND;
		pStencil->SetUse(true);		// �X�e���V���|���S���̎g�p
	}
	else if ((pKeyboard->GetKeyboardTrigger(DIK_Q) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_Y))
		&& m_bFind && pStencil != nullptr)
	{
		m_bFind = false;			// ���G����
		pStencil->SetUse(false);	// �X�e���V���|���S���̎g�p
	}

	if (m_Motion == MOTION_NEUTRAL || m_Motion == MOTION_BUTTON)
	{
		// ���{�b�g����ɐ؂�ւ���
		ChangeControl(CONTROL_ROBOT);
	}
}

//==============================================================================================
// ���S
//==============================================================================================
void CPlayer::Death()
{
	// �̗͂�0�ɂȂ���
	if (m_nLife <= 0)
	{
		m_Motion = CCharManager::MOTION_DEATH;
		m_bDeath = true;
		m_bFind = false;
		m_bHoldGun = false;

		// ���[�V�����ύX
		ChangeMotion(m_Motion);

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			// ���[�V�������I�������
			if (!PlayMotion(CCharManager::MOTION_DEATH))
			{
				CFade::SetFade(CApplication::MODE_RESULT, 0.01f);
			}
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// ���[�V�������I�������
			if (!PlayMotion(CCharManager::MOTION_DEATH))
			{
				CCharManager::SetPos(m_InitPos);
				SetPosOld(m_InitPos);

				m_RotDest = { 0.0f,0.0f,0.0f };
				SetRotDest(m_RotDest);

				m_nLife = 120;
				SetLife(m_nLife);

				m_bDeath = false;
				m_Motion = CCharManager::MOTION_NEUTRAL;

				// ���[�V�����ύX
				ChangeMotion(m_Motion);
			}
		}
	}
}

//==============================================================================================
// ���[�V�������̏���
//==============================================================================================
void CPlayer::Process_Motion()
{
	// �J�����̏��
	CCamera *pCamera = nullptr;
	D3DXVECTOR3 CPosV = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();
		CPosV = pCamera->GetPosV();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();
		CPosV = pCamera->GetPosV();
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		if (m_CPosV.z == 0.0f)
		{
			m_CPosV.z = CPosV.z;
		}

		// �ˌ����[�V�������Đ�����
		m_bPlay_Shot = PlayMotion(CCharManager::MOTION_SHOT);

		// �j���[�g������Ԃɂ���
		if (!m_bHoldGun
			&& !m_bPlay_StealthKill
			&& !m_bFind)
		{// �e���\���ĂȂ��Ƃ��A���G���Ă��Ȃ��Ƃ�
			m_Motion = CCharManager::MOTION_NEUTRAL;	// �^�C�v�̐ݒ�

			// �J��������������
			CPosV.z += ((m_CPosV.z) - CPosV.z) * 0.1f;
			pCamera->SetPosV(CPosV);
		}

		if (m_bHoldGun && !m_bPlay_Shot)
		{// �e���\���Ă�Ƃ�
			m_Motion = CCharManager::MOTION_GUN;		// �^�C�v�̐ݒ�
			m_RotDest = pCamera->GetRot();	// ���ʂ�����
			m_fRotSpeed = 0.5f;				// ��]���x

			// �J�������߂Â���
			CPosV.z += ((m_CPosV.z + m_fCameraDis) - CPosV.z) * 0.5f;
			pCamera->SetPosV(CPosV);
		}
		else if (m_bPlay_Shot)
		{// �ˌ����[�V������
			m_RotDest = pCamera->GetRot();	// ���ʂ�����
			m_fRotSpeed = 0.5f;				// ��]���x
		}

		CCharManager::SetRotDest(m_RotDest);// �ړI�̊p�x
	}
}

//==============================================================================================
// ���[�V�����̒l�̐ݒ�
//==============================================================================================
void CPlayer::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// ���[�V�����̐ݒ�
//==============================================================================================
void CPlayer::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// ���[�V�����̐؂�ւ�
//==============================================================================================
void CPlayer::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel *CPlayer::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
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

//==============================================================================================
// ������炷
//==============================================================================================
void CPlayer::Sound_Footstep()
{
	CSound *pSound = CApplication::GetSound();

	if (m_Motion == CCharManager::MOTION_WALK && !m_bFootstep)
	{// �������
		pSound->PlaySound(CSound::SOUND_LABEL_WARK);
		m_bFootstep = true;
	}
	else if(m_Motion != CCharManager::MOTION_WALK && m_bFootstep)
	{// ����������
		pSound->StopSound(CSound::SOUND_LABEL_WARK);

		m_bFootstep = false;
	}
}