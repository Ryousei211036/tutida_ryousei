//==============================================
//
//�J�����̏���[camera.cpp]
//Author�F�y�c����
//
//==============================================
#include"camera.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"
#include"player.h"
#include"game.h"
#include"setobject.h"
#include"objectX.h"
#include"title.h"
#include"tutorial.h"
#include"tutorial_text.h"

#include"debugproc.h"

//==============================================
//�R���X�g���N�^
//==============================================
CCamera::CCamera()
{

}

//==============================================
//�f�X�g���N�^
//==============================================
CCamera::~CCamera()
{

}

//==============================================
//�J�����̏���������
//==============================================
void CCamera::Init(void)
{
	//���_�E�����_�E�������ݒ肷��
	m_posV = D3DXVECTOR3(40.0f, 100.0f, -200.0f);		// ���_(�I�t�Z�b�g)
	m_InitposV = D3DXVECTOR3(40.0f, 100.0f, -200.0f);	// ���_(�I�t�Z�b�g)
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);			// �����_(�I�t�Z�b�g)
	m_InitposR = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);		// �����_(�I�t�Z�b�g)
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �p�x
	m_fRotSpeed = 0.03f;							// ��]���x
	m_nShiftGoalCount = 250;
	m_nGoalCount = 100;
	m_fPlayerDistance = 0.0f;

	CCharManager *pPlayer = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
	}
	if (pPlayer != nullptr)
	{
		m_CPos = pPlayer->GetPos();			// �J�����̏����ʒu(�v���C���[�̈ʒu)
	}

	if (CApplication::GetMode() == CApplication::MODE_TITLE)
	{
		m_CPos = { -30.0f,-20.0f,-100.0f };
		m_rot.y = D3DX_PI;
	}
}

//==============================================
//�J�����̏I������
//==============================================
void CCamera::Uninit(void)
{

}

//==============================================
//�J�����̍X�V����
//==============================================
void CCamera::Update(void)
{
	// �s����g�����J��������
	Matrix();

	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if ((CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		&& !Tutorial_Display)
	{
		// �J�����̈ړ�
		Move();

		// ����̃^�C�v
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (type != CCharManager::CONTROL_CAMERA
			&& !m_bGoalOpen)
		{
			// �J�����̓����蔻��
			Collision();
		}

		// �S�[�����J�������̃J����
		GoalCamera();
	}
}

//==============================================
// �J�����̃Z�b�g����
//==============================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X�̐���
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_WorldCameraPosV,
		&m_WorldCameraPosR,
		&m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̐���
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)1280 / (float)720,
		10.0f,
		10000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==============================================
// �s����g�����J��������
//==============================================
void CCamera::Matrix()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	// ���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	CCharManager *pPlayer = nullptr;
	CCharManager *pRobot = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pRobot = CGame::GetRobot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pRobot = CTutorial::GetRobot();
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		// ����̃^�C�v
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (!m_bGoalOpen)
		{
			if (pPlayer != nullptr)
			{
				m_PPos = pPlayer->GetPos();	// �v���C���[�̈ʒu
			}
			if (pRobot != nullptr)
			{
				m_RPos = pRobot->GetPos();	// ���{�b�g�̈ʒu
			}

			if (type == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[�̈ʒu�Ɉړ�
				m_CPos += (m_PPos - m_CPos) * 0.1f;
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ���{�b�g�̈ʒu�Ɉړ�
				m_CPos += (m_RPos - m_CPos) * 0.1f;
				m_CPos.y += (-80.0f - m_CPos.y) * 0.1f;
			}
		}
	}

	// �s�����]�ɔ��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �s��Ɉړ��𔽉f(�v���C���[�̈ʒu)
	D3DXMatrixTranslation(&mtxTrans, m_CPos.x, m_CPos.y, m_CPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&m_WorldCameraPosV, &m_posV, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_WorldCameraPosR, &m_posR, &m_mtxWorld);

	// ���_����v���C���[�܂ł̋���
	if (m_fPlayerDistance == 0.0f)
	{
		m_fPlayerDistance = sqrtf((m_CPos.x - m_WorldCameraPosV.x) * (m_CPos.x - m_WorldCameraPosV.x)
			+ (m_CPos.z - m_WorldCameraPosV.z) * (m_CPos.z - m_WorldCameraPosV.z));
	}
}

