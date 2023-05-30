//==============================================================================================
//
// �G�@�@�@enemy.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include<time.h>
#include"enemy.h"
#include"player.h"
#include"hpgage.h"
#include"score.h"
#include"exp.h"
#include"fade.h"
#include"game.h"
#include"result.h"
#include"sound.h"
#include"statuslevel.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
int CEnemy::m_nNumEnemy = 0;
int CEnemy::m_nDeathCount = 0;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CEnemy::CEnemy() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e�����˂����ʒu
	m_fSize = 0.0f;									// �T�C�Y
	m_fWidth = 0.0f;								// ��
	m_fHeight = 0.0f;								// ����
	m_fSpeed = 0;									// �X�s�[�h
	m_fCoolTime = 0.0f;								// �e�̃N�[���^�C��
	m_fRadius = 0.0f;								// ���a
	m_fDamageCoolTime = 0.0f;						// �U�����󂯂���̃N�[���^�C��
	m_fTargetDistance = 0.0f;						// �^�[�Q�b�g�Ƃ̋���
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �^�[�Q�b�g�̈ʒu
	m_bNearPlayer = false;	// �v���C���[�ɋ߂���
	m_fLife = 0;			// �̗�
	m_fAttack = 0;			// �U����
	m_fBodyDamage = 0.0f;	// �{�̃_���[�W
	m_fBulletSpeed = 0;		// �e�̃X�s�[�h
	m_fBulletLife = 0.0f;	// �e�̗̑�
	m_nExpSave = 0;			// �o���l�̕ۑ���
	m_nLevel = 0;			// �G�̏������x��
	m_nLevelSave = 0;		// ���x���̕ۑ�
	m_nGiveExp = 0;			// �o���l
	m_nGrawLevel[3] = {};	// �i�����郌�x��
	m_bGrawUp = false;		// �i��������
	m_nLvUpCount = 0;		// ���x�����オ������
	m_bCreateHp = false;	// �̗̓Q�[�W���o����
	m_fHpAlpha = 0.0f;		// �̗̓Q�[�W�̃A���t�@�l
	m_fTargetCount = 0;		// �v���C���[��_������
	m_fHealLife = 0.0f;		// �����񕜗�
	m_nScore = 0;			// �X�R�A
	m_nNumEnemy++;			// �G�̐�

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{
		//�X�e�[�^�X���x��
		m_nStatuslevel[nCnt] = 0;
	}
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	m_Pos = pos;									// �ʒu
	m_InitPos = pos;								// �����ʒu
	m_fSize = ENEMY_SIZE;							// �T�C�Y
	m_fWidth = ENEMY_WIDTH;							// ��
	m_fHeight = ENEMY_HEIGHT;						// ����
	m_fRadius = ENEMY_RADIUS;						// ���a
	m_fTargetDistance = 10000.0f;					// �^�[�Q�b�g�Ƃ̋���
	m_fLife = ENEMY_DEFAULTLIFE;					// �̗�
	m_fMaxLife = ENEMY_DEFAULTMAXLIFE;				// �ő�̗�
	m_fBodyDamage = ENEMY_DEFAULTBODYDAMAGE;		// �{�̃_���[�W
	m_fBulletSpeed = ENEMY_BULLETSPEED;				// �e�̑��x
	m_fBulletLife = ENEMY_BULLETLIFE;				// �e�̗̑�
	m_fAttack = ENEMY_DEFAULTATTACK;				// �U����
	m_fReload = ENEMY_DEFAULTRELOATD;				// �����[�h���x
	m_fSpeed = ENEMY_DEFAULTSPEED;					// �ړ��X�s�[�h
	m_fBulletDecrease = BULLET_DEFAULTDECREASE;		// �e�̌����W��
	m_nLevelSave = 1;								// ���x���̕ۑ�
	m_nGiveExp = 40;								// �o���l
	m_nGrawLevel[0] = 5;							// �i�����郌�x��1
	m_nGrawLevel[1] = 20;							// �i�����郌�x��2
	m_nGrawLevel[2] = 30;							// �i�����郌�x��3
	m_EnemyType = ENEMY_TYPE0;						// �v���C���[�̃^�C�v
	m_fTargetCount = TARGET_COUNT;					// �v���C���[��_������
	m_nScore = 0;									// �X�R�A

	CObject2D::Init(pos);
	CObject2D::SetSize(m_fWidth, m_fHeight);
	CObject2D::SetTexture(CTexture::TEXTURE_ENEMY0);	//�e�N�X�`���̐ݒ�
	
	// �̗̓Q�[�W�̐���
	m_pHpgage = CHpGage::Create({ m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z }, 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{
		//�X�e�[�^�X���x��
		m_nStatuslevel[nCnt] = 1;
	}

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CEnemy::Update()
{
	CObject2D::Update();

	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);			// �e�����˂����ʒu

	m_BulletPos.x += sinf(m_fAngle) * -PLAYER_BULLETPOS;
	m_BulletPos.y += cosf(m_fAngle) * -PLAYER_BULLETPOS;

	// �ړ��ʂ̌���
	m_Move.x += (0.0f - m_Move.x) * ENEMY_DECREASE;
	m_Move.y += (0.0f - m_Move.y) * ENEMY_DECREASE;

	D3DXVECTOR3 StopX = CPlayer::StopX(m_InitPos);
	D3DXVECTOR3 StopY = CPlayer::StopY(m_InitPos);

	m_PlayerMove = CPlayer::Move();

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_PlayerMove.x = 0.0f;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_PlayerMove.y = 0.0f;
	}

	m_TotalMove = m_PlayerMove + m_Move;

	//�ړ��ʂ̍X�V
	m_Pos += m_TotalMove;

	CObject2D::Setposition(m_Pos);

	CFade::FADE FadeMode = CFade::GetFade();
	CApplication::MODE GameMode = CApplication::GetMode();

	if (FadeMode == CFade::FADE_NONE
		&& GameMode == CApplication::MODE_GAME)
	{
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
		float PlayerRad = CGame::GetPlayer()->GetRadius();
		float PlayerBodyDamage = CGame::GetPlayer()->GetBodyDamage();

		// �G�ƃv���C���[�̋���
		float m_fDistance = sqrtf((m_Pos.x - PlayerPos.x) * (m_Pos.x - PlayerPos.x)
			+ (m_Pos.y - PlayerPos.y) * (m_Pos.y - PlayerPos.y));

		m_fDamageCoolTime--;

		// �G�ƃv���C���[����������
		if (m_fDistance <= PlayerRad + m_fRadius
			&& m_fDamageCoolTime <= 0)
		{
			SetLife(0, PlayerBodyDamage);
			CGame::GetPlayer()->SetLife(0, m_fBodyDamage);

			m_fDamageCoolTime = ENEMY_DAMAGECOOLTIME;
		}

		if (m_fDistance < 500)
		{
			m_bNearPlayer = true;
		}
		else
		{
			m_bNearPlayer = false;
		}
	}

	bool bRelease = false;

	if (m_fLife > 0)
	{
		// �G�̋���
		Move();

		if (m_fMaxLife > m_fLife && m_fHealLife > 0)
		{
			// �̗͂̎�����
			SetLife(1, m_fHealLife);

			m_nHpCoolTime = HP_COOLTIME;

			if (m_fMaxLife <= m_fLife)
			{
				m_fLife = m_fMaxLife;
			}
		}

		// �ړI�̊p�x�̐ݒ�
		m_fAngle = (float)atan2(m_Pos.x - m_TargetPos.x, m_Pos.y - m_TargetPos.y);

		CObject2D::Setrot(m_fAngle);

		// �e��������
		Shot();
	}
	else
	{
		// �G��������
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);

		m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (bRelease == false)
	{
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
		for (int nPri = 0; nPri < MAX_PRI; nPri++)
		{
			for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
			{
				CObject *pObject;
				pObject = CObject::Getobject(nPri, nCnt);

				if (pObject != nullptr)
				{
					CObject::EObjType objType;
					objType = pObject->GetObjType();		// �I�u�W�F�N�g�̎��

					if (objType == OBJTYPE_PLAYER)
					{// �v���C���[�Ɍo���l��n��
						// �o���l�̐ݒ�
						m_nGiveExp += m_nLevel;

						pObject->SetExp(m_nGiveExp);
						m_nScore += 50;

						// �X�R�A���Z
						CGame::GetScore()->AddScore(m_nScore);

						Uninit();
						m_nNumEnemy--;			// �G�̐�
						m_nDeathCount++;		// �|������

						break;
					}
				}
			}
		}
	}

	// ���x���A�b�v
	LevelUp();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CEnemy::Draw()
{
	CObject2D::Draw();

#ifdef _DEBUG
	m_Font = CRenderer::GetFont();

	//Enemy(m_Font);
#endif // _DEBUG
}

//==============================================================================================
// �G�̐���
//==============================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int Level)
{
	CEnemy *pEnemy = nullptr;

	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos);
		pEnemy->SetLevel(Level);
	}

	return pEnemy;
}

