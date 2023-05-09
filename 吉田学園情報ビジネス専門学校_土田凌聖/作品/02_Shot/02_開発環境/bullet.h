//==============================================================================================
//
// �e�@�@�@bullet.h
// �y�c����
//
//==============================================================================================
#ifndef _BULLET_H_
#define	_BULLET_H_

#include"object2D.h"

#define	BULLET_SIZE			(15.0f)		// �e�̃T�C�Y
#define BULLET_WIDTH		(20.0f)		// �e�̕�
#define BULLET_HEIGHT		(20.0f)		// �e�̍���
#define	BULLET_SPEED		(8.0f)		// �e�̃X�s�[�h
#define BULLET_DEFAULTDECREASE		(0.01f)	// �����W��
#define BULLET_RADIUS		(10.0f)		// �e�̔��a

class CPlayer;
class CEnemy;

class CBullet : public CObject2D
{
public:
	CBullet();
	~CBullet();

	enum BULLET_TYPE
	{
		BULLET_NONE = 0,
		BULLET_PLAYER,		// �v���C���[�̒e
		BULLET_ENEMY,		// �G�̒e
		BULLET_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CBullet *Create(D3DXVECTOR3 pos, float rot, int type, float life, float speed, float size, float attack, float decrease);
	bool BulletMove(float *BulletLife, D3DXVECTOR3 *ShotMove, float Decrease);

	void SetRad(float Rad) { m_fRadius = Rad; }							// ���a�̐ݒ�
	void SetRot(float rot) { m_fRot = rot; }							// �p�x�̐ݒ�
	void SetType(int type) { m_nType = type; }							// ��ނ̐ݒ�
	void SetPBulletLife(float life) { m_fPBulletLife = life; }			// �v���C���[�̒e�̗̑͂̐ݒ�
	void SetPBulletSpeed(float speed) { m_PlayerShotMove.y = speed; }	// �v���C���[�̒e�̑��x�̐ݒ�
	void SetPBulletAttack(float attack) { m_fPAttack = attack; }		// �v���C���[�̒e�̍U���͂̐ݒ�
	void SetPDecrease(float decrease) { m_fPDecrease = decrease; }		// �v���C���[�̒e�̌����W���̐ݒ�
	void SetEBulletLife(float life) { m_fEBulletLife = life; }			// �G�̒e�̗̑͂̐ݒ�
	void SetEBulletSpeed(float speed) { m_EnemyShotMove.y = speed; }	// �G�̒e�̑��x�̐ݒ�
	void SetEBulletAttack(float attack) { m_fEAttack = attack; }		// �G�̒e�̍U���͂̐ݒ�
	void SetEDecrease(float decrease) { m_fEDecrease = decrease; }		// �G�̒e�̌����W���̐ݒ�

	EObjType GetObjType() override { return OBJTYPE_BULLET; }	// �I�u�W�F�N�g�̃^�C�v�̎擾
	D3DXVECTOR3 GetPos() override { return m_Pos; }				// �ʒu�̎擾

private:
	D3DXVECTOR3 m_Pos;				// �ʒu
	D3DXVECTOR3 m_InitPos;			// �����ʒu
	D3DXVECTOR3 m_Move;				// �ړ���
	D3DXVECTOR3 m_PlayerShotMove;	// �v���C���[�̒e�̈ړ���
	D3DXVECTOR3 m_EnemyShotMove;	// �G�̒e�̈ړ���
	int m_nType;					// �e�̎��
	float m_fWidth;					// ��
	float m_fHeight;				// ����
	float m_fRot;					// �p�x
	float m_fPBulletLife;			// �v���C���[�̒e�̎���
	float m_fEBulletLife;			// �G�̒e�̎���
	float m_fPAttack;				// �v���C���[�̍U����
	float m_fEAttack;				// �G�̍U����
	float m_fPDecrease;				// �v���C���[�̌����W��
	float m_fEDecrease;				// �G�̌����W��
	float m_fDistance;				// �I�u�W�F�N�g�ƒe�̋���
	float m_fRadius;				// �e�̔��a
	bool m_bHit;					// ����������
};

#endif // !_BULLET_H_
