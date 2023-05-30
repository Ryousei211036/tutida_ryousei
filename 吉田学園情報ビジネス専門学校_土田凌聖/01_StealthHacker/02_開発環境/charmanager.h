//==============================================================================================
// 
// �L�����N�^�[�}�l�[�W���[�@�@�@charmanager.h
// �y�c����
// 
//==============================================================================================
#ifndef _CHARMANAGER_H_
#define	_CHARMANAGER_H_

#include"object.h"
#include"model.h"

class CCharManager : public CObject
{
public:
	CCharManager();
	~CCharManager();
	
	// ���[�V�����̃^�C�v
	enum Motion_Type
	{
		MOTION_NEUTRAL = 0,		// �j���[�g����
		MOTION_WALK,			// �������
		MOTION_GUN,				// �e���\����
		MOTION_SHOT,			// ����
		MOTION_KILL,			// �X�e���X�L��
		MOTION_STEALTHDEATH,	// �X�e���X�f�X
		MOTION_DEATH,			// ���S
		MOTION_BUTTON,			// �{�^��������
		MOTION_FIND,			// ���G
		MOTION_MAX,
		MOTION_NONE
	};

	// ����̃^�C�v
	enum Control_Type
	{
		CONTROL_PLAYER = 0,	// �v���C���[
		CONTROL_ROBOT,		// ���{�b�g
		CONTROL_CAMERA,		// �J����
		CONTROL_MAX,
		CONTROL_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void Move() = 0;			// �ړ�����
	virtual void Action() = 0;			// �A�N�V����
	virtual void SetMotionData(int maxkey, int key, int parts, int motion,
				int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// ���[�V�����l�̓ǂݍ���
	virtual void Motion();							// ���[�V�����̐ݒ�
			bool PlayMotion(Motion_Type motion);	// ���[�V�������I�������
	virtual void ChangeMotion(int nummotion);		// ���[�V�����̐؂�ւ�
	virtual CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type);	// ���f���p�[�c�̐ݒ�
	virtual void Process_Motion() = 0;				// ���[�V�����Đ����̏���
	void ChangeControl(Control_Type type);			// ����̕ύX
	bool ChangeCoolTime();							// ����ύX�̃N�[���^�C��
	int Damage(int hp, int damage);					// �_���[�W
	float NormalizeRot(float rotspeed);				// �p�x�̐��K��
	virtual void Death() = 0;		// ���S

	// �Z�b�^�[
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }					// �ʒu
	void SetPosOld(D3DXVECTOR3 posold) { m_PosOld = posold; }		// �O��̈ʒu
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }					// �p�x
	void SetRotDest(D3DXVECTOR3 rotdest) { m_RotDest = rotdest; }	// �ړI�̊p�x
	void SetSizeMin(D3DXVECTOR3 min) { m_SizeMin = min; }			// �T�C�Y�̍ŏ��l
	void SetSizeMax(D3DXVECTOR3 max) { m_SizeMax = max; }			// �T�C�Y�̍ő�l
	void SetGunPos(D3DXVECTOR3 gunpos) { m_GunPos = gunpos; }		// �e�̈ʒu
	void SetMotion(Motion_Type motion) { m_Motion_Type = motion; }	// ���[�V����
	void SetRadius(float radius) { m_fRadius = radius; }			// ���a
	static void SetControl(Control_Type type) { m_Control_Type = type; }	// ����̃^�C�v
	void SetChangeCoolTime(int cooltime) { m_nControlChange_CoolTime = cooltime; }
	void SetLife(int life) { m_nLife = life; }						// �̗�
	///*static*/ void SetStealthKill(bool kill) { m_bStealthKill = kill; }
	void SetPlayStealthKill(bool kill) { m_bPlayStealthKill = kill; }
	void SetHit(bool hit) { m_bHit = hit; }
	void SetPushButton(bool button) { m_bPushButton = button; }		// �{�^������������

	// �Q�b�^�[
	D3DXVECTOR3 GetPos() { return m_Pos; }						// �ʒu
	D3DXVECTOR3 GetPosOld() { return m_PosOld; }				// �O��̈ʒu
	D3DXVECTOR3 GetGunPos() { return m_GunPos; }				// �e�̈ʒu
	D3DXVECTOR3 GetRot() { return m_Rot; }						// �p�x
	D3DXVECTOR3 GetRotDest() { return m_RotDest; }				// �ړI�̊p�x
	D3DXVECTOR3 GetSizeMin() { return m_SizeMin; }				// �T�C�Y�̍ŏ��l
	D3DXVECTOR3 GetSizeMax() { return m_SizeMax; }				// �T�C�Y�̍ő�l
	Motion_Type GetMotion_Type() { return m_Motion_Type; }		// ���[�V����
	static Control_Type GetControlType() { return m_Control_Type; }	// ����̃^�C�v
	float GetRadius() { return m_fRadius; }						// ���a
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }				// ���[���h�}�g���b�N�X
	int GetLife() { return m_nLife; }							// �̗�
	///*static*/ bool GetStealthKill() { return m_bStealthKill; }		// �X�e���X�L���ł��邩
	bool GetPlayStealthKill() { return m_bPlayStealthKill; }		// �X�e���X�L���ł��邩
	bool GetHit() { return m_bHit; }							// �e�ɓ���������
	bool GetPushButton() { return m_bPushButton; }				// �{�^������������

private:
	static const int m_nNumParts = 11;		// �p�[�c��
	static const int m_nNumMotion = 11;		// ���[�V������

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Pos;						// �ʒu
	D3DXVECTOR3 m_PosOld;					// �ߋ��̈ʒu
	D3DXVECTOR3 m_GunPos;					// �e�̈ʒu
	D3DXVECTOR3 m_Rot;						// ���݂̊p�x
	D3DXVECTOR3 m_RotDest;					// �ړI�̊p�x
	D3DXVECTOR3 InitPos[m_nNumParts];		// �p�[�c�̏����ʒu
	D3DXVECTOR3 InitRot[m_nNumParts];		// �p�[�c�̏����p�x
	D3DXVECTOR3 m_SizeMin;					// ���_�̍ŏ��l
	D3DXVECTOR3 m_SizeMax;					// ���_�̍ő�l
	CModel *m_pModel[m_nNumParts];			// ���f���̏��
	int m_nLife;							// �̗�
	int m_nCurrentKey;						// ���݂̃L�[�ԍ�
	int m_nCurrentMotion;					// ���݂̃��[�V�����ԍ�
	int m_nCountMotion;						// ���[�V�����J�E���^�[
	static int m_nControlChange_CoolTime;	// ����ύX�̃N�[���^�C��
	float m_fRadius;						// ���a
	float m_fSpeed;							// �ړ����x
	bool m_bFootstep;						// �������Ȃ��Ă邩
	bool m_bHit;							// �U�����󂯂���
	bool m_bPushButton;						// �{�^������������
	bool m_bMove;							// ��������
	///*static*/ 	bool m_bStealthKill;			// �X�e���X�L�����ł��邩
	bool m_bPlayStealthKill;				// �X�e���X�L������
	char *m_XFileName[m_nNumParts];			// X�t�@�C���l�[��
	Motion_Type m_Motion_Type;				// ���[�V�����̃^�C�v
	static Control_Type m_Control_Type;		// ����̃^�C�v
	D3DXVECTOR3 POS[m_nNumParts];
	D3DXVECTOR3 ROT[m_nNumParts];

	// �L�[
	struct KEY
	{
		D3DXVECTOR3 KeyPos;
		D3DXVECTOR3 KeyRot;
	};

	// �L�[�Z�b�g
	struct KEY_SET
	{
		int nFrame;
		KEY aKey[m_nNumParts];
	};

	// ���[�V�����Z�b�g
	struct MOTION_SET
	{
		int nMaxKey;			// �L�[�̑���
		bool bLoop;				// ���[�v���邩
		bool bPlay;				// ���[�V�����Đ�����
		bool bStop;				// ���[�V�������~�܂�����
		KEY_SET aKeySet[15];
	};
	MOTION_SET m_MotionSet[m_nNumMotion];
};

#endif // !_CHARMANAGER_H_