//==============================================================================================
//�ړ�����
//==============================================================================================
void CEnemy::Move()
{
	// �v���C���[�Ɖa�̈ʒu
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 TrianglePos;
	D3DXVECTOR3 BoxPos;
	D3DXVECTOR3 PentagonPos;

	// �v���C���[�A�a�Ƃ̋���
	float fPlayerDistance = 0.0f;
	float fTriangleDistance = 0.0f;
	float fBoxDistance = 0.0f;
	float fPentagonDistance = 0.0f;

	// �a�̗̑�
	float fPlayerLife = 0.0f;
	float fTrinagleLife = 0.0f;
	float fBoxLife = 0.0f;
	float fPentagonLife = 0.0f;

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

				if (objType == OBJTYPE_PLAYER)
				{// �v���C���[�̏��̎擾
					PlayerPos = pObject->GetPos();				// �ʒu
					fPlayerLife = pObject->GetLife();			// �̗�

					// �G�ƃv���C���[�̋���
					fPlayerDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
						+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

					if (m_fTargetDistance > fPlayerDistance)
					{// �v���C���[
						m_fTargetDistance = fPlayerDistance;
						m_TargetPos = PlayerPos;
					}

					if (fPlayerDistance >= 300.0f + 100.0f
						|| fPlayerLife <= 0)
					{
						m_fTargetDistance = 10000.0f;
					}
				}
				else if (objType == OBJTYPE_EXP)
				{
					CApplication::MODE GameMode = CApplication::GetMode();
					CExp::Exptype ExpType = CExp::EXPTYPE_NONE;

					if (GameMode == CApplication::MODE_GAME)
					{
						ExpType = CGame::GetExp()->GetExpType();
					}
					else
					{
						ExpType = CResult::GetExp()->GetExpType();
					}

					if (ExpType == CExp::EXPTYPE_TRIANGLE)
					{// �O�p�̏ꍇ
						int nNum = pObject->GetNumTriangle();	// �O�p�̐�
						TrianglePos = pObject->GetPos();		// �ʒu
						fTrinagleLife = pObject->GetLife();		// �̗�

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// �G�ƎO�p�a�̋���
							fTriangleDistance = sqrtf((TrianglePos.x - m_Pos.x) * (TrianglePos.x - m_Pos.x)
								+ (TrianglePos.y - m_Pos.y) * (TrianglePos.y - m_Pos.y));

							if (m_fTargetDistance > fTriangleDistance)
							{// �O�p�a
								fTrinagleLife = pObject->GetLife();		// �̗͂̎擾
								m_fTargetDistance = fTriangleDistance;
								m_TargetPos = TrianglePos;
							}

							if (fTrinagleLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
					else if (ExpType == CExp::EXPTYPE_BOX)
					{// �l�p�̏ꍇ
						int nNum = pObject->GetNumBox();		// �l�p�̐�
						BoxPos = pObject->GetPos();				// �ʒu
						fBoxLife = pObject->GetLife();			// �̗�

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// �G�Ǝl�p�a�̋���
							fBoxDistance = sqrtf((BoxPos.x - m_Pos.x) * (BoxPos.x - m_Pos.x)
								+ (BoxPos.y - m_Pos.y) * (BoxPos.y - m_Pos.y));

							if (m_fTargetDistance > fBoxDistance)
							{// �l�p�a
								fBoxLife = pObject->GetLife();		// �̗͂̎擾
								m_fTargetDistance = fBoxDistance;
								m_TargetPos = BoxPos;
							}

							if (fBoxLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
					else if (ExpType == CExp::EXPTYPE_PENTAGON)
					{// �܊p�`�̏ꍇ
						int nNum = pObject->GetNumPentagon();	// �܊p�`�̐�
						PentagonPos = pObject->GetPos();		// �ʒu
						fPentagonLife = pObject->GetLife();		// �̗�

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// �G�ƌ܊p�`�a�̋���
							fPentagonDistance = sqrtf((PentagonPos.x - m_Pos.x) * (PentagonPos.x - m_Pos.x)
								+ (PentagonPos.y - m_Pos.y) * (PentagonPos.y - m_Pos.y));

							if (m_fTargetDistance > fPentagonDistance)
							{// �܊p�`
								fPentagonLife = pObject->GetLife();		// �̗͂̎擾
								m_fTargetDistance = fPentagonDistance;
								m_TargetPos = PentagonPos;
							}

							if (fPentagonLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
				}
			}
		}
	}

	// �^�[�Q�b�g�Ƃ̋���������Ă���ꍇ
	if (m_fTargetDistance >= 300.0f
		&& m_EnemyType != ENEMY_TYPE8)
	{
		// �^�[�Q�b�g�ɋ߂Â�
		m_Move.x = sinf(m_fAngle) * -m_fSpeed;
		m_Move.y = cosf(m_fAngle) * -m_fSpeed;
	}
	else if (m_fTargetDistance >= 30.0f
		&& m_EnemyType == ENEMY_TYPE8)
	{
		// �^�[�Q�b�g�ɋ߂Â�
		m_Move.x = sinf(m_fAngle) * -m_fSpeed;
		m_Move.y = cosf(m_fAngle) * -m_fSpeed;
	}

	if (m_bHit == true)
	{
		m_fTargetDistance = fPlayerDistance;
		m_TargetPos = PlayerPos;

		if (m_fTargetDistance >= 300.0f)
		{
			m_Move.x = sinf(m_fAngle) * -m_fSpeed;
			m_Move.y = cosf(m_fAngle) * -m_fSpeed;
		}
		
		m_fTargetCount--;

		if(m_fTargetCount <= 0)
		{
			m_bHit = false;
			m_fTargetCount = TARGET_COUNT;
		}
	}
}

//==============================================================================================
// �e��������
//==============================================================================================
void CEnemy::Shot()
{
	m_fCoolTime--;

	if (m_fCoolTime <= 0.0f)
	{
		if (m_bNearPlayer == true)
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SHOT_SE);
		}

		// �e������
		if (m_EnemyType == ENEMY_TYPE0)
		{// �v���C���[�^�C�v�O�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE1)
		{// �v���C���[�^�C�v�P�̏ꍇ
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI / 2) * 10.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI / 2) * 10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI / 2) * -10.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI / 2) * -10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE2)
		{// �v���C���[�^�C�v�Q�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.85f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.85f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE3)
		{// �v���C���[�^�C�v�R�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed * 1.8f, BULLET_SIZE - 5.0f, m_fAttack * 0.7f, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE4)
		{// �v���C���[�^�C�v�S�̏ꍇ		
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.y - cosf(m_fAngle - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE5)
		{// �v���C���[�^�C�v�T�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE6)
		{// �v���C���[�^�C�v�U�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE + 7.0f, m_fAttack * 1.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 20.0f;
		}
		else if (m_EnemyType == ENEMY_TYPE7)
		{// �v���C���[�^�C�v�V�̏ꍇ
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI) * 50.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI) * 50.0f, m_BulletPos.z), m_fAngle + D3DX_PI, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.5f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.5f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.75f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.75f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE8)
		{// �v���C���[�^�C�v�W�̏ꍇ
		}
		else if (m_EnemyType == ENEMY_TYPE9)
		{// �v���C���[�^�C�v�X�̏ꍇ
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle) * -10.0f, m_BulletPos.y + cosf(m_fAngle) * -10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed + 1.0f, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.05f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.05f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.1f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.1f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.15f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.15f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.25f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.25f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 20.0f;
		}

		if (m_EnemyType != ENEMY_TYPE6
			&& m_EnemyType != ENEMY_TYPE9)
		{
			m_fCoolTime = m_fReload;
		}
	}
}

