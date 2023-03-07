//==============================================================================================
//
// �G�@�@�@enemy.h
// �y�c����
//
//==============================================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include"player.h"
#include"charmanager.h"

class CModel;
class CObject3D;

class CEnemy : public CCharManager

{
public:
	CEnemy();
	~CEnemy();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	void Move() override;		// �ړ�����
	void Action() override;		// �G�̃A�N�V����
	void Death() override;		// ���S
	void Search(CObject::EObjType type);		// ���G
	void SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop) override;	// ���[�V�����l�̓ǂݍ���
	void Motion() override;						// ���[�V�����̐ݒ�
	void ChangeMotion(int nummotion) override;	// ���[�V�����̐؂�ւ�
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type) override;		// ���f���p�[�c�̐ݒ�
	void Process_Motion() override;		// ���[�V�������̏���

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	static void SetNumEnemy(int num) { m_nNumEnemy = num; }

	EObjType GetObjType() override { return OBJTYPE_ENEMY; }
	static int GetNumEnemy() { return m_nNumEnemy; }

private:
	D3DXVECTOR3 m_Pos;				// �ʒu
	D3DXVECTOR3 m_PosOld;			// �O��̈ʒu
	D3DXVECTOR3 m_Rot;				// �p�x
	D3DXVECTOR3 m_RotDest;			// �p�x
	D3DXVECTOR3 m_EyeRot;			// �����̊p�x
	D3DXVECTOR3 m_SizeMin;			// ���_�̍ŏ��l
	D3DXVECTOR3 m_SizeMax;			// ���_�̍ő�l
	D3DXVECTOR3 m_TargetPos;		// �^�[�Q�b�g�̈ʒu
	Motion_Type m_Motion;			// �G�̏��
	static int m_nNumEnemy;			// �G�̐�
	int m_nLife;					// �̗�
	int m_nBulletCoolTime;			// �e�����Ԋu
	int m_nAlpha_Exclamation;		// �r�b�N���}�[�N��������܂ł̎���
	int m_nChaseTime;				// �ǂ������鎞��
	float m_fSpeed;					// �ړ����x
	float m_fRotSpeed;				// ��]�̃X�s�[�h
	float m_fRadius;				// ���a
	float m_fKillArea;				// �X�e���X�L���ł���͈�
	float m_fKillDistance;			// �X�e���X�L���ł��鋗��
	float m_fView_Distance;			// �G�Ɍ����鋗��
	bool m_bStealthKill;
	bool m_bPlay_Shot;				// �ˌ����[�V��������
	bool m_bStealthDeath;
	bool m_bFind_Player;			// �v���C���[����������
	bool m_bFind_Robot;				// ���{�b�g����������
	bool m_bLostTarget;				// �^�[�Q�b�g������������
	bool m_bDeath;					// ���S������
	bool m_bHit;					// �U�����󂯂���
	bool m_bEyeType;
	bool HitObject;
	bool FindSound;
	CObject3D *pObject3D;

	// �v�Z�p
	D3DXVECTOR3 PPos;		// �ʒu(�v���C���[)
	D3DXVECTOR3 RPos;		// �ʒu(���{�b�g)
	D3DXVECTOR3 OPos;		// �ʒu(�I�u�W�F�N�gX)
};

#endif // !_ENEMY_H_
