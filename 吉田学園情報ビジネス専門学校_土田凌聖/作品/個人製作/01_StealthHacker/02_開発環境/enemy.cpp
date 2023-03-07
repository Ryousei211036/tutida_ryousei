//==============================================================================================
//
// �G�@�@�@enemy.cpp
// �y�c����
//
//==============================================================================================
#include"enemy.h"
#include"model.h"
#include"application.h"
#include"game.h"
#include"objectX.h"
#include"bullet.h"
#include"object3D.h"
#include"score.h"
#include"sound.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"tutorial.h"
#include"tutorial_text.h"
#include"model.h"

#include"debugproc.h"

int CEnemy::m_nNumEnemy = 0;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CEnemy::CEnemy()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CEnemy::~CEnemy()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CEnemy::Init()
{
	CCharManager::Init();

	m_fRadius = 30.0f;			// ���a
	m_fKillArea = 10.0f;		// �X�e���X�L���ł���͈�
	m_fKillDistance = 15.0f;	// �X�e���X�L���ł��鋗��
	m_fRotSpeed = 0.1f;			// ��]���x
	m_nLife = 120;				// �̗�
	m_fSpeed = 2;				// �ړ����x(����)
	m_nBulletCoolTime = 50;		// �e�����Ԋu
	m_bFind_Player = false;		// �v���C���[����������
	m_bFind_Robot = false;		// ���{�b�g����������
	m_bDeath = false;			// ���S������
	HitObject = true;
	m_nAlpha_Exclamation = 50;

	m_fView_Distance = 650.0f;
	m_TargetPos = CCharManager::GetPos();

	m_Pos = CCharManager::GetPos();

	pObject3D = CObject3D::Create({ m_Pos.x,m_Pos.y + 120.0f,m_Pos.z }, { 20.0f,20.0f,20.0f }, { 1.0f,1.0f,1.0f,1.0f }, true);
	pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
	pObject3D->SetTexture(CTexture::TEXTURE_EXCLAMATION);

	CObject *pTop = GetTop(3);

	if (pTop != nullptr)
	{
		CObject *pObject = pTop;

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->GetNext();

			if (pObject->GetObjType() == OBJTYPE_ENEMY
				&& !pObject->GetDeathFlag())
			{
				// �̗͂̐ݒ�
				pObject->SetHp(m_nLife);
			}

			pObject = pObjectNext;
		}
	}

	CCharManager::SetRadius(m_fRadius);
	CCharManager::SetLife(m_nLife);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CEnemy::Uninit()
{
	CCharManager::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CEnemy::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCharManager::Update();

		// �̗͂̎擾
		m_nLife = GetHp();

		if (!m_bDeath)
		{
			// �ړ�����
			Move();

			// �A�N�V����
			Action();

			if (!m_bStealthDeath)
			{
				// ���G
				Search(OBJTYPE_PLAYER);		// �v���C���[
				Search(OBJTYPE_ROBOT);		// ���{�b�g
				Search(OBJTYPE_OBJECTX);	// �I�u�W�F�N�gX
			}
		}

		pObject3D->SetPos({ m_Pos.x,m_Pos.y + 120.0f,m_Pos.z });

		// ���S
		Death();

		// ���[�V�����̐ݒ�
		SetMotion(m_Motion);

		if (!m_bPlay_Shot)
		{
			// �j���[�g������Ԃɂ���
			m_Motion = CCharManager::MOTION_NEUTRAL;
		}
	}

	CObject::SetPos(m_Pos);
	CObject::SetRadius(m_fRadius);
	CObject::SetHp(m_nLife);
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CEnemy::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->CCharManager::SetPos(pos);
		pEnemy->CCharManager::SetPosOld(pos);
		pEnemy->CCharManager::SetRotDest(rot);
	}

	return pEnemy;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CEnemy::Move()
{
	if (!m_bFind_Player && !m_bFind_Robot && !m_bStealthDeath)
	{
		m_PosOld = m_Pos;

		m_Pos = GetPos();
		m_RotDest = GetRotDest();
		m_PosOld = m_Pos;

		// �^�[�Q�b�g�̕�������
		if (m_bLostTarget)
		{
			m_RotDest.y = (float)atan2(m_TargetPos.x - m_Pos.x, m_TargetPos.z - m_Pos.z);
		}

		float Distance = CObject::Distance(m_TargetPos, m_Pos);

		if (m_fRadius < Distance)
		{
			m_Pos.x += sinf(m_RotDest.y) * m_fSpeed;
			m_Pos.z += cosf(m_RotDest.y) * m_fSpeed;
			
			m_Motion = CCharManager::MOTION_WALK;
		}
		else
		{
			m_bLostTarget = false;
		}
	}

	// �p�x�̐��K��
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	// �e�N���X�ɓn��
	CCharManager::SetPos(m_Pos);
	CCharManager::SetPosOld(m_PosOld);
	CCharManager::SetRotDest(m_RotDest);
}

//==============================================================================================
// �G�̃A�N�V����
//==============================================================================================
void CEnemy::Action()
{
	// �v���C���[�̏��
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

	D3DXVECTOR3 PPos = { 0.0f,0.0f,0.0f };			// �ʒu
	int nPLife = 0;									// �̗�
	float fPRadius = 0.0f;							// ���a
	CCharManager::Motion_Type Motion = MOTION_NONE;	// �v���C���[�̏��

	// ���{�b�g�̏��
	int nRLife = 0;									// �̗�
	D3DXVECTOR3 RPos = { 0.0f,0.0f,0.0f };			// �ʒu
	float fRRadius = 0.0f;							// ���a

	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// �v���C���[�̏��
	if (pPlayer != nullptr)
	{
		PPos = pPlayer->GetPos();			// �ʒu
		nPLife = pPlayer->GetLife();		// �̗�
		fPRadius = pPlayer->GetRadius();	// ���a
		Motion = pPlayer->GetMotion_Type();	// ���

	}
	// ���{�b�g�̏��
	if (pRobot != nullptr)
	{
		RPos = pRobot->GetPos();			// �ʒu
		nRLife = pRobot->GetLife();			// �̗�
		fRRadius = pRobot->GetRadius();		// ���a
	}

	// ���݂̈ʒu��ۑ�
	D3DXVECTOR3 pos = m_Pos;

	// �X�e���X�L���ł���ʒu
	pos.x -= sinf(m_Rot.y) * m_fKillDistance;
	pos.z -= cosf(m_Rot.y) * m_fKillDistance;

	float PDistance = 0.0f;
	float RDistance = 0.0f;

	// �v���C���[�ƓG�̋���
	PDistance = CObject::Distance(PPos, pos);
	// ���{�b�g�ƓG�̋���
	RDistance = CObject::Distance(RPos, pos);

	// �X�e���X�L���ł��鋗���Ƀv���C���[������
	if (fPRadius + m_fKillArea > PDistance
		&& !m_bFind_Player && !m_bFind_Robot)
	{
		if ((pKeyboard->GetKeyboardTrigger(DIK_F) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_B)))
		{
			m_bStealthDeath = true;
			pPlayer->SetPlayStealthKill(m_bStealthDeath);
		}
	}

	if (m_bStealthDeath)
	{
		m_bFind_Player = false;
		m_bFind_Robot = false;
		pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });

		// �X�e���X�f�X���[�V����
		m_Motion = CCharManager::MOTION_STEALTHDEATH;

		// ���[�V�����ύX
		ChangeMotion(m_Motion);

		pPlayer->SetPos(pos);
		pPlayer->SetRotDest(m_Rot);
		pPlayer->ChangeMotion(CCharManager::MOTION_KILL);

		bool bMotion = PlayMotion(CCharManager::MOTION_STEALTHDEATH);

		// ���[�V�������I�������
		if (!bMotion)
		{
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CGame::GetScore()->AddScore(100);
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				CTutorial::GetScore()->AddScore(100);
			}

			pPlayer->SetPlayStealthKill(false);

			// �X�e���X�L��
			Uninit();
		}
	}

	bool Hit = CObject::GetHit();
	
	if (Hit)
	{
		m_bHit = true;
	}
	if (m_bHit)
	{
		// �v���C���[�̕�������
		m_RotDest.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);

		CCharManager::SetRotDest(m_RotDest);
	}

	// �G�Ɍ��������ꍇ
	if (!m_bStealthDeath && (m_bFind_Player || m_bFind_Robot))
	{
		if (!m_bPlay_Shot)
		{
			// �e���\����
			m_Motion = CCharManager::MOTION_GUN;

			if (!FindSound)
			{
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_FINDENEMY);
				FindSound = true;
			}
		}

		// �v���C���[����������
		if (m_bFind_Player)
		{
			if (pObject3D != nullptr)
			{
				m_nAlpha_Exclamation--;

				if (m_nAlpha_Exclamation > 0)
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,1.0f });


				}
				else
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
				}
			}

			// ��苗�����ꂽ��U������߂�
			if (fPRadius + m_fView_Distance < PDistance)
			{
				FindSound = false;
				m_bFind_Player = false;
				m_nAlpha_Exclamation = 50;
				pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			}

			// �v���C���[�̕�������
			m_RotDest.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);
		}
		// ���{�b�g����������
		else if (m_bFind_Robot)
		{
			if (pObject3D != nullptr)
			{
				m_nAlpha_Exclamation--;

				if (m_nAlpha_Exclamation > 0)
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,1.0f });
				}
				else
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
				}
			}

			// ��苗�����ꂽ��U������߂�
			if (fRRadius + m_fView_Distance < RDistance)
			{
				m_bHit = false;
				FindSound = false;
				m_bFind_Robot = false;
				m_nAlpha_Exclamation = 50;
				pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			}

			// ���{�b�g�̕�������
			m_RotDest.y = (float)atan2(RPos.x - m_Pos.x, RPos.z - m_Pos.z);
		}

		D3DXVECTOR3 m_GunPos = m_Pos;
		m_GunPos.y = 80.0f;

		m_GunPos.x += sinf(m_Rot.y) * 50;
		m_GunPos.z += cosf(m_Rot.y) * 50;

		m_GunPos.x += sinf(m_Rot.y + D3DX_PI / 2) * 10.0f;
		m_GunPos.z += cosf(m_Rot.y + D3DX_PI / 2) * 10.0f;

		m_nBulletCoolTime--;

		if (m_nBulletCoolTime <= 0)
		{
			m_Motion = CCharManager::MOTION_SHOT;

			m_bPlay_Shot = true;

			// �e������
			CBullet::Create(m_GunPos, m_Rot, { 0.2f,0.2f,0.2f }, "Data/XFILE/Player/01_head.x", CObjectX::OBJECTX_BULLET, CModel::MODEL_TYPE_ENEMY);

			m_nBulletCoolTime = 50;
		}

		// ���[�V�����ύX
		ChangeMotion(m_Motion);

		if (!PlayMotion(MOTION_SHOT))
		{
			m_bPlay_Shot = false;
		}

		// �v���C���[�A���{�b�g�̗̑͂�0�ɂȂ����Ƃ�
		if (nPLife <= 0)
		{
			m_bFind_Player = false;
			m_Motion = CCharManager::MOTION_NEUTRAL;
		}
		if (pRobot != nullptr)
		{
			if (nRLife <= 0)
			{
				m_bFind_Robot = false;
			}
		}
	}
}

//==============================================================================================
// �̗͂�0�ɂȂ���
//==============================================================================================
void CEnemy::Death()
{
	// �̗͂�0�ɂȂ���
	if (m_nLife <= 0)
	{
		pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });

		m_bFind_Player = false;
		m_bFind_Robot = false;
		m_bDeath = true;

		m_Motion = CCharManager::MOTION_DEATH;

		// ���[�V�����ύX
		ChangeMotion(m_Motion);

		bool bMotion = PlayMotion(CCharManager::MOTION_DEATH);

		// ���[�V�������I�������
		if (!bMotion)
		{
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CGame::GetScore()->AddScore(50);
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				CTutorial::GetScore()->AddScore(50);
			}

			// ���S
			Uninit();
		}
	}
}

//==============================================================================================
// �v���C���[�����G����
//==============================================================================================
void CEnemy::Search(CObject::EObjType type)
{
	if (!m_bFind_Player && !m_bFind_Robot)
	{
		// �����̈ړ�
		if (m_EyeRot.y <= m_Rot.y + D3DX_PI * 1 / 4
			&& !m_bEyeType)
		{
			m_EyeRot.y += 0.05f;
		}
		if (m_EyeRot.y >= m_Rot.y + D3DX_PI * 1 / 4)
		{
			m_bEyeType = true;
		}

		if (m_EyeRot.y >= m_Rot.y - D3DX_PI * 1 / 4
			&& m_bEyeType)
		{
			m_EyeRot.y -= 0.05f;
		}
		if (m_EyeRot.y <= m_Rot.y - D3DX_PI * 1 / 4)
		{
			m_bEyeType = false;
		}
	}

	// �����x�N�g��
	D3DXVECTOR3 EyeVec = { 0.0f,0.0f,0.0f };

	EyeVec.x = sinf(m_EyeRot.y);
	EyeVec.z = cosf(m_EyeRot.y);
	EyeVec.y = 0.0f;

	// �x�N�g���̐��K��
	D3DXVec3Normalize(&EyeVec, &EyeVec);

	D3DXVECTOR3 ObjPos = { 0.0f,0.0f,0.0f };	// �ʒu
	D3DXVECTOR3 ObjSize = { 0.0f,0.0f,0.0f };	// �T�C�Y
	D3DXVECTOR3 ObjMin = { 0.0f,0.0f,0.0f };	// �ŏ��l
	D3DXVECTOR3 ObjMax = { 0.0f,0.0f,0.0f };	// �ő�l
	float CharRadius = 0.0f;	// �L�����̔��a
	int nLife = 1;				// �̗�
	int nNumObject = 1;			// �I�u�W�F�N�g�̐�
	CObjectX::ObjectX_Type Type = CObjectX::OBJECTX_NONE;	// �I�u�W�F�N�gX�̃^�C�v

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

	if ((*pObjectX) != nullptr && type == OBJTYPE_OBJECTX)
	{
		// �I�u�W�F�N�g�̐�
		nNumObject = (*pObjectX)->GetNumObject();
	}

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		// �I�u�W�F�N�gX�̏ꍇ
		if (type == OBJTYPE_OBJECTX && (*pObjectX) != nullptr)
		{
			bool bOpenDoor = (*pObjectX)->OpenDoor();
			CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (bOpenDoor && Type != CObjectX::OBJECTX_CEILING)
			{
				ObjPos = (*pObjectX)->GetPos();			// �ʒu
				ObjSize = (*pObjectX)->GetSize();		// �T�C�Y
				ObjMin = (*pObjectX)->GetMin();			// ���_�̍ŏ��l
				ObjMax = (*pObjectX)->GetMax();			// ���_�̍ő�l
				Type = (*pObjectX)->GetObjectXType();	// �^�C�v
				OPos = ObjPos;			// �v�Z�p
			}
		}
		else if (type == OBJTYPE_PLAYER)
		{
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

			if (pPlayer != nullptr)
			{
				ObjPos = pPlayer->GetPos();			// �ʒu
				ObjSize = { 1.0f,1.0f,1.0f };		// �T�C�Y
				ObjMin = pPlayer->GetSizeMin();		// ���_�̍ŏ��l
				ObjMax = pPlayer->GetSizeMax();		// ���_�̍ő�l
				CharRadius = pPlayer->GetRadius();	// ���a
				nLife = pPlayer->GetLife();			// �̗�
				PPos = ObjPos;		// �v�Z�p
			}
		}
		else if (type == OBJTYPE_ROBOT)
		{
			// ���{�b�g�̏��
			CCharManager *pRobot = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pRobot = CGame::GetRobot();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pRobot = CTutorial::GetRobot();
			}

			if (pRobot != nullptr)
			{
				ObjPos = pRobot->GetPos();			// �ʒu
				ObjSize = { 1.0f,1.0f,1.0f };		// �T�C�Y
				ObjMin = pRobot->GetSizeMin();		// ���_�̍ŏ��l
				ObjMax = pRobot->GetSizeMax();		// ���_�̍ő�l
				CharRadius = pRobot->GetRadius();	// ���a
				nLife = pRobot->GetLife();			// �̗�
				RPos = ObjPos;		// �v�Z�p
			}
		}

		// �I�u�W�F�N�g����G�̃x�N�g��
		D3DXVECTOR3 Vec = ObjPos - m_Pos;

		// ���K��
		D3DXVec3Normalize(&Vec, &Vec);

		// ����
		float fInner = D3DXVec3Dot(&Vec, &EyeVec);

		// �ŏ��l�ɃT�C�Y��������
		ObjMin.x *= ObjSize.x;
		ObjMin.z *= ObjSize.z;

		// �ő�l�ɃT�C�Y��������
		ObjMax.x *= ObjSize.x;
		ObjMax.z *= ObjSize.z;

		// �I�u�W�F�N�g�ɍő�A�ŏ��𑫂�
		D3DXVECTOR3 Min = ObjPos + ObjMin;
		D3DXVECTOR3 Max = ObjPos + ObjMax;

		// �G�̑O�ɂ���
		if (fInner > 0)
		{
			// X���W�̃j�A�ƃt�@�[
			float fNearX = 0.0f;
			float fFarX = 0.0f;

			// Z���W�̃j�A�ƃt�@�[
			float fNearZ = 0.0f;
			float fFarZ = 0.0f;

			// �I�u�W�F�N�g��荶�ɂ���
			if (Min.x > m_Pos.x)
			{
				fNearX = (Min.x - m_Pos.x) / EyeVec.x;
				fFarX = (Max.x - m_Pos.x) / EyeVec.x;
			}
			// �I�u�W�F�N�g���E�ɂ���
			else if (Max.x < m_Pos.x)
			{
				fNearX = (Max.x - m_Pos.x) / EyeVec.x;
				fFarX = (Min.x - m_Pos.x) / EyeVec.x;
			}

			// �I�u�W�F�N�g����O�ɂ���
			if (Min.z > m_Pos.z)
			{
				fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
				fFarZ = (Max.z - m_Pos.z) / EyeVec.z;
			}
			// �I�u�W�F�N�g��艜�ɂ���
			else if (Max.z < m_Pos.z)
			{
				fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
				fFarZ = (Min.z - m_Pos.z) / EyeVec.z;
			}

			float in = 0.0f;	// �x�N�g���̓��葤
			float out = 0.0f;	// �x�N�g���̏o��

			if (Max.x > m_Pos.x && Min.x < m_Pos.x)
			{
				if (EyeVec.x == 0.0f)
				{
					fNearX = EyeVec.x;
					fFarX = 10000;
				}
				else if (EyeVec.x > 0.0f)
				{
					fFarX = (Max.x - m_Pos.x) / EyeVec.x;

					if (Min.z > m_Pos.z)
					{
						fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
					}
					else if (Max.z < m_Pos.z)
					{
						fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
					}
				}
				else if (EyeVec.x < 0.0f)
				{
					fFarX = (Min.x - m_Pos.x) / EyeVec.x;

					if (Min.z > m_Pos.z)
					{
						fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
					}
					else if (Max.z < m_Pos.z)
					{
						fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
					}
				}
			}
			if (Max.z > m_Pos.z && Min.z < m_Pos.z)
			{
				if (EyeVec.z == 0.0f)
				{
					fNearZ = EyeVec.z;
					fFarZ = 10000;
				}
				else if (EyeVec.z > 0.0f)
				{
					fFarZ = (Max.z - m_Pos.z) / EyeVec.z;

					if (Min.x > m_Pos.x)
					{
						fNearX = (Min.x - m_Pos.x) / EyeVec.x;
					}
					else if (Max.x < m_Pos.x)
					{
						fNearX = (Max.x - m_Pos.x) / EyeVec.x;
					}
				}
				else if (EyeVec.z < 0.0f)
				{
					fFarZ = (Min.z - m_Pos.z) / EyeVec.z;

					if (Min.x > m_Pos.x)
					{
						fNearX = (Min.x - m_Pos.x) / EyeVec.x;
					}
					else if (Max.x < m_Pos.x)
					{
						fNearX = (Max.x - m_Pos.x) / EyeVec.x;
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

			// hit���v���X�̏ꍇ�͓������Ă�
			if (hit > 0 && nLife > 0)
			{
				float PDistance = CObject::Distance(m_Pos, PPos);
				float RDistance = CObject::Distance(m_Pos, RPos);

				if (type == OBJTYPE_PLAYER
					&& m_fView_Distance > PDistance)
				{
					// �v���C���[��������
					m_bFind_Player = true;
				}
			
				// �v���C���[�̑O�ɃI�u�W�F�N�g������
				if (type == OBJTYPE_OBJECTX && m_bFind_Player
					&& PDistance > in)
				{
					m_Motion = MOTION_NEUTRAL;

					FindSound = false;
					m_bFind_Player = false;

					m_nAlpha_Exclamation = 50;
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
					CObject *pTop = GetTop(3);

					if (pTop != nullptr)
					{
						CObject *pObject = pTop;

						while (pObject != nullptr)
						{
							CObject *pObjectNext = pObject->GetNext();

							if (pObject->GetObjType() == OBJTYPE_ENEMY
								&& !pObject->GetDeathFlag())
							{
								// �̗͂̐ݒ�
								pObject->SetHit(false);
							}

							pObject = pObjectNext;
						}
					}
					m_bHit = false;

					m_EyeRot.y = m_Rot.y;

					break;
				}
				// �I�u�W�F�N�g�̑O�Ƀv���C���[������
				else if (type == OBJTYPE_OBJECTX && m_bFind_Player
					&& PDistance <= in)
				{
					m_bFind_Player = true;

					// �v���C���[�̕�������
					m_EyeRot.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);
				}
			}
		}
	}
}

//==============================================================================================
// ���[�V�������̏���
//==============================================================================================
void CEnemy::Process_Motion()
{
}

//==============================================================================================
// ���[�V�����l�̐ݒ�
//==============================================================================================
void CEnemy::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// ���[�V��������
//==============================================================================================
void CEnemy::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// ��������
//==============================================================================================
void CEnemy::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel *CEnemy::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
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