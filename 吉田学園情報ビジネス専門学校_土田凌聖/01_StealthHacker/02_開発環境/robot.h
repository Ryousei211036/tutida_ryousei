//==============================================================================================
//
// ���{�b�g�@�@�@robot.h
// �y�c����
//
//==============================================================================================
#ifndef _ROBOT_H_
#define	_ROBOT_H_

#include"charmanager.h"

class CGage;

class CRobot : public CCharManager
{
public:
	CRobot();
	~CRobot();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRobot *Create(D3DXVECTOR3 pos);

	void Move() override;			// �ړ�����
	void Action() override;			// �A�N�V����
	void Death() override;			// ���S
	void SetMotionData(int maxkey, int key, int parts, int motion,
		int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop) override;				// ���[�V�����l�̓ǂݍ���
	void Motion() override;							// ���[�V�����̐ݒ�
	void ChangeMotion(int nummotion) override;		// ���[�V�����̐؂�ւ�
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type) override;	// ���f���p�[�c�̐ݒ�
	void Process_Motion() override;		// ���[�V�������̏���

private:
	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_PosOld;		// �ߋ��̈ʒu
	D3DXVECTOR3 m_Rot;			// ���݂̊p�x
	D3DXVECTOR3 m_RotDest;		// �ړI�̊p�x
	Motion_Type m_Motion;		// ���{�b�g�̏��
	D3DXVECTOR3 m_SizeMin;		// ���_�̍ŏ��l
	D3DXVECTOR3 m_SizeMax;		// ���_�̍ő�l
	int m_nConChange_CoolTime;	// ����ύX�̃N�[���^�C��
	int m_nLife;				// �̗�
	float m_fRotSpeed;			// ��]�̃X�s�[�h
	float m_fSpeed;				// �ړ����x
	float m_fRadius;			// ���a
	bool m_bDeath;				// ���S������
	bool m_bGageFull_Up;		// �Q�[�W�����^���ɂȂ�����
	bool m_bResetGage;			// �Q�[�W�����Z�b�g������
	bool m_bPushButton;			// �{�^������������
	CGage *m_pGage;				// �Q�[�W�̏��
};

#endif // !_ROBOT_H_
