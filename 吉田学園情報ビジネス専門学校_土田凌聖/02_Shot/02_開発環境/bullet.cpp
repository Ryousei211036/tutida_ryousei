//==============================================================================================
//
// �e�@�@�@bullet.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"bullet.h"
#include"player.h"
#include"exp.h"
#include"sound.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CBullet::CBullet() : CObject2D(1)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �����ʒu
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���
	m_PlayerShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �v���C���[�̒e�̈ړ���
	m_EnemyShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �G�̒e�̈ړ���
	m_nType = 0;										// �e�̎��
	m_fWidth = 0.0f;									// ��
	m_fHeight = 0.0f;									// ����
	m_fRot = 0.0f;										// �p�x
	m_fPBulletLife = 0.0f;								// �v���C���[�̒e�̎���
	m_fEBulletLife = 0.0f;								// �G�̒e�̎���
	m_fPAttack = 0.0f;									// �v���C���[�̍U����
	m_fEAttack = 0.0f;									// �G�̍U����
	m_fPDecrease = 0.0f;								// �v���C���[�̌����W��
	m_fEDecrease = 0.0f;								// �G�̌����W��
	m_fDistance = 0.0f;									// �I�u�W�F�N�g�ƒe�̋���
	m_fRadius = 0.0f;									// �e�̔��a
	m_bHit = false;										// ����������
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
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);
	m_Pos = pos;					// �ʒu
	m_InitPos = pos;				// �����ʒu
	m_fWidth = BULLET_WIDTH;		// ��
	m_fHeight = BULLET_HEIGHT;		// ����

	CObject2D::SetSize(m_fWidth,  m_fHeight);		// �T�C�Y�ƕ��̐ݒ�

	// �e�N�X�`���̐ݒ�
	if (m_nType == BULLET_PLAYER)
	{// �v���C���[�̒e
		CObject2D::SetTexture(CTexture::TEXTURE_PLAYERBULLET);
	}
	else if (m_nType == BULLET_ENEMY)
	{// �G�̒e
		CObject2D::SetTexture(CTexture::TEXTURE_ENEMYBULLET);
	}
	
	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CBullet::Update()
{
	CObject2D::Update();

	// �v���C���[�̏��̊i�[��
	float fPlayerLife = 0.0f;						// �̗�
	float fPlayerRad = 0.0f;						// ���a
	D3DXVECTOR3 PlayerPos = { 0.0f,0.0f,0.0f };		// �ʒu

	// �G�̏��̊i�[��
	float fEnemyLife = 0.0f;						// �̗�
	float fEnemyRad = 0.0f;							// ���a
	D3DXVECTOR3 EnemyPos = { 0.0f,0.0f,0.0f };		// �ʒu

	// �a�̏��̊i�[��
	float fExpLife = 0.0f;							// �̗�
	float fExpRad = 0.0f;							// ���a
	D3DXVECTOR3 ExpPos = { 0.0f,0.0f,0.0f };		// �ʒu

	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CObject *pObject;

			// �I�u�W�F�N�g���̎擾
			pObject = CObject::Getobject(nPri, nCnt);

			if (pObject != nullptr)
			{
				CObject::EObjType objType;
				objType = pObject->GetObjType();				// �I�u�W�F�N�g�̎�ނ̎擾

				// ���̎擾
				if (objType == OBJTYPE_PLAYER)
				{// �v���C���[�̏��̎擾
					fPlayerLife = pObject->GetLife();		// �̗�
					PlayerPos = pObject->GetPos();			// �ʒu
					fPlayerRad = pObject->GetRadius();		// ���a
				}
				else if (objType == OBJTYPE_ENEMY)
				{// �G�̏��̎擾
					fEnemyLife = pObject->GetLife();		// �̗�
					EnemyPos = pObject->GetPos();			// �ʒu
					fEnemyRad = pObject->GetRadius();		// ���a
				}
				else if (objType == OBJTYPE_EXP)
				{// �a�̏��̎擾
					ExpPos = pObject->GetPos();
					fExpRad = pObject->GetRadius();
					fExpLife = pObject->GetLife();
				}

				if (m_nType == BULLET_PLAYER)
				{
					// �G�ƒe�̋���
					m_fDistance = sqrtf((EnemyPos.x - m_Pos.x) * (EnemyPos.x - m_Pos.x)
						+ (EnemyPos.y - m_Pos.y) * (EnemyPos.y - m_Pos.y));

					// �G�ɒe����������
					if (fEnemyRad + m_fRadius >= m_fDistance
						&& m_bHit == false)
					{
						if (objType == OBJTYPE_ENEMY
							&& fEnemyLife > 0)
						{
							// �̗͂�0�ɂȂ�
							m_fPBulletLife = 0;
							m_PlayerShotMove.y = 0.0f;

							//�T�E���h�̍Đ�
							PlaySound(SOUND_LABEL_HIT_SE);

							fEnemyLife -= m_fPAttack;
							pObject->SetLife(0, m_fPAttack);
						}
					}
				}
				else if (m_nType == BULLET_ENEMY)
				{
					// �v���C���[�ƒe�̋���
					m_fDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
						+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

					// �v���C���[�ɒe����������
					if (fPlayerRad + m_fRadius >= m_fDistance
						&& m_bHit == false)
					{
						if (objType == OBJTYPE_PLAYER
							&& fPlayerLife > 0)
						{
							// �̗͂�0�ɂȂ�
							m_fEBulletLife = 0;
							m_EnemyShotMove.y = 0.0f;

							//�T�E���h�̍Đ�
							PlaySound(SOUND_LABEL_HIT_SE);

							pObject->SetLife(0, m_fEAttack);
						}
					}
				}

				// �a�ƒe�̋���
				float m_fExpDistance = sqrtf((ExpPos.x - m_Pos.x) * (ExpPos.x - m_Pos.x)
					+ (ExpPos.y - m_Pos.y) * (ExpPos.y - m_Pos.y));

				// �a�ɒe����������
				if (fExpRad + m_fRadius >= m_fExpDistance
					&& m_bHit == false)
				{
					if (m_nType == BULLET_PLAYER)
					{
						if (objType == OBJTYPE_EXP
							&& fExpLife > 0)
						{
							// �̗͂�0�ɂȂ�
							m_fPBulletLife = 0;
							m_PlayerShotMove.y = 0.0f;

							//�T�E���h�̍Đ�
							PlaySound(SOUND_LABEL_HIT_SE);

							// �_���[�W��^����
							pObject->SetLife(0, m_fPAttack);

							CExp::SetExpBreakType(CBullet::BULLET_PLAYER);	// �a���󂵂��e�̎��
						}
					}
					else if (m_nType == BULLET_ENEMY)
					{
						if (objType == OBJTYPE_EXP
							&& fExpLife > 0)
						{
							// �̗͂�0�ɂȂ�
							m_fEBulletLife = 0;
							m_EnemyShotMove.y = 0.0f;

							// �_���[�W��^����
							pObject->SetLife(0, m_fEAttack);

							CExp::SetExpBreakType(CBullet::BULLET_ENEMY);	// �a���󂵂��e�̎��
						}
					}
				}
			}
		}
	}

	D3DXVECTOR3 StopX = CPlayer::StopX(m_InitPos);
	D3DXVECTOR3 StopY = CPlayer::StopY(m_InitPos);

	m_Move = CPlayer::Move();

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_Move.x = 0.0f;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_Move.y = 0.0f;
	}

	bool Release = false;

	if (m_nType == BULLET_PLAYER)
	{// �v���C���[�̒e
		Release = BulletMove(&m_fPBulletLife, &m_PlayerShotMove, m_fPDecrease);
	}
	else if (m_nType == BULLET_ENEMY)
	{// �G�̒e
		Release = BulletMove(&m_fEBulletLife, &m_EnemyShotMove, m_fEDecrease);
	}

	if (Release == true)
	{
		Uninit();
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CBullet::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// �e�̐���
//==============================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, float rot, int type, float life, float speed, float size, float attack, float decrease)
{
		CBullet *pBullet = nullptr;

		pBullet = new CBullet;

		if (pBullet != nullptr)
		{
			pBullet->SetType(type);
			pBullet->Init(pos);
			pBullet->SetSize(size, size);
			pBullet->SetRad(size);
			pBullet->SetRot(rot);

			if (type == 1)
			{
				pBullet->SetPBulletLife(life);
				pBullet->SetPBulletSpeed(speed);
				pBullet->SetPBulletAttack(attack);
				pBullet->SetPDecrease(decrease);
			}
			else
			{
				pBullet->SetEBulletLife(life);
				pBullet->SetEBulletSpeed(speed);
				pBullet->SetEBulletAttack(attack);
				pBullet->SetEDecrease(decrease);
			}
		}

		return pBullet;
}

//==============================================================================================
// �e�̈ړ�����
//==============================================================================================
bool CBullet::BulletMove(float *BulletLife, D3DXVECTOR3 *ShotMove, float Decrease)
{
	m_Pos += m_Move;

	// �ʒu�̍X�V
	CObject2D::Setposition(m_Pos);

	bool bRelease = false;

	// �̗͂�0�ȏ�̏ꍇ
	if (*BulletLife > 0)
	{
		*BulletLife -= 1;

		// ���x�̌���
		ShotMove->y += (0.0f - ShotMove->y) * Decrease;

		D3DXVECTOR3 PosOld = m_Pos;

		// pos��move�����Z
		if (*BulletLife >= 0)
		{
			m_Pos.x += sinf(m_fRot) * -ShotMove->y;
			m_Pos.y += cosf(m_fRot) * -ShotMove->y;
		}
	}
	else
	{
		*ShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bHit = true;

		// �e��������
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);
	}
	
	return bRelease;
}