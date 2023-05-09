//==============================================================================================
//
// �v���C���[�@�@�@player.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"fade.h"
#include"player.h"
#include"inputkeyboard.h"
#include"bullet.h"
#include"hpgage.h"
#include"expgage.h"
#include"statusdisplay.h"
#include"statuslevel.h"
#include"evodisplay.h"
#include"leveldisplay.h"
#include"game.h"
#include"sound.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
float CPlayer::m_fLife = 0.0f;										// �̗�
D3DXVECTOR3 CPlayer::m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
D3DXVECTOR3 CPlayer::m_BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e�����˂����ʒu
float CPlayer::m_fRot = 0.0f;										// �p�x
float CPlayer::m_fCoolTime = 0.0f;									// �e�����˂����܂ł̎���
D3DXVECTOR3 CPlayer::m_ZeroPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 0�̈ʒu
float CPlayer::m_fSpeed = 0.0f;										// �ړ����x
bool CPlayer::m_bCollisionX = false;
bool CPlayer::m_bCollisionY = false;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CPlayer::CPlayer() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_fSize = 0.0f;			// �T�C�Y
	m_fWidth = 0.0f;		// ��
	m_fHeight = 0.0f;		// ����
	m_fRadius = 0.0f;		// �����蔻��̔��a
	m_fAttack = 0;			// �_���[�W
	m_fBodyDamage = 0.0f;	// �{�̃_���[�W
	m_fMaxLife = 0;			// �̗͂̍ő�l
	m_fBulletSpeed = 0;		// �e�̑��x
	m_fBulletLife = 0;		// �e�̗̑�
	m_nExp = 0;				// �o���l
	m_nLevelUpExp = 0;		// ���x���A�b�v�ɕK�v�Ȍo���l
	m_nLevel = 0;			// �v���C���[�̃��x��
	m_nCntLevel = 0;		// ���x�����オ������
	m_nLevelSave = 0;		// ���x���̕ۑ�
	m_nGrawLevel[3] = {};	// �i�����x��
	m_nGrawCount = 0;		// �i��������
	m_bLevelUp = false;		// ���x�����オ������
	m_bCreateHp = false;	// �̗̓Q�[�W���o����
	m_nHpCoolTime = 0;		// �̗̓Q�[�W��������܂ł̎���
	m_fHpAlpha = 0.0f;		// �̗̓Q�[�W�̃A���t�@�l
	m_fStatusAlpha = 0.0f;	// �X�e�[�^�X�\���̃A���t�@�l
	m_fEvoAlpha = 0.0f;		// �i����\���̃A���t�@�l
	m_PlayerType = PLAYER_NONE;	// �v���C���[�^�C�v

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{// �X�e�[�^�X���x���̕\��
		m_pStatuslevel[nCnt] = nullptr;

		//�X�e�[�^�X���x��
		m_nStatuslevel[nCnt] = 0;
	}
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
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);		// �ʒu
	m_ZeroPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 0�̈ʒu
	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y - 100, 0.0f);			// �e�����˂����ʒu
	m_fRot = 0.0f;								// �p�x
	m_fSize = PLAYER_SIZE;						// �T�C�Y
	m_fWidth = PLAYER_WIDTH;					// ��
	m_fHeight = PLAYER_HEIGHT;					// ����
	m_fCoolTime = 0;							// �e�����˂����܂ł̎���
	m_fRadius = PLAYER_RADIUS;					// �����蔻��̔��a
	m_fAttack = PLAYER_DEFAULTATTACK;			// �v���C���[�̍U����
	m_fBodyDamage = PLAYER_DEFAULTBODYDAMAGE;	// �{�̃_���[�W
	m_fSpeed = PLAYER_DEFAULTSPEED;				// �X�s�[�h
	m_fReload = PLAYER_DEFAULTRELOATD;			// �����[�h����
	m_fLife = PLAYER_DEFAULTLIFE;				// �v���C���[�̗̑�
	m_fMaxLife = PLAYER_DEFAULTMAXLIFE;			// �v���C���[�̗̑͂̍ő�l
	m_fBulletSpeed = PLAYER_BULLETSPEED;		// �e�̑��x
	m_fBulletLife = PLAYER_BULLETLIFE;			// �e�̗̑�
	m_nExp = 0;									// �o���l
	m_nLevelUpExp = 50;							// ���x���A�b�v�ɕK�v�Ȍo���l
	m_nLevel = 1;								// �v���C���[�̏������x��
	m_nCntLevel = 0;							// ���x�����オ������
	m_nLevelSave = 1;							// ���x���̕ۑ�
	m_nGrawLevel[0] = 5;						// �i�����x��1
	m_nGrawLevel[1] = 15;						// �i�����x��2
	m_nGrawLevel[2] = 25;						// �i�����x��3
	m_nGrawCount = 0;							// �i��������
	m_bLevelUp = false;							// ���x�����オ������
	m_PlayerType = PLAYER_TYPE0;				// �v���C���[�^�C�v
	m_nHpCoolTime = HP_COOLTIME;				// �̗̓Q�[�W��������܂ł̎���
	m_fBulletDecrease = BULLET_DEFAULTDECREASE;	// �e�̌����W��
	bRelease = false;

	CObject2D::SetSize(m_fWidth, m_fHeight);
	CObject2D::Init(pos);
	CObject2D::SetTexture(CTexture::TEXTURE_PLAYER0);	//�e�N�X�`���̐ݒ�
	
	// �̗̓Q�[�W�̐���
	m_pHpgage = CHpGage::Create({ m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z }, 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	// �o���l�Q�[�W�̐���
	CExpgage::Create({ 1280 / 4, 680.0f, 0.0f }, 0, EXPGAGE_WIDTH, 10, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_pExpgage = CExpgage::Create({ 1280 / 4, 680.0f, 0.0f }, 0, 0, 10, D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f));

	// �X�e�[�^�X������ʂ̐���
	m_pStatus = CStatus::Create({ 1280 / 2, 590.0f, 0.0f }, 500, 70);

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{// �X�e�[�^�X���x���̕\��
		m_pStatuslevel[nCnt] = CStatuslevel::Create({ 335.0f + nCnt * 90.0f, 601.0f, 0.0f }, 20.0f, 20.0f);

		//�X�e�[�^�X���x��
		m_nStatuslevel[nCnt] = 1;
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{// �i����̕\��
		m_pEvodisplay[nCnt] = CEvodisplay::Create({ 500.0f + nCnt * 130.0f, 75.0f, 0.0f }, 80.0f, 80.0f);
	}

	// �v���C���[���x���̕\��
	m_plevel = CLevelDisplay::Create({ 640, 650.0f, 0.0f }, 20.0f, 20.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	m_plevel->SetAlpha(0.0f);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CPlayer::Update()
{
	CObject2D::Update();

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{
		m_fRot -= PLAYER_ROT;
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{
		m_fRot += PLAYER_ROT;
	}
#ifdef _DEBUG
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E) == true)
	{
		SetExp(50);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_F) == true)
	{
		SetLife(0, 1);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_G) == true)
	{
		SetLife(1, 1);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_C) == true)
	{
		CGame::GetScore()->AddScore(1353);
	}
#endif // _DEBUG

	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);			// �e�����˂����ʒu

	m_BulletPos.x += sinf(m_fRot) * -PLAYER_BULLETPOS;
	m_BulletPos.y += cosf(m_fRot) * -PLAYER_BULLETPOS;

	// �ʒu�̐ݒ�
	CObject2D::Setposition(m_Pos);
	
	D3DXVECTOR3 ZeroPosOld = m_ZeroPos;
	D3DXVECTOR3 PosOld = m_Pos;

	D3DXVECTOR3 StopX = CPlayer::StopX({ 0.0f,0.0f,0.0f });
	D3DXVECTOR3 StopY = CPlayer::StopY({ 0.0f,0.0f,0.0f });

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_ZeroPos.x = StopX.x;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_ZeroPos.y = StopY.y;
	}

	m_ZeroPos += m_Move;

	if (m_fRot > D3DX_PI)
	{
		m_fRot -= D3DX_PI * 2;
	}
	else if (m_fRot < -D3DX_PI)
	{
		m_fRot += D3DX_PI * 2;
	}

	// �p�x�̐ݒ�
	CObject2D::Setrot(m_fRot);

	if (m_fLife > 0)
	{
		// �e������
		Shot();
	}
	// �̗͂�0�ȉ��̏ꍇ
	else
	{
		// �v���C���[��������
		bRelease = CObject2D::SetDisappear(50.0f, 0.0f, 0.5f, 0.03f);

		if (m_bDeathSound == false)
		{
			//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_DEATH_SE);

			m_bDeathSound = true;
		}
	}
	
	if (bRelease == false)
	{
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

			// �̗̓Q�[�W�̐F�̐ݒ�
			m_pHpgage->SetCol({ 0.0f,1.0f,0.0f, m_fHpAlpha });
		}

		// �X�e�[�^�X�\���̐F�̐ݒ�
		m_pStatus->SetCol({ 1.0f, 1.0f, 1.0f, m_fStatusAlpha });

		for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
		{// �X�e�[�^�X���x���̕\��
			m_pStatuslevel[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fStatusAlpha });
		}

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{// �i����̕\���̃A���t�@�l
			m_pEvodisplay[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fEvoAlpha });
		}
	}
	else
	{
		// ���
		Uninit();

		// �X�R�A�̃Z�[�u
		CGame::GetScore()->SaveScore();

		CFade::SetFade(CApplication::MODE_RESULT, 0.015f);
	}

	// ���x���Ɛi���̏���
	CPlayer::LevelUp();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CPlayer::Draw()
{
	CObject2D::Draw();

#ifdef _DEBUG
	m_Font = CRenderer::GetFont();

	//Player(m_Font);
#endif // _DEBUG
}

//==============================================================================================
// ��������
//==============================================================================================
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init(pPlayer->m_Pos);
	}

	return pPlayer;
}

//==============================================================================================
// ���x���A�b�v����
//==============================================================================================
void CPlayer::LevelUp()
{
	if (m_bLevelUp == true)
	{
		m_nLevel++;
		m_nCntLevel++;
		m_nLevelUpExp += 3;
		m_plevel->AddLevel(1, PLAYER_MAXLEVEL, { 670.0f, 650.0f, 0.0f }, OBJTYPE_PLAYER);

		if (m_nLevel < 10)
		{
			m_plevel->SetAlpha(0.0f);
		}

		m_bLevelUp = false;
	}

	// �X�e�[�^�X�̋���
	if (m_nCntLevel != 0)
	{
		// �X�e�[�^�X�\���̐F�̐ݒ�
		m_fStatusAlpha = 1.0f;

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_1))
		{// �̗͂̍ő�lUP
			m_nStatuslevel[0]++;

			if (m_nStatuslevel[0] <= MAX_STATUSLEVLE)
			{
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

				m_pStatuslevel[0]->AddStatuslevel(1);		// ���x���̕\��

				float fOldLife = m_fMaxLife;

				m_fMaxLife += 5;
				m_nCntLevel--;

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
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_2))
		{// �����񕜗�UP
			// �T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[1]++;

			if (m_nStatuslevel[1] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[1]->AddStatuslevel(1);		// ���x���̕\��

				m_fHealLife += m_fMaxLife * 0.0001f;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_3))
		{// �{�̂̍U����UP
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[2]++;

			if (m_nStatuslevel[2] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[2]->AddStatuslevel(1);		// ���x���̕\��

				m_fBodyDamage += 4;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_4))
		{// �e�̑��x
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[3]++;

			if (m_nStatuslevel[3] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[3]->AddStatuslevel(1);		// ���x���̕\��

				m_fBulletSpeed++;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_5))
		{// �e�̗̑�
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[4]++;

			if (m_nStatuslevel[4] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[4]->AddStatuslevel(1);		// ���x���̕\��

				m_fBulletLife += 50.0f;
				m_fBulletDecrease -= 0.001f;

				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_6))
		{// �U����
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[5]++;

			if (m_nStatuslevel[5] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[5]->AddStatuslevel(1);		// ���x���̕\��

				m_fAttack += 2;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_7))
		{// �����[�h���x
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[6]++;

			if (m_nStatuslevel[6] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[6]->AddStatuslevel(1);		// ���x���̕\��

				m_fReload -= 2.0f;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_8))
		{// �ړ����xUP
				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[7]++;

			if (m_nStatuslevel[7] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[7]->AddStatuslevel(1);		// ���x���̕\��

				m_fSpeed += 0.2f;
				m_nCntLevel--;
			}
		}
	}
	else
	{
		// �X�e�[�^�X�\���̐F�̐ݒ�
		m_fStatusAlpha = 0.0f;
	}

	// �i���P���
	if (m_nLevel >= m_nGrawLevel[0]
		&& m_nGrawCount == 0)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION1);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION2);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION3);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER1);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE1;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER2);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE2;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER3);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE3;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
	}
	// �i���Q���
	else if (m_nLevel >= m_nGrawLevel[1]
		&& m_nGrawCount == 1)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION4);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION5);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION6);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER4);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE4;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER5);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE5;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER6);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE6;
			m_fEvoAlpha = 0.0f;

			m_fSpeed /= 1.5;

			m_nGrawCount++;
		}
	}
	// �i���R���
	else if (m_nLevel >= m_nGrawLevel[2]
		&& m_nGrawCount == 2)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION7);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION8);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION9);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER7);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE7;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER8);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE8;
			m_fEvoAlpha = 0.0f;

			m_fHealLife += 0.05f;
			m_fBodyDamage += 15;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER9);	//�e�N�X�`���̐ݒ�
			m_PlayerType = PLAYER_TYPE9;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
	}
}

//==============================================================================================
// �ړ�����
//==============================================================================================
D3DXVECTOR3 CPlayer::Move()
{
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_W) == true)
	{
		m_Move.y = m_fSpeed;

		if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * m_fSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * -m_fSpeed;
		}
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_S) == true)
	{
		m_Move.y = -m_fSpeed;

		if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * m_fSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * -m_fSpeed;
		}
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
	{
		m_Move.x = m_fSpeed;
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
	{
		m_Move.x = -m_fSpeed;
	}

	// �ړ��ʂ̌���
	m_Move.x += (0.0f - m_Move.x) * PLAYER_DECREASE;
	m_Move.y += (0.0f - m_Move.y) * PLAYER_DECREASE;
	
	if (m_fLife > 0)
	{
		return m_Move;
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// �G���A�O�ɏo�Ȃ��悤�ɂ���ݒ�@X���W�p
//==============================================================================================
D3DXVECTOR3 CPlayer::StopX(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos;

	// X���W
	if (m_ZeroPos.x >= PLAYER_STOPPOS)
	{
		Pos.x = pos.x + PLAYER_STOPPOS;

		m_ZeroPos.x = PLAYER_STOPPOS;
		return{ Pos.x ,0.0f,0.0f };
	}
	else if (m_ZeroPos.x <= -PLAYER_STOPPOS)
	{
		Pos.x = pos.x - PLAYER_STOPPOS;

		m_ZeroPos.x = -PLAYER_STOPPOS;
		return{ Pos.x ,0.0f,0.0f };
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// �G���A�O�ɏo�Ȃ��悤�ɂ���ݒ�@Y���W�p
//==============================================================================================
D3DXVECTOR3 CPlayer::StopY(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos;

	// Y���W
	if (m_ZeroPos.y >= PLAYER_STOPPOS)
	{
		Pos.y = pos.y + PLAYER_STOPPOS;

		m_ZeroPos.y = PLAYER_STOPPOS;
		return{ 0.0f ,Pos.y,0.0f };
	}
	else if (m_ZeroPos.y <= -PLAYER_STOPPOS)
	{
		Pos.y = pos.y - PLAYER_STOPPOS;

		m_ZeroPos.y = -PLAYER_STOPPOS;
		return{ 0.0f ,Pos.y,0.0f };
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// �e�����ݒ�
//==============================================================================================
void CPlayer::Shot()
{
	m_fCoolTime--;

	if (m_fCoolTime <= 0.0f)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SHOT_SE);

		// �e������
		if (m_PlayerType == PLAYER_TYPE0)
		{// �v���C���[�^�C�v�O�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE1)
		{// �v���C���[�^�C�v�P�̏ꍇ
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI / 2) * 10.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI / 2) * 10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI / 2) * -10.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI / 2) * -10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE2)
		{// �v���C���[�^�C�v�Q�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.85f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.85f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE3)
		{// �v���C���[�^�C�v�R�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed * 1.8f, BULLET_SIZE - 5.0f, m_fAttack * 0.7f, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE4)
		{// �v���C���[�^�C�v�S�̏ꍇ		
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.y - cosf(m_fRot - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE5)
		{// �v���C���[�^�C�v�T�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE6)
		{// �v���C���[�^�C�v�U�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE + 7.0f, m_fAttack * 1.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 50.0f;
		}
		else if (m_PlayerType == PLAYER_TYPE7)
		{// �v���C���[�^�C�v�V�̏ꍇ
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI) * 50.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI) * 50.0f, m_BulletPos.z), m_fRot + D3DX_PI, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.5f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.5f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.75f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.75f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE8)
		{// �v���C���[�^�C�v�W�̏ꍇ
		}
		else if (m_PlayerType == PLAYER_TYPE9)
		{// �v���C���[�^�C�v�X�̏ꍇ
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot) * -10.0f, m_BulletPos.y + cosf(m_fRot) * -10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed + 1.0f, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.05f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.05f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.1f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.1f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.15f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.15f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.25f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.25f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
		
			m_fCoolTime = m_fReload + 20.0f;
		}

		if (m_PlayerType != PLAYER_TYPE6
			&& m_PlayerType != PLAYER_TYPE9)
		{
			m_fCoolTime = m_fReload;
		}
	}
}

//==============================================================================================
// �̗̓Q�[�W�����鏈��
//==============================================================================================
void CPlayer::HitDamage(float damage)
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
void CPlayer::HealHp(float heal)
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
void CPlayer::SetLife(int type, float life)
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

//==============================================================================================
// �o���l�̐ݒ�
//==============================================================================================
void CPlayer::SetExp(int exp)
{
	m_nExp += exp;

	// �o���l�̏㏸�ʂ̊����̌v�Z
	float fExpPercent = ((float)exp / m_nLevelUpExp) * 100;

	// �o���l�Q�[�W�𑝂₷
	m_bLevelUp = m_pExpgage->AddExpgage(fExpPercent);
}

void CPlayer::Player(LPD3DXFONT pFont)
{
	RECT rect;
	TCHAR str[256];

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		rect = { 0, 0 + nCnt * 20, 1280, 720 };

		if (nCnt == 0)
		{
			sprintf(str, ("E�L�[ : �o���l�̎擾"));
		}
		else if (nCnt == 1)
		{
			sprintf(str, ("F�L�[ : ����"));
		}
		else if (nCnt == 2)
		{
			sprintf(str, ("C�L�[ : �X�R�A�̉��Z"));
		}
		// �e�L�X�g�`��
		pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));
	}
}