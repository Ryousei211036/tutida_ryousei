//==============================================================================================
// 
// �L�����N�^�[�}�l�[�W���[�@�@�@charmanager.cpp
// �y�c����
// 
//==============================================================================================
#include"charmanager.h"
#include"model.h"
#include"application.h"
#include"file.h"
#include"game.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"objectX.h"
#include"tutorial.h"

CCharManager::Control_Type CCharManager::m_Control_Type = CONTROL_PLAYER;
int CCharManager::m_nControlChange_CoolTime = 60;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCharManager::CCharManager()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCharManager::~CCharManager()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CCharManager::Init()
{
	m_Control_Type = CONTROL_PLAYER;	// ����̃^�C�v

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			// �p�[�c�̏����ʒu
			InitPos[nCnt] = m_pModel[nCnt]->GetPos();
			InitRot[nCnt] = m_pModel[nCnt]->GetRot();
		}
	}

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CCharManager::Uninit()
{
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
		}
	}

	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCharManager::Update()
{
	// ���[�V�����ԍ��̐ݒ�
	ChangeMotion(m_Motion_Type);

	// ���[�V����
	Motion();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CCharManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			// ���f���̕`�揈��
			m_pModel[nCnt]->Draw(m_mtxWorld);
		}
	}
}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CCharManager::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// �L�[�t���[���̐ݒ�
	m_MotionSet[motion].aKeySet[key].nFrame = frame;

	// �ʒu�Ɗp�x�̐ݒ�
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyRot = rot;

	// �L�[�̍ő吔
	m_MotionSet[motion].nMaxKey = maxkey;

	// ���[�v���邩
	m_MotionSet[motion].bLoop = loop;
}

//==============================================================================================
// ���[�V�����̍Đ�
//==============================================================================================
void CCharManager::Motion()
{
	// ���[�V�����J�E���^�[
	m_nCountMotion++;

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr && !m_MotionSet[m_nCurrentMotion].bStop)
		{
			// ���Βl
			float fRelative = (float)m_nCountMotion / (float)m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame;

			// ����
			D3DXVECTOR3 fPosDifference;		// �ʒu�p
			D3DXVECTOR3 fRotDifference;		// ��]�p

			// ����
			// �Ō�̃L�[����Ȃ��ꍇ
			if (m_nCurrentKey != m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// �ʒu
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// �I���l - �J�n�l

				// �p�x
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// �I���l - �J�n�l
			}
			// �Ō�̃L�[�̏ꍇ
			else if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// �ʒu
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// �I���l - �J�n�l

				// �p�x
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// �I���l - �J�n�l
			}

			// ���ݒl
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// �ʒu
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// ��]

			POS[nCnt] = Pos;
			ROT[nCnt] = Rot;

			m_pModel[nCnt]->SetPos(Pos);	// �ʒu�̐ݒ�
			m_pModel[nCnt]->SetRot(Rot);	// ��]�̐ݒ�
		}
	}

	// �Ō�̃t���[���܂Ői��
	if (m_nCountMotion >= m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame)
	{
		// �t���[����0�ɖ߂�
		m_nCountMotion = 0;

		// �L�[��i�߂�
		m_nCurrentKey++;

		// ���[�v����
		if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey
			&& m_MotionSet[m_nCurrentMotion].bLoop)
		{
			// �L�[��߂��ă��[�v����
			m_nCurrentKey = 0;
		}

		// ���[�v���Ȃ�
		else if (m_nCurrentKey >= m_MotionSet[m_nCurrentMotion].nMaxKey
			&& !m_MotionSet[m_nCurrentMotion].bLoop)
		{
			m_MotionSet[m_nCurrentMotion].bPlay = false;
			m_MotionSet[m_nCurrentMotion].bStop = true;

			for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
			{
				m_pModel[nCnt]->SetPos(POS[nCnt]);	// �ʒu�̐ݒ�
				m_pModel[nCnt]->SetRot(ROT[nCnt]);	// ��]�̐ݒ�
			}
		}
	}
}

