//==============================================================================================
//
// �v���C���[�@�@�@player.h
// �y�c����
//
//==============================================================================================
#ifndef _PLAYER_H_
#define	_PLAYER_H_

#include"object2D.h"

#define	PLAYER_SIZE					(25.0f)		// �T�C�Y
#define PLAYER_WIDTH				(50.0f)		// ��
#define PLAYER_HEIGHT				(50.0f)		// ����
#define PLAYER_BULLETPOS			(30.0f)		// ���S����C���܂ł̋���
#define PLAYER_DEFAULTLIFE			(200)		// �ŏ��̗̑�
#define PLAYER_DEFAULTMAXLIFE		(200)		// �ŏ��̗̑͂̍ő�l
#define	PLAYER_DEFAULTATTACK		(5.0f)		// �ŏ��̍U����
#define	PLAYER_DEFAULTSPEED			(3)			// �ŏ��̈ړ��X�s�[�h
#define	PLAYER_DEFAULTBODYDAMAGE	(10.0f)		// �ŏ��̖{�̍U����
#define PLAYER_BULLETSPEED			(8)			// �e�̑��x
#define PLAYER_BULLETLIFE			(100.0f)	// �e�̗̑�
#define	PLAYER_ROT					(0.05f)		// ��]���x
#define	PLAYER_RADIUS				(30.0f)		// ���a
#define	PLAYER_DEFAULTRELOATD		(40.0f)		// �ŏ��̃����[�h����
#define PLAYER_DECREASE				(0.02f)		// �����W��
#define PLAYER_STOPPOS				(1200)		// �ړ��ł���͈�
#define PLAYER_MAXLEVEL				(30)		// �ő僌�x��
#define PLAYER_NUMSTATUS			(8)			// �X�e�[�^�X�̐�sdf

// �O���錾
class CEnemy;
class CHpGage;
class CExpgage;
class CStatus;
class CStatuslevel;
class CEvodisplay;
class CLevelDisplay;

class CPlayer : public CObject2D
{
public:
	CPlayer();
	~CPlayer();

	enum PlayerType
	{
		PLAYER_NONE = 0,
		PLAYER_TYPE0,		// �v���C���[�^�C�v�O
		PLAYER_TYPE1,		// �v���C���[�^�C�v�P
		PLAYER_TYPE2,		// �v���C���[�^�C�v�Q
		PLAYER_TYPE3,		// �v���C���[�^�C�v�R
		PLAYER_TYPE4,		// �v���C���[�^�C�v�S
		PLAYER_TYPE5,		// �v���C���[�^�C�v�T
		PLAYER_TYPE6,		// �v���C���[�^�C�v�U
		PLAYER_TYPE7,		// �v���C���[�^�C�v�V
		PLAYER_TYPE8,		// �v���C���[�^�C�v�W
		PLAYER_TYPE9,		// �v���C���[�^�C�v�X
		PLAYER_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CPlayer *Create();
	void LevelUp();
	static D3DXVECTOR3 Move();
	static D3DXVECTOR3 StopX(D3DXVECTOR3 pos);
	static D3DXVECTOR3 StopY(D3DXVECTOR3 pos);
	void Shot();
	void HitDamage(float damage);		// �̗̓Q�[�W�����鏈��
	void HealHp(float heal);			// �̗̓Q�[�W�������鏈��
	void Player(LPD3DXFONT pFont);

	void SetLife(int type, float life) override;
	void SetExp(int exp) override;

	EObjType GetObjType() override { return OBJTYPE_PLAYER; }		// �I�u�W�F�N�g�̃^�C�v�̎擾
	D3DXVECTOR3 GetPos() override { return m_Pos; }					// �v���C���[�̈ʒu�̎擾
	D3DXVECTOR3 GetMove() override { return m_Move; }				// �v���C���[�̈ړ��ʂ̎擾
	float GetSize() override { return m_fSize; }					// �T�C�Y�̎擾
	float GetRadius() override { return m_fRadius; }				// ���a�̎擾
	float GetLife() override { return m_fLife; }						// �̗͂̎擾
	float GetAttack() override { return m_fAttack; }					// �U���͂̎擾
	float GetBodyDamage() override { return m_fBodyDamage; }			// �{�̃_���[�W�̎擾
	float GetBulletSpeed() override { return m_fBulletSpeed; }		// �e�̃X�s�[�h�̎擾

private:
	int m_nLevel;								// �v���C���[�̃��x��
	int m_nCntLevel;							// ���x�����オ������
	int m_nLevelSave;							// ���x���̕ۑ�
	int m_nGrawLevel[3];						// �i�����郌�x��
	int m_nGrawCount;							// �i��������
	int m_nExp;									// �o���l
	int m_nLevelUpExp;							// ���x���A�b�v�ɕK�v�Ȍo���l
	int m_nHpCoolTime;							// �̗̓Q�[�W��������܂ł̎���
	int m_nStatuslevel[8];						// �X�e�[�^�X���x��
	float m_fSize;								// �T�C�Y
	float m_fWidth;								// ��
	float m_fHeight;							// ����
	float m_fRadius;							// �v���C���[�̔��a
	float m_fDamage;							// �󂯂��_���[�W
	float m_fTotalDamage;						// �󂯂��_���[�W�̍��v
	float m_fHeal;								// �̗͉񕜗�
	float m_fHpAlpha;							// �̗̓Q�[�W�̃A���t�@�l
	float m_fStatusAlpha;						// �X�e�[�^�X�\���̃A���t�@�l
	float m_fEvoAlpha;							// �i����\���̃A���t�@�l
	float m_fHealLife;							// �̗͂̉񕜗�
	float m_fMaxLife;							// �̗͂̍ő�l
	float m_fBodyDamage;						// �{�̃_���[�W
	float m_fBulletSpeed;						// �e�̑��x
	float m_fBulletLife;						// �e�̗̑�
	float m_fAttack;							// �U����
	float m_fReload;							// �����[�h�̎���
	float m_fBulletDecrease;					// �e�̌����W��
	bool bRelease;								// �v���C���[�����[�X���ꂽ��
	bool m_bCreateHp;							// �̗̓Q�[�W���o����
	bool m_bLevelUp;							// ���x�����オ������
	bool m_bDeathSound;							// ���S�����Ȃ�����
	D3DXVECTOR3 m_Pos;							// �ʒu
	PlayerType m_PlayerType;					// �v���C���[�̃^�C�v
	CHpGage *m_pHpgage;							// �̗̓Q�[�W�̂̏��
	CExpgage *m_pExpgage;						// �o���l�Q�[�W�̏��
	CStatus *m_pStatus;							// �X�e�[�^�X�̏��
	CStatuslevel *m_pStatuslevel[PLAYER_NUMSTATUS];	// �X�e�[�^�X���x���̏��
	CEvodisplay *m_pEvodisplay[3];				// �i����̏��
	CLevelDisplay *m_plevel;					// �v���C���[�̃��x���\���̏��
	static D3DXVECTOR3 m_ZeroPos;				// 0�̈ʒu
	static D3DXVECTOR3 m_Move;					// �ړ���
	static D3DXVECTOR3 m_BulletPos;				// �e�𔭎˂���ʒu
	static float m_fRot;						// �p�x
	static float m_fLife;						// �̗�
	static float m_fSpeed;						// �X�s�[�h
	static float m_fCoolTime;					// �e�̃N�[���^�C��
	static bool m_bCollisionX;					// �ǂɓ���������(X���W)
	static bool m_bCollisionY;					// �ǂɓ���������(Y���W)

	LPD3DXFONT m_Font;		// �t�H���g
};

#endif // !_PLAYER_H_
