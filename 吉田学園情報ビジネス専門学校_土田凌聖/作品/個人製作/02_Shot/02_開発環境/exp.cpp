//==============================================================================================
//
// �a�@�@�@exp.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"exp.h"
#include"player.h"
#include"hpgage.h"
#include"score.h"
#include"game.h"
#include"fade.h"
#include"sound.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
D3DXVECTOR3 CExp::m_CreatePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CExp::m_nNumExp = 0;
int CExp::m_nNumTriangle = 0;
int CExp::m_nNumBox = 0;
int CExp::m_nNumPentagon = 0;
CBullet::BULLET_TYPE CExp::m_BulletType = CBullet::BULLET_NONE;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CExp::CExp() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ʒu
	m_fSize = 0.0f;					// �T�C�Y
	m_fWidth = 0.0f;				// ��
	m_fHeight = 0.0f;				// ����
	m_nType = EXPTYPE_NONE;			// ���
	m_fLife = 0;					// �̗�
	m_fRadius = 0.0f;				// ���a
	m_nExp = 0;						// �o���l
	m_fBodyDamage = 0;				// �{�̃_���[�W
	m_fDamageCoolTime = 0;			// �U�������������̃N�[���^�C��
	m_bDamage = false;				// �U�������������
	m_bCreateHp = false;			// �̗̓Q�[�W���o����
	m_nHpCoolTime = 0;				// �̗̓Q�[�W��������܂ł̎���
	m_fHpAlpha = 0.0f;				// �̗̓Q�[�W�̃A���t�@�l
	m_fHpGage = 0.0f;				// �̗̓Q�[�W�̒���
	m_nBodyDamageType = 0;			// �v���C���[�ƓG�ǂ����ɓ���������
	m_nNumExp++;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CExp::~CExp()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CExp::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	m_Pos = pos;								// �ʒu
	m_fRadius = EXP_RADIUS;						// ���a
	m_nExp = 0;									// �o���l
	m_fBodyDamage = EXP_BODYDAMAGE;				// �{�̃_���[�W
	m_fDamageCoolTime = EXP_DAMAGECOOLTIME;		// �U�������������̃N�[���^�C��
	m_bDamage = false;							// �U�������������
	m_fWidth = EXP_WIDTH;						// ��
	m_fHeight = EXP_HEIGHT;						// ����
	m_nHpCoolTime = HP_COOLTIME;				// �̗̓Q�[�W��������܂ł̎���
	m_fHpGage = HP_WIDTH;						// �̗̓Q�[�W�̒���

	// �̗̓Q�[�W�̐���
	m_pHpgage = CHpGage::Create(D3DXVECTOR3(m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z), 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	if (m_nType == EXPTYPE_TRIANGLE)
	{// �O�p
		m_fLife = EXP_TRIANGLELIFE;								// �̗͂̐ݒ�
		m_fMaxLife = EXP_TRIANGLELIFE;							// �ő�̗͂̐ݒ�
		m_fSize = EXP_TRIANGLESIZE;								// �T�C�Y�̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_EXTRIANGLE);	// �e�N�X�`���̐ݒ�
		m_nNumTriangle++;										// ��
	}
	else if (m_nType == EXPTYPE_BOX)
	{// �l�p
		m_fLife = EXP_BOXLIFE;									// �̗͂̐ݒ�
		m_fMaxLife = EXP_BOXLIFE;								// �ő�̗͂̐ݒ�
		m_fSize = EXP_BOXSIZE;									// �T�C�Y�̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_EXBOX);			// �e�N�X�`���̐ݒ�
		m_nNumBox++;											// ��
	}
	else if (m_nType == EXPTYPE_PENTAGON)
	{// �܊p�`
		m_fLife = EXP_PENTAGONLIFE;								// �̗͂̐ݒ�
		m_fMaxLife = EXP_PENTAGONLIFE;							// �ő�̗͂̐ݒ�
		m_fSize = EXP_PENTAGONSIZE;								// �T�C�Y�̐ݒ�
		CObject2D::SetTexture(CTexture::TEXTURE_EXPENTAGON);	// �e�N�X�`���̐ݒ�
		m_nNumPentagon++;										// ��
	}

	CObject2D::SetSize(m_fSize, m_fSize);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CExp::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CExp::Update()
{
	CObject2D::Update();

	// �v���C���[�̏��̊i�[��
	D3DXVECTOR3 PlayerPos = { 0.0f,0.0f,0.0f };		// �ʒu
	float fPlayerSize = 0.0f;						// �T�C�Y
	float fPlayerBodyDamage = 0.0f;					// �{�̍U����
	float fPlayerDistance = 0.0f;					// ����
	float fPlayerAngle = 0.0f;						// �p�x

	// �G�̏��̊i�[��
	D3DXVECTOR3 EnemyPos = { 0.0f,0.0f,0.0f };		// �ʒu
	float fEnemySize = 0.0f;						// �T�C�Y
	float fEnemyBodyDamage = 0.0f;					// �{�̍U����
	float fEnemyDistance = 0.0f;					// ����
	float fEnemyAngle = 0.0f;						// �p�x

	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CObject *pObject;
			pObject = CObject::Getobject(nPri, nCnt);

			if (pObject != nullptr)
			{
				CObject::EObjType objType;
				objType = pObject->GetObjType();			// �I�u�W�F�N�g�̎��

				if (objType == OBJTYPE_PLAYER)
				{
					PlayerPos = pObject->GetPos();					// �v���C���[�̈ʒu
					fPlayerSize = pObject->GetSize();				// �v���C���[�̃T�C�Y
					fPlayerBodyDamage = pObject->GetBodyDamage();	// �v���C���[�̖{�̍U����

					if (m_fLife >= 0)
					{
						// �v���C���[�Ɖa�̋���
						fPlayerDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
							+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

						// �����蔻��
						if (m_fSize + fPlayerSize >= fPlayerDistance)
						{
							// �v���C���[�Ɖa�̊p�x
							fPlayerAngle = (float)atan2(m_Pos.x - PlayerPos.x, m_Pos.y - PlayerPos.y);

							// �v���C���[�ɉ����o�����
							m_PushMove.x = sinf(fPlayerAngle);
							m_PushMove.y = cosf(fPlayerAngle);

							if (m_bDamage == false)
							{
								//�T�E���h�̍Đ�
								PlaySound(SOUND_LABEL_HIT_SE);

								// �a�̗̑͂̐ݒ�
								SetLife(0, fPlayerBodyDamage);

								// �v���C���[�ɓ�������
								m_nBodyDamageType = 1;

								// �v���C���[�̗̑͂̐ݒ�
								pObject->SetLife(0, EXP_BODYDAMAGE);
							}

							m_bDamage = true;
						}
					}
				}
				else if (objType == OBJTYPE_ENEMY)
				{
					EnemyPos = pObject->GetPos();					// �G�̈ʒu
					fEnemySize = pObject->GetSize();				// �G�̃T�C�Y
					fEnemyBodyDamage = pObject->GetBodyDamage();	// �G�̖{�̍U����

					if (m_fLife >= 0)
					{
						// �G�Ɖa�̋���
						fEnemyDistance = sqrtf((EnemyPos.x - m_Pos.x) * (EnemyPos.x - m_Pos.x)
							+ (EnemyPos.y - m_Pos.y) * (EnemyPos.y - m_Pos.y));

						if (m_fSize + fEnemySize >= fEnemyDistance)
						{
							// �G�Ɖa�̊p�x
							fEnemyAngle = (float)atan2(m_Pos.x - EnemyPos.x, m_Pos.y - EnemyPos.y);

							// �G�ɉ����o�����
							m_PushMove.x = sinf(fEnemyAngle);
							m_PushMove.y = cosf(fEnemyAngle);

							if (m_bDamage == false)
							{
								// �a�̗̑͂̐ݒ�
								SetLife(0, fEnemyBodyDamage);

								// �G�ɓ�������
								m_nBodyDamageType = 2;

								// �G�̗̑͂̐ݒ�
								pObject->SetLife(0, EXP_BODYDAMAGE);
							}

							m_bDamage = true;
						}
					}
				}
			}
		}
	}

	bool bRelease = false;

	if (m_fLife > 0)
	{
		if (m_bDamage == true)
		{
			m_fDamageCoolTime--;

			if (m_fDamageCoolTime <= 0)
			{
				m_fDamageCoolTime = EXP_DAMAGECOOLTIME;
				m_bDamage = false;
			}
		}

		// �ړ��ʂ̌���
		m_PushMove.x += (0.0f - m_PushMove.x) * EXP_DECREASE;
		m_PushMove.y += (0.0f - m_PushMove.y) * EXP_DECREASE;

		m_Pos += m_PushMove;

		if (m_pHpgage != nullptr)
		{
			if (m_bCreateHp == true)
			{
				// �̗̓Q�[�W��������܂ł̎���
				m_nHpCoolTime--;

				if (m_nHpCoolTime <= 0 && m_fLife >= m_fMaxLife)
				{
					m_fHpAlpha = 0.0f;		// ��莞�Ԍ�̗̓Q�[�W�𓧖��ɂ���
					m_bCreateHp = false;
					m_nHpCoolTime = HP_COOLTIME;
				}
			}
			// �̗̓Q�[�W�̈ʒu
			m_pHpgage->SetPos(D3DXVECTOR3(m_Pos.x - 18.0f, m_Pos.y + 30, m_Pos.z));

			// �̗̓Q�[�W�̐F�̐ݒ�
			m_pHpgage->SetCol({ 0.0f,1.0f,0.0f, m_fHpAlpha });
		}
	}
	else
	{
		// �a��������
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);

		m_PushMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (bRelease == true)
	{
		// �o���l�̐ݒ�
		if (m_nType == EXPTYPE_TRIANGLE)
		{// �O�p
			m_nExp = 15;
			m_nScore = 20;
		}
		else if (m_nType == EXPTYPE_BOX)
		{// �l�p
			m_nExp = 10;
			m_nScore = 10;
		}
		else if (m_nType == EXPTYPE_PENTAGON)
		{// �܊p�`
			m_nExp = 20;
			m_nScore = 30;
		}

		if (m_nBodyDamageType == 1
			|| m_BulletType == CBullet::BULLET_PLAYER)
		{
			// �v���C���[�Ɍo���l��n��
			CGame::GetPlayer()->SetExp(m_nExp);

			// �X�R�A���Z
			CGame::GetScore()->AddScore(m_nScore);
		}

		m_nNumExp--;

		// ���
		Uninit();
	}

	CFade::FADE FadeMode = CFade::GetFade();
	CApplication::MODE GameMode = CApplication::GetMode();

	if (FadeMode == CFade::FADE_NONE
		&& GameMode == CApplication::MODE_GAME)
	{
		float PLife = CGame::GetPlayer()->GetLife();

		if (PLife <= 0)
		{
			m_nNumExp = 0;
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

	m_TotalMove = m_PushMove + m_Move;

	// �ʒu�̍X�V
	m_Pos += m_TotalMove;

	// �ʒu�̐ݒ�
	CObject2D::Setposition(m_Pos);
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CExp::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// �a�̐���
//==============================================================================================
CExp *CExp::Create(D3DXVECTOR3 pos, int type, float rot)
{
	CExp *pExp = nullptr;

	pExp = new CExp;

	if (pExp != nullptr)
	{
		pExp->SetType(type);
		pExp->Init(pos);
		pExp->Setrot(rot);
	}

	return pExp;
}

//==============================================================================================
// �̗͂̐ݒ�
//==============================================================================================
void CExp::SetLife(int type, float life)
{
	float fLife = 0;

	if (type == 0)
	{// �̗͂�����
		fLife = m_fLife;
		m_fLife -= life;		// �̗͂����炷
	}
	else
	{// �̗͂�������
		m_fLife += life;		// �̗͂��񕜂���
	}

	m_fHpAlpha = 1.0f;
	m_nHpCoolTime = HP_COOLTIME;
	m_bCreateHp = true;

	if (fLife > 0)
	{
		HitDamage(life);
	}
}

//==============================================================================================
// �󂯂��_���[�W�̊����̎擾
//==============================================================================================
float CExp::HitDamage(float damage)
{
	float fDamagePercent = 0;

	// �󂯂��_���[�W�̊����̌v�Z
	if (m_nType == EXPTYPE_TRIANGLE)
	{// �O�p
		fDamagePercent = (damage / EXP_TRIANGLELIFE) * 100;
	}
	if (m_nType == EXPTYPE_BOX)
	{// �l�p
		fDamagePercent = (damage / EXP_BOXLIFE) * 100;
	}
	if (m_nType == EXPTYPE_PENTAGON)
	{// �܊p�`
		fDamagePercent = (damage / EXP_PENTAGONLIFE) * 100;
	}

	// �̗̓Q�[�W�����炷
	m_pHpgage->SubHpgage(fDamagePercent);

	return fDamagePercent;
}

//==============================================================================================
// �o���l�̎�ނ̎擾
//==============================================================================================
CExp::Exptype CExp::GetExpType()
{
	if (m_nType == EXPTYPE_TRIANGLE)
	{// �O�p
		return CExp::EXPTYPE_TRIANGLE;
	}
	else if (m_nType == EXPTYPE_BOX)
	{// �l�p
		return CExp::EXPTYPE_BOX;
	}
	else if (m_nType == EXPTYPE_PENTAGON)
	{// �܊p�`
		return CExp::EXPTYPE_PENTAGON;
	}
	else
	{
		return CExp::EXPTYPE_NONE;
	}
}