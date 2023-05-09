//==============================================================================================
//
// �a�@�@�@exp.h
// �y�c����
//
//==============================================================================================
#ifndef _EX_H_
#define	_EX_H_

#include"object2D.h"
#include"object.h"
#include"bullet.h"

#define	MAX_EXP				(50)		// �a�̍ő吔
#define	EXP_RADIUS			(20.0f)		// �a�̔��a
#define EXP_CREATETIME		(50)		// �����܂ł̎���
#define	EXP_WIDTH			(10.0f)		// �a�̕�
#define	EXP_HEIGHT			(10.0f)		// �a�̍���
#define	EXP_TRIANGLESIZE	(20.0f)		// �O�p�a�̃T�C�Y
#define	EXP_BOXSIZE			(15.0f)		// �l�p�a�̃T�C�Y
#define	EXP_PENTAGONSIZE	(30.0f)		// �܊p�`�a�̃T�C�Y
#define	EXP_TRIANGLELIFE	(15)		// �O�p�a�̗̑�
#define	EXP_BOXLIFE			(10)		// �l�p�a�̗̑�
#define	EXP_PENTAGONLIFE	(20)		// �܊p�`�a�̗̑�
#define EXP_BODYDAMAGE		(5)			// �{�̍U����
#define	EXP_DAMAGECOOLTIME	(100.0f)	// �U�������������̃N�[���^�C��
#define EXP_DECREASE		(0.1f)		// �����W��

class CHpGage;

class CExp : public CObject2D
{
public:
	CExp();
	~CExp();

	enum Exptype
	{
		EXPTYPE_TRIANGLE = 0,	// �O�p
		EXPTYPE_BOX,			// �l�p
		EXPTYPE_PENTAGON,		// �܊p�`
		EXPTYPE_MAX,
		EXPTYPE_NONE,
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	static CExp *Create(D3DXVECTOR3 pos, int type, float rot);

	void SetLife(int type, float life) override;
	void SetType(int type) { m_nType = type; }
	float HitDamage(float damage); 								// �󂯂��_���[�W�̎擾
	static void SetExpBreakType(CBullet::BULLET_TYPE type) { m_BulletType = type; }
	void SetHpGage(float hpgage) override{ m_fHpGage = hpgage; }

	float GetSize() override { return m_fSize; }				// �T�C�Y�̎擾
	float GetLife() override { return m_fLife; }				// �̗͂̎擾
	EObjType GetObjType() override { return OBJTYPE_EXP; }		// �I�u�W�F�N�g�̃^�C�v�̎擾
	CExp::Exptype GetExpType();									// �a�̃^�C�v�̎擾
	D3DXVECTOR3 GetPos() override { return m_Pos; }				// �ʒu�̎擾
	float GetRadius() override { return m_fRadius; }			// ���a�̎擾
	int GetExp() override { return m_nExp; }					// �o���l�̎擾
	int GetNumTriangle() override { return m_nNumTriangle; }	// �O�p�̐�
	int GetNumBox() override { return m_nNumBox; }				// �l�p�̐�
	int GetNumPentagon() override { return m_nNumPentagon; }	// �܊p�`�̐�
	static int GetNumExp() { return m_nNumExp; }				// �a�̐�
	float GetBodyDamage() override { return m_fBodyDamage; }	// �U���͂̎擾
	float GetHpGage() override { return m_fHpGage; }			// �̗̓Q�[�W�̒���

private:
	D3DXVECTOR3 m_Pos;				// �ʒu
	D3DXVECTOR3 m_InitPos;			// �����ʒu
	D3DXVECTOR3 m_Move;				// �ړ���
	D3DXVECTOR3 m_PushMove;			// �����ꂽ���̈ړ���
	D3DXVECTOR3 m_TotalMove;		// �ړ��ʂ̍��v
	int m_nExp;						// �o���l
	int m_nScore;					// �X�R�A
	int m_nHpCoolTime;				// �̗̓Q�[�W��������܂ł̎���
	int m_nType;					// ���
	int m_nBodyDamageType;			// �v���C���[�ƓG�ǂ����ɓ���������
	float m_fSize;					// �T�C�Y
	float m_fWidth;					// ��
	float m_fHeight;				// ����
	float m_fLife;					// �̗�
	float m_fMaxLife;				// �ő�̗�
	float m_fRadius;				// ���a
	float m_fBodyDamage;			// �{�̍U����
	float m_fDamageCoolTime;		// �U�������������̃N�[���^�C��
	float m_fHpAlpha;				// �̗̓Q�[�W�̃A���t�@�l
	float m_fHpGage;				// �̗̓Q�[�W�̒���
	float m_fDamage;				// �󂯂��_���[�W
	bool m_bDamage;					// �U�������������
	bool m_bCreateHp;				// �̗̓Q�[�W���o����
	CHpGage *m_pHpgage;			// �̗̓Q�[�W�̏��
	static CBullet::BULLET_TYPE m_BulletType;		// �a���󂵂��e�̎��
	static D3DXVECTOR3 m_CreatePos;	// ��������ʒu
	static int m_nNumTriangle;		// �O�p�̐�
	static int m_nNumBox;			// �l�p�̐�
	static int m_nNumPentagon;		// �܊p�`�̐�
	static int m_nNumExp;			// ��
};

#endif // !_EX_H_