//==============================================================================================
// ���x���A�b�v����
//==============================================================================================
void CEnemy::LevelUp()
{
	srand((unsigned int)time(0));

	if (m_nLvUpCount + 1 < m_nLevel)
	{
		int nStatus = rand() % 8 + 1;

		// �X�e�[�^�X�̋���
		if (nStatus == 1)
		{// �̗͂̍ő�lUP
			m_nStatuslevel[0]++;

			if (m_nStatuslevel[0] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				float fOldLife = m_fMaxLife;

				m_fMaxLife += 5;

				float fOldDamagePercent;
				float fDamagePercent;

				// �󂯂��_���[�W�̊����̌v�Z
				fOldDamagePercent = (m_fTotalDamage / fOldLife) * 100;
				fDamagePercent = (m_fTotalDamage / m_fMaxLife) * 100;

				float nDamage = fOldDamagePercent - fDamagePercent;

				float fLife = m_fMaxLife - fOldLife;

				// �̗̓Q�[�W�𑝂₷
				m_pHpgage->AddHpgage(nDamage);

				// �̗͂𑝂₷
				m_fLife += fLife;

				m_fHpAlpha = 1.0f;
				m_nHpCoolTime = HP_COOLTIME;
				m_bCreateHp = true;

				m_nScore += 5;
			}
		}
		else if (nStatus == 2)
		{// �����񕜗�UP
			m_nStatuslevel[1]++;

			if (m_nStatuslevel[1] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fHealLife += m_fMaxLife * 0.0002f;

				m_nScore += 3;
			}
		}
		else if (nStatus == 3)
		{// �{�̂̍U����UP
			m_nStatuslevel[2]++;

			if (m_nStatuslevel[2] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBodyDamage += 4;

				m_nScore += 5;
			}
		}
		else if (nStatus == 4)
		{// �e�̑��xUP
			m_nStatuslevel[3]++;

			if (m_nStatuslevel[3] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBulletSpeed++;

				m_nScore += 3;
			}
		}
		else if (nStatus == 5)
		{// �e�̗̑�
			m_nStatuslevel[4]++;

			if (m_nStatuslevel[4] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBulletLife += 50.0f;
				m_fBulletDecrease -= 0.001f;

				m_nScore += 3;
			}
		}
		else if (nStatus == 6)
		{// �U����UP
			m_nStatuslevel[5]++;

			if (m_nStatuslevel[5] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fAttack += 2;

				m_nScore += 5;
			}
		}
		else if (nStatus == 7)
		{// �����[�h���xUP
			m_nStatuslevel[6]++;

			if (m_nStatuslevel[6] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fReload -= 2.0f;

				m_nScore += 5;
			}
		}
		else if (nStatus == 8)
		{// �ړ����xUP
			m_nStatuslevel[7]++;

			if (m_nStatuslevel[7] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fSpeed += 0.2f;

				m_nScore += 3;
			}
		}
	}

	int evolution = rand() % 10 + 1;

	// �i���P���
	if (m_nLvUpCount >= m_nGrawLevel[0]
		&& m_bGrawUp == false)
	{
		if (evolution == 1)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY1);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE1;
		}
		else if (evolution == 2)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY2);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE2;
		}
		else if (evolution == 3)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY3);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE3;
		}
		else if (evolution == 4)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY4);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE4;
		}
		else if (evolution == 5)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY5);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE5;
		}
		else if (evolution == 6)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY6);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE6;

			m_fSpeed /= 1.5;
		}
		else if (evolution == 7)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY7);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE7;
		}
		else if (evolution == 8)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY8);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE8;

			m_fHealLife += 0.5f;
			m_fBodyDamage += 10;
		}
		else if (evolution == 9)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY9);	//�e�N�X�`���̐ݒ�
			m_EnemyType = ENEMY_TYPE9;
		}

		m_bGrawUp = true;
	}
}

//==============================================================================================
// �̗̓Q�[�W�����鏈��
//==============================================================================================
void CEnemy::HitDamage(float damage)
{
	float fDamagePercent;

	// �󂯂��_���[�W�̊����̌v�Z
	fDamagePercent = (damage / m_fMaxLife) * 100;

	// �̗̓Q�[�W�����炷
	m_pHpgage->SubHpgage(fDamagePercent);
}

//==============================================================================================
// �̗̓Q�[�W�������鏈��
//==============================================================================================
void CEnemy::HealHp(float heal)
{
	float fHealPercent;

	// �񕜗ʂ̊����̌v�Z
	fHealPercent = (heal / m_fMaxLife) * 100;

	// �̗̓Q�[�W�𑝂₷
	m_pHpgage->AddHpgage(fHealPercent);
}

//==============================================================================================
// �̗͂̐ݒ�
//==============================================================================================
void CEnemy::SetLife(int type, float life)
{
	CApplication::MODE GameMode = CApplication::GetMode();

	if (GameMode == CApplication::MODE_GAME)
	{
		if (type == 0)
		{// �̗͂�����
			float fLife = m_fLife;

			m_fLife -= life;		// �̗͂����炷
			m_fTotalDamage += life;	// �󂯂��_���[�W�̍��v

			if (fLife > 0)
			{
				// �_���[�W��^���鏈��
				HitDamage(life);

				m_bHit = true;
			}
		}
		else
		{
			if (m_fLife < m_fMaxLife)
			{
				// �̗͂��񕜂���
				m_fLife += life;

				// �񕜂��鏈��
				HealHp(life);
			}
		}

		m_fHpAlpha = 1.0f;
		m_nHpCoolTime = HP_COOLTIME;
		m_bCreateHp = true;
	}
}

//==============================================================================================
// ���x���̐ݒ�
//==============================================================================================
void CEnemy::SetLevel(int Level)
{
	m_nLevel = Level;
}

void CEnemy::Enemy(LPD3DXFONT pFont)
{
	RECT rect;
	TCHAR str[256];

	for (int nCnt = 0; nCnt < 9; nCnt++)
	{
		rect = { 0, 0 + nCnt * 20, 1280, 720 };

		if (nCnt == 0)
		{
			sprintf(str, ("�ő�̗́@%.1f"), m_fMaxLife);
		}
		else if (nCnt == 1)
		{
			sprintf(str, ("�񕜗ʁ@%.1f"), m_fHealLife);
		}
		else if (nCnt == 2)
		{
			sprintf(str, ("�{�̃_���[�W�@%.1f"), m_fBodyDamage);
		}
		else if (nCnt == 3)
		{
			sprintf(str, ("�e�̑��x�@%.1f"), m_fBulletSpeed);
		}
		else if (nCnt == 4)
		{
			sprintf(str, ("�e�̗̑́@%.1f"), m_fBulletLife);
		}
		else if (nCnt == 5)
		{
			sprintf(str, ("�U���́@%.1f"), m_fAttack);
		}
		else if (nCnt == 6)
		{
			sprintf(str, ("�����[�h�@%.1f"), m_fReload);
		}
		else if (nCnt == 7)
		{
			sprintf(str, ("�ړ����x�@%.1f"), m_fSpeed);
		}
		else if (nCnt == 8)
		{
			sprintf(str, ("���x����������񐔁@%d"), m_nLevel);
		}
		// �e�L�X�g�`��
		pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));
	}
}