//==============================================
// �J�����̈ړ�
//==============================================
void CCamera::Move()
{
	// ����̃^�C�v
	CCharManager::Control_Type type = CCharManager::GetControlType();

	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	if (!m_bGoalOpen)
	{
		// �J�����̈ړ�
		if (pKeyboard->GetKeyboardPress(DIK_RIGHT) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_RIGHT_STICK).x >= 0.3f)
		{
			m_rot.y += m_fRotSpeed;
		}
		if (pKeyboard->GetKeyboardPress(DIK_LEFT) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_RIGHT_STICK).x <= -0.3f)
		{
			m_rot.y -= m_fRotSpeed;
		}
	}

#ifdef _DEBUG
	if (type == CCharManager::CONTROL_CAMERA)
	{
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A))
			{// ���O
				m_CPos.x -= sinf(m_rot.y + D3DX_PI * 3 / 4) * 3;
				m_CPos.z -= cosf(m_rot.y + D3DX_PI * 3 / 4) * 3;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D))
			{// �E�O
				m_CPos.x += sinf(m_rot.y + D3DX_PI * 1 / 4) * 3;
				m_CPos.z += cosf(m_rot.y + D3DX_PI * 1 / 4) * 3;
			}
			else
			{// �O
				m_CPos.x += sinf(m_rot.y) * 3;
				m_CPos.z += cosf(m_rot.y) * 3;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			if (pKeyboard->GetKeyboardPress(DIK_A))
			{// ����
				m_CPos.x -= sinf(m_rot.y + D3DX_PI * 1 / 4) * 3;
				m_CPos.z -= cosf(m_rot.y + D3DX_PI * 1 / 4) * 3;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D))
			{// �E��
				m_CPos.x += sinf(m_rot.y + D3DX_PI * 3 / 4) * 3;
				m_CPos.z += cosf(m_rot.y + D3DX_PI * 3 / 4) * 3;
			}
			else
			{// ��
				m_CPos.x -= sinf(m_rot.y) * 3;
				m_CPos.z -= cosf(m_rot.y) * 3;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_A))
		{// ��
			m_CPos.x -= sinf(m_rot.y + D3DX_PI * 1 / 2) * 3;
			m_CPos.z -= cosf(m_rot.y + D3DX_PI * 1 / 2) * 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_D))
		{// �E
			m_CPos.x += sinf(m_rot.y + D3DX_PI * 1 / 2) * 3;
			m_CPos.z += cosf(m_rot.y + D3DX_PI * 1 / 2) * 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E))
		{
			m_CPos.y += 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_Q))
		{
			m_CPos.y -= 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_UP))
		{
			m_rot.x -= m_fRotSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN))
		{
			m_rot.x += m_fRotSpeed;
		}

		CCharManager *pPlayer = nullptr;

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pPlayer = CGame::GetPlayer();
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pPlayer = CTutorial::GetPlayer();
		}

		if (pPlayer != nullptr)
		{
			pPlayer->ChangeControl(CCharManager::CONTROL_PLAYER);
		}
	}
#endif // _DEBUG

	// �p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================
// �J�����̓����蔻��
//==============================================
void CCamera::Collision()
{
	// ����̃^�C�v
	CCharManager::Control_Type type = CCharManager::GetControlType();

	CCharManager *pCharacter = nullptr;
	D3DXVECTOR3 CPos = { 0.0f,0.0f,0.0f };

	// �I�u�W�F�N�gX�̏��
	CObjectX **pObjectX = nullptr;

	// �Q�[��
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		if (type == CCharManager::CONTROL_PLAYER)
		{
			// �v���C���[
			pCharacter = CGame::GetPlayer();
		}
		else if (type == CCharManager::CONTROL_ROBOT)
		{
			// ���{�b�g
			pCharacter = CGame::GetRobot();
		}

		// �I�u�W�F�N�gX
		pObjectX = CGame::GetObjectX();
	}
	// �`���[�g���A��
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		if (type == CCharManager::CONTROL_PLAYER)
		{
			// �v���C���[
			pCharacter = CTutorial::GetPlayer();
		}
		else if (type == CCharManager::CONTROL_ROBOT)
		{
			// ���{�b�g
			pCharacter = CTutorial::GetRobot();
		}

		// �I�u�W�F�N�gX
		pObjectX = CTutorial::GetObjectX();
	}

	if (pCharacter != nullptr)
	{
		// �L�����N�^�[�̈ʒu
		CPos = pCharacter->GetPos();
	}

	// �v���C���[���王�_�̃x�N�g��
	D3DXVECTOR3 CameraVec = m_WorldCameraPosV - CPos;
	CameraVec.y = 0.0f;

	// ���K��
	D3DXVec3Normalize(&CameraVec, &CameraVec);

	// �I�u�W�F�N�g�̐�
	int nNumObject = (*pObjectX)->GetNumObject();

	D3DXVECTOR3 ObjPos = { 0.0f,0.0f,0.0f };	// �ʒu
	D3DXVECTOR3 ObjSize = { 0.0f,0.0f,0.0f };	// �T�C�Y
	D3DXVECTOR3 ObjMin = { 0.0f,0.0f,0.0f };	// �ŏ��l
	D3DXVECTOR3 ObjMax = { 0.0f,0.0f,0.0f };	// �ő�l

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		if (*pObjectX != nullptr)
		{
			CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (Type != CObjectX::OBJECTX_KEY_RED
				&& Type != CObjectX::OBJECTX_KEY_BLUE
				&& Type != CObjectX::OBJECTX_KEY_GREEN
				&& Type != CObjectX::OBJECTX_CEILING)
			{
				ObjPos = (*pObjectX)->GetPos();			// �ʒu
				ObjSize = (*pObjectX)->GetSize();		// �T�C�Y
				ObjMin = (*pObjectX)->GetMin();			// ���_�̍ŏ��l
				ObjMax = (*pObjectX)->GetMax();			// ���_�̍ő�l

				// �ŏ��l�ɃT�C�Y��������
				ObjMin.x *= ObjSize.x;
				ObjMin.z *= ObjSize.z;

				// �ő�l�ɃT�C�Y��������
				ObjMax.x *= ObjSize.x;
				ObjMax.z *= ObjSize.z;

				// �I�u�W�F�N�g�ɍő�A�ŏ��𑫂�
				D3DXVECTOR3 Min = ObjPos + ObjMin;
				D3DXVECTOR3 Max = ObjPos + ObjMax;

				// X���W�̃j�A�ƃt�@�[
				float fNearX = 0.0f;
				float fFarX = 0.0f;

				// Z���W�̃j�A�ƃt�@�[
				float fNearZ = 0.0f;
				float fFarZ = 0.0f;

				// �I�u�W�F�N�g��荶�ɂ���
				if (Min.x > CPos.x)
				{
					fNearX = (Min.x - CPos.x) / CameraVec.x;
					fFarX = (Max.x - CPos.x) / CameraVec.x;
				}
				// �I�u�W�F�N�g���E�ɂ���
				else if (Max.x < CPos.x)
				{
					fNearX = (Max.x - CPos.x) / CameraVec.x;
					fFarX = (Min.x - CPos.x) / CameraVec.x;
				}

				// �I�u�W�F�N�g����O�ɂ���
				if (Min.z > CPos.z)
				{
					fNearZ = (Min.z - CPos.z) / CameraVec.z;
					fFarZ = (Max.z - CPos.z) / CameraVec.z;
				}
				// �I�u�W�F�N�g��艜�ɂ���
				else if (Max.z < CPos.z)
				{
					fNearZ = (Max.z - CPos.z) / CameraVec.z;
					fFarZ = (Min.z - CPos.z) / CameraVec.z;
				}

				float in = 0.0f;	// �x�N�g���̓��葤
				float out = 0.0f;	// �x�N�g���̏o��

				if (Max.x > CPos.x && Min.x < CPos.x)
				{
					if (CameraVec.x == 0.0f)
					{
						fNearX = CameraVec.x;
						fFarX = 10000;
					}
					else if (CameraVec.x > 0.0f)
					{
						fFarX = (Max.x - CPos.x) / CameraVec.x;

						if (Min.z > CPos.z)
						{
							fNearZ = (Min.z - CPos.z) / CameraVec.z;
						}
						else if (Max.z < CPos.z)
						{
							fNearZ = (Max.z - CPos.z) / CameraVec.z;
						}
					}
					else if (CameraVec.x < 0.0f)
					{
						fFarX = (Min.x - CPos.x) / CameraVec.x;

						if (Min.z > CPos.z)
						{
							fNearZ = (Min.z - CPos.z) / CameraVec.z;
						}
						else if (Max.z < CPos.z)
						{
							fNearZ = (Max.z - CPos.z) / CameraVec.z;
						}
					}
				}
				if (Max.z > CPos.z && Min.z < CPos.z)
				{
					if (CameraVec.z == 0.0f)
					{
						fNearZ = CameraVec.z;
						fFarZ = 10000;
					}
					else if (CameraVec.z > 0.0f)
					{
						fFarZ = (Max.z - CPos.z) / CameraVec.z;

						if (Min.x > CPos.x)
						{
							fNearX = (Min.x - CPos.x) / CameraVec.x;
						}
						else if (Max.x < CPos.x)
						{
							fNearX = (Max.x - CPos.x) / CameraVec.x;
						}
					}
					else if (CameraVec.z < 0.0f)
					{
						fFarZ = (Min.z - CPos.z) / CameraVec.z;

						if (Min.x > CPos.x)
						{
							fNearX = (Min.x - CPos.x) / CameraVec.x;
						}
						else if (Max.x < CPos.x)
						{
							fNearX = (Max.x - CPos.x) / CameraVec.x;
						}
					}
				}

				// �������̃j�A����
				if (fNearX < fNearZ)
				{
					in = fNearZ;
				}
				else
				{
					in = fNearX;
				}

				// �߂����̃t�@�[����
				if (fFarX < fFarZ)
				{
					out = fFarX;
				}
				else
				{
					out = fFarZ;
				}

				float hit = out - in;

				float CameraDistance = sqrtf((m_WorldCameraPosV.x - CPos.x) * (m_WorldCameraPosV.x - CPos.x)
					+ (m_WorldCameraPosV.z - CPos.z) * (m_WorldCameraPosV.z - CPos.z));

				// hit���v���X�̏ꍇ�͓������Ă�
				if (hit > 0 && in - 35 < m_fPlayerDistance)
				{
					if (CPos.z > Max.z)
					{
						m_WorldCameraPosV.z = Max.z + 30;
					}
					else if (CPos.z < Min.z)
					{
						m_WorldCameraPosV.z = Min.z - 30;
					}

					if (CPos.x > Max.x)
					{
						m_WorldCameraPosV.x = Max.x + 30;
					}
					else if (CPos.x < Min.x)
					{
						m_WorldCameraPosV.x = Min.x - 30;
					}
				}
			}
		}
	}
}

//==============================================
// �S�[�����f��
//==============================================
void CCamera::GoalCamera()
{
	if (m_nShiftGoalCount > 0)
	{
		m_bGoalOpen = CObjectX::GetGoalOpen();

		if (m_bGoalOpen)
		{
			m_nShiftGoalCount--;
			m_nGoalCount--;

			if (m_nGoalCount == 0)
			{
				m_Saverot = m_rot;

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

				if (pObjectX != nullptr)
				{
					// �I�u�W�F�N�g�̐�
					int nNumObject = (*pObjectX)->GetNumObject();

					for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
					{
						// �I�u�W�F�N�g�̃^�C�v
						CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

						if (Type == CObjectX::OBJECTX_GOAL)
						{
							D3DXVECTOR3 Pos = (*pObjectX)->GetPos();
							D3DXVECTOR3 Rot = (*pObjectX)->GetRot();

							Pos.x += sinf(Rot.y) * -80;
							Pos.z += cosf(Rot.y) * -80;

							Pos.x += sinf(Rot.y + D3DX_PI / 2) * 80;
							Pos.z += cosf(Rot.y + D3DX_PI / 2) * 80;

							m_CPos = Pos;
							m_rot.y = Rot.y - 1.0f;
						}
					}
				}
			}
		}
	}
	else if(m_nShiftGoalCount == 0)
	{
		m_nShiftGoalCount--;

		// ����̃^�C�v
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (type == CCharManager::CONTROL_PLAYER)
		{
			// �v���C���[�̈ʒu�Ɉړ�
			m_CPos = m_PPos;
			m_CPos.y = 0.0f;
			m_rot = m_Saverot;
		}
		else if (type == CCharManager::CONTROL_ROBOT)
		{
			// ���{�b�g�̈ʒu�Ɉړ�
			m_CPos = m_RPos;
			m_CPos.y = m_RPos.y;
			m_rot = m_Saverot;
		}

		m_bGoalOpen = false;
	}
}