//==============================================================================================
// ���[�V����������
//==============================================================================================
bool CCharManager::PlayMotion(Motion_Type motion)
{
	return m_MotionSet[motion].bPlay;
}

//==============================================================================================
// ���[�V�����ύX
//==============================================================================================
void CCharManager::ChangeMotion(int nummotion)
{
	// ���݂̃��[�V��������ς�����ꍇ
	if (m_nCurrentMotion != nummotion)
	{
		// �O��̃��[�V�����̃v���C����false�ɂ���
		m_MotionSet[m_nCurrentMotion].bPlay = false;
		
		// �O��̎~�܂������[�V������false�ɂ���
		m_MotionSet[m_nCurrentMotion].bStop = false;

		m_nCurrentMotion = nummotion;	// ���[�V������؂�ւ���
		m_nCurrentKey = 0;				// �L�[��0�ɂ���
		m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���

		// ���̃��[�V�����̃v���C����true�ɂ���
		m_MotionSet[m_nCurrentMotion].bPlay = true;
	}
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel *CCharManager::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
{
	if (m_pModel[index] == nullptr)
	{
		if (parent >= 0)
		{// �e������ꍇ
			m_pModel[index] = CModel::Create(pos, rot, m_pModel[parent], Xfilename, type);
		}
		else
		{// �e�����Ȃ��ꍇ
			m_pModel[index] = CModel::Create(pos, rot, nullptr, Xfilename, type);
		}
	}

	return m_pModel[index];
}

//==============================================================================================
// ����̕ύX
//==============================================================================================
void CCharManager::ChangeControl(Control_Type type)
{
	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// ����ύX�̃N�[���^�C��
	bool bChange = ChangeCoolTime();

	if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_V) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_X))
		&& bChange)
	{
		if (CApplication::GetMode() == CApplication::MODE_GAME
			&& CGame::GetRobot() == nullptr)
		{
			CCharManager *pRobot;

			pRobot = *CGame::GetFile()->LoadCharFile(CFile::TXTNAME_ROBOT, m_Pos);

			CGame::SetRobot(pRobot);
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL
			&& CTutorial::GetRobot() == nullptr)
		{
			CCharManager *pRobot;

			pRobot = *CTutorial::GetFile()->LoadCharFile(CFile::TXTNAME_ROBOT, m_Pos);

			CTutorial::SetRobot(pRobot);
		}

		// �����؂�ւ���
		CCharManager::SetControl(type);

		// �N�[���^�C���̃��Z�b�g
		SetChangeCoolTime(60);
	}

#ifdef _DEBUG
	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_O))
	{
		// �I�u�W�F�N�g����ɐ؂�ւ���
		CCharManager::SetControl(CONTROL_CAMERA);
	}
#endif // _DEBUG
}

//==============================================================================================
// ����ύX�̃N�[���^�C��
//==============================================================================================
bool CCharManager::ChangeCoolTime()
{
	m_nControlChange_CoolTime--;

	if (m_nControlChange_CoolTime <= 0)
	{
		return true;
	}

	return false;
}

//==============================================================================================
// �_���[�W
//==============================================================================================
int CCharManager::Damage(int hp, int damage)
{
	int nHp = hp - damage;

	return nHp;
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
float CCharManager::NormalizeRot(float rotspeed)
{
	// �ړI�̊p�x�̐��K��
	if (m_RotDest.y - m_Rot.y > D3DX_PI)
	{
		m_RotDest.y -= D3DX_PI * 2;
	}
	else if (m_RotDest.y - m_Rot.y < -D3DX_PI)
	{
		m_RotDest.y += D3DX_PI * 2;
	}

	// �ړI�̊p�x�ɂ���
	m_Rot.y += (m_RotDest.y - m_Rot.y) * rotspeed;

	// �p�x�̐��K��
	if (m_Rot.y > D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2;
	}
	else if (m_Rot.y < -D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2;
	}

	return m_Rot.y;
}