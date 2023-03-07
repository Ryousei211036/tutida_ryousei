//==============================================================================================
//
// �e�@�@�@bullet.cpp
// �y�c����
//
//==============================================================================================
#include"bullet.h"
#include"player.h"
#include"game.h"
#include"application.h"
#include"enemy.h"
#include"sound.h"
#include"tutorial.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CBullet::CBullet()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CBullet::~CBullet()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CBullet::Init()
{
	CObjectX::Init();

	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SHOT);

	m_nDeathTime = 25;		// �e�̎���
	m_fSpeed = 20.0f;		// �ړ����x
	m_nAddDamage = 30;		// �^����_���[�W
	m_fRadius = 5.0f;		// ���a

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CBullet::Uninit()
{
	CObjectX::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CBullet::Update()
{
	CObjectX::Update();

	// �e��������
	Hit();

	// ����
	Death();

	// �e�����ł����p�x
	m_Pos.x += sinf(m_Rot.y) * m_fSpeed;
	m_Pos.z += cosf(m_Rot.y) * m_fSpeed;

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

	// �I�u�W�F�N�g�̐�
	int nNumObject = (*pObjectX)->GetNumObject();

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		if (*pObjectX != nullptr)
		{
			// �I�u�W�F�N�g�Ƃ̓����蔻��
			bool Hit = (*pObjectX)->Collision(m_Pos);
			ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (Hit && Type != OBJECTX_CEILING)
			{
				// ����
				Uninit();
			}
		}
	}

	// �e�N���X�ɓn��
	CObjectX::SetPos(m_Pos);
	CObjectX::SetRot(m_Rot);
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CBullet::Draw()
{
	CObjectX::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, ObjectX_Type type, CModel::EModel_Type modeltype)
{
	CBullet *pBullet = nullptr;

	pBullet = new CBullet;

	if (pBullet != nullptr)
	{
		pBullet->SetModelType(modeltype);
		pBullet->SetPos(pos);
		pBullet->SetRot(rot);
		pBullet->CObjectX::SetSize(size);
		pBullet->CObjectX::SetXFileName(name);
		pBullet->CObjectX::SetType(type);
		pBullet->Init();
	}

	return pBullet;
}

//==============================================================================================
// �e�̎���
//==============================================================================================
void CBullet::Death()
{
	m_nDeathTime--;

	if (m_nDeathTime <= 0)
	{
		Uninit();
	}
}

//==============================================================================================
// �e����������
//==============================================================================================
void CBullet::Hit()
{
	D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };		// �ʒu
	float CharRadius = 0.0f;	// ���a
	int CharHp = 0;				// �̗�

	// �G�̏��
	CCharManager **pEnemy = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pEnemy = CGame::GetEnemy();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pEnemy = CTutorial::GetEnemy();
	}

	if (m_nType == CModel::MODEL_TYPE_PLAYER)
	{
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
					D3DXVECTOR3 EPos = { 0.0f,0.0f,0.0f };
					float fERadius = 0.0f;
					int nEHp = 0;

					EPos = pObject->GetPos();
					fERadius = pObject->GetRadius();
					nEHp = pObject->GetHp();

					// �G�ƒe�̋���
					float fDistance = CObject::Distance(EPos, m_Pos);

					if (m_fRadius + fERadius >= fDistance)
					{
						// �̗͂̐ݒ�
						pObject->SetHp(nEHp - m_nAddDamage);
						pObject->SetHit(true);

						Uninit();
					}
				}

				pObject = pObjectNext;
			}
		}
	}
	else if (m_nType == CModel::MODEL_TYPE_ENEMY)
	{
		// �v���C���[�̏��
		CCharManager *pPlayer = nullptr;
		// ���{�b�g�̏��
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
		
		if (pPlayer != nullptr)
		{
			CharPos = pPlayer->GetPos();		// �ʒu
			CharRadius = pPlayer->GetRadius();	// ���a
			CharHp = pPlayer->GetLife();		// �̗�

			// �v���C���[�ƒe�̋���
			float fPDistance = CObject::Distance(CharPos, m_Pos);

			if (m_fRadius + CharRadius >= fPDistance)
			{
				// �̗͂̐ݒ�
				pPlayer->SetLife(pPlayer->Damage(CharHp, m_nAddDamage));

				Uninit();
			}
		}

		if (pRobot != nullptr)
		{
			CharPos = pRobot->GetPos();			// �ʒu
			CharRadius = pRobot->GetRadius();	// ���a
			CharHp = pRobot->GetLife();			// �̗�

			// ���{�b�g�ƒe�̋���
			float fRDistance = CObject::Distance(CharPos, m_Pos);

			if (m_fRadius + CharRadius >= fRDistance)
			{
				// �̗͂̐ݒ�
				pRobot->SetLife(pRobot->Damage(CharHp, m_nAddDamage));

				Uninit();
			}
		}
	}
}