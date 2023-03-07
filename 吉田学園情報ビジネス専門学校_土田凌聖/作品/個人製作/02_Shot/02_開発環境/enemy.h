//==============================================================================================
//
// �G�@�@�@enemy.h
// �y�c����
//
//==============================================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include"object2D.h"

#define MAX_ENEMY				(5)			// �G�̍ő吔
#define	ENEMY_SIZE				(25.0f)		// �G�̃T�C�Y
#define	ENEMY_WIDTH				(50.0f)		// �G�̕�
#define	ENEMY_HEIGHT			(50.0f)		// �G�̍���
#define ENEMYCREATE_TIME		(600)		// �����܂ł̎���
#define	ENEMY_DAMAGECOOLTIME	(100.0f)	// �U�������������̃N�[���^�C��
#define	ENEMY_RADIUS			(30.0f)		// �G�̔��a
#define	ENEMY_ROT				(0.05f)		// �G�̉�]���x
#define TARGET_COUNT			(600)		// �v���C���[��_������
#define ENEMY_DEFAULTLIFE		(20)		// �ŏ��̗̑�
#define ENEMY_DEFAULTMAXLIFE	(20)		// �ŏ��̗̑͂̍ő�l
#define	ENEMY_DEFAULTATTACK		(5.0f)		// �ŏ��̍U����
#define	ENEMY_DEFAULTSPEED		(3)			// �ŏ��̈ړ��X�s�[�h
#define	ENEMY_DEFAULTBODYDAMAGE	(10.0f)		// �ŏ��̖{�̍U����
#define ENEMY_BULLETSPEED		(8)			// �e�̑��x
#define ENEMY_BULLETLIFE		(100.0f)	// �e�̗̑�
#define	ENEMY_DEFAULTRELOATD	(40.0f)		// �ŏ��̃����[�h����
#define ENEMY_MAXLEVEL			(30)		// �ő僌�x��
#define ENEMY_DECREASE			(0.05f)		// �����W��
#define ENEMY_NUMSTATUS			(8)			// �X�e�[�^�X�̐�

class CPlayer;
class CHpGage;
class CStatuslevel;

class CEnemy : public CObject2D
{
public:
	CEnemy();
	~CEnemy() override;

	enum EnemyType
	{
		ENEMY_NONE = 0,
		ENEMY_TYPE0,		// �G�^�C�v�O
		ENEMY_TYPE1,		// �G�^�C�v�P
		ENEMY_TYPE2,		// �G�^�C�v�Q
		ENEMY_TYPE3,		// �G�^�C�v�R
		ENEMY_TYPE4,		// �G�^�C�v�S
		ENEMY_TYPE5,		// �G�^�C�v�T
		ENEMY_TYPE6,		// �G�^�C�v�U
		ENEMY_TYPE7,		// �G�^�C�v�V
		ENEMY_TYPE8,		// �G�^�C�v�W
		ENEMY_TYPE9,		// �G�^�C�v�X
		ENEMY_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CEnemy *Create(D3DXVECTOR3 pos, int Level);
	void Move();						// �ړ�����
	void Shot();						// �e������
	void LevelUp();						// ���x���A�b�v����
	void HitDamage(float damage);		// �̗̓Q�[�W�����鏈��
	void HealHp(float heal);			// �̗̓Q�[�W�������鏈��
	void Enemy(LPD3DXFONT pFont);

	void SetLevel(int Level);			// ���x���̐ݒ�
	void SetLife(int type, float life) override;

	EObjType GetObjType() override { return OBJTYPE_ENEMY; }		// �I�u�W�F�N�g�̃^�C�v�̎擾
	D3DXVECTOR3 GetPos() override { return m_Pos; }					// �ʒu�̎擾
	float GetSize() override { return m_fSize; }					// �T�C�Y�̎擾
	float GetRadius() override { return m_fRadius; }				// ���a�̎擾
	float GetLife() { return m_fLife; }								// �̗͂̎擾
	float GetAttack() override { return m_fAttack; }				// �U���͂̎擾
	float GetBodyDamage() override { return m_fBodyDamage; }		// �{�̃_���[�W�̎擾
	float GetBulletSpeed() override { return m_fBulletSpeed; }		// �e�̃X�s�[�h�̎擾
	static int GetNumEnemy() { return m_nNumEnemy; }				// �G�̐��̎擾
	static int GetNumMaxEnemy() { return m_nNumEnemy; }				// �G�̍ő吔�̎擾
	static int GetNumDeathEnemy() { return m_nDeathCount; }				// �G�̍ő吔�̎擾

	static void SetNumEnemy(int numenemy) { m_nNumEnemy = numenemy; }

private:
	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_InitPos;		// �����ʒu
	D3DXVECTOR3 m_Move;			// �G�̈ړ���
	D3DXVECTOR3 m_PlayerMove;	// �v���C���[�̈ړ���
	D3DXVECTOR3 m_TotalMove;	// �ړ��ʂ̍��v
	D3DXVECTOR3 m_CreatePos;	// ��������ʒu
	D3DXVECTOR3 m_BulletPos;	// �e�𔭎˂���ʒu
	int m_fTargetCount;			// �v���C���[��_������
	int m_nExp;					// �o���l
	int m_nExpSave;				// �o���l�̕ۑ���
	int m_nGiveExp;				// �n���o���l
	int m_nScore;				// �X�R�A
	int m_nDamage;				// �󂯂��_���[�W
	int m_nHpCoolTime;			// �̗̓Q�[�W��������܂ł̎���
	int m_nLevel;				// �G�̃��x��
	int m_nLevelSave;			// ���x���̕ۑ�
	int m_nGrawLevel[3];		// �i�����郌�x��
	int m_nStatuslevel[8];		// �X�e�[�^�X���x��
	int m_nLvUpCount;			// ���x�����オ������
	float m_fRot;				// �p�x
	float m_fSize;				// �T�C�Y
	float m_fWidth;				// ��
	float m_fHeight;			// ����
	float m_fCoolTime;			// �e�̃N�[���^�C��
	float m_fRadius;			// �G�̔��a
	float m_fBulletDecrease;	// �e�̌����W��
	float m_fDamageCoolTime;	// �U�����󂯂���̃N�[���^�C��
	float m_fTotalDamage;		// �󂯂��_���[�W�̍��v
	float m_fHpAlpha;			// �̗̓Q�[�W�̃A���t�@�l
	float m_fAngle;				// �G�ƃv���C���[�A�a�̊p�x
	float m_fAngleDest;			// �ړI�̊p�x
	float m_fTargetDistance;	// �^�[�Q�b�g�Ƃ̋���
	float m_fLife;				// �̗�
	float m_fHealLife;			// �̗͂̎����񕜗�
	float m_fMaxLife;			// �̗͂̍ő�l
	float m_fBodyDamage;		// �{�̃_���[�W
	float m_fBulletSpeed;		// �e�̑��x
	float m_fBulletLife;		// �e�̗̑�
	float m_fAttack;			// �U����
	float m_fReload;			// �����[�h�̎���
	float m_fSpeed;				// �X�s�[�h
	bool m_bGrawUp;				// �i��������
	bool m_bHit;				// �U��������������
	bool m_bNearPlayer;			// �v���C���[�ɋ߂���
	bool m_bCreateHp;			// �̗̓Q�[�W���o����
	CHpGage *m_pHpgage;			// �̗̓Q�[�W�̏��
	EnemyType m_EnemyType;		// �G�̃^�C�v
	D3DXVECTOR3 m_TargetPos;	// �^�[�Q�b�g�̈ʒu
	CStatuslevel *m_pStatuslevel[ENEMY_NUMSTATUS];	// �X�e�[�^�X���x���̏��
	static int m_nDeathCount;	// �|���ꂽ��
	static int m_nNumEnemy;		// �G�̐�
	static int m_nCreateTime;	// �G�����̃N�[���^�C��

	LPD3DXFONT m_Font;		// �t�H���g
};

#endif // !_ENEMY_H_
