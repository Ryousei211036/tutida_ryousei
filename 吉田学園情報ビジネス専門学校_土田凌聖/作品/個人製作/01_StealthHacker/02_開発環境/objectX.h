//==============================================================================================
//
//���f���̏���[model.h]
//Author�F�y�c����
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include"Object.h"
#include"renderer.h"
#include"model.h"
#include"texture.h"

class CGage;

class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 3);
	~CObjectX();

	// �����̃^�C�v
	enum ObjectX_Type
	{
		OBJECTX_WALL = 0,	// ��
		OBJECTX_CHASE_WALL,	// �G���ǂ������Ă����
		OBJECTX_ROBOT_WALL,	// ���{�b�g���ʂ���
		OBJECTX_KEY_RED,	// ��(��)
		OBJECTX_KEY_BLUE,	// ��(��)
		OBJECTX_KEY_GREEN,	// ��(��)
		OBJECTX_DOOR_RED,	// �h�A(��)
		OBJECTX_DOOR_BLUE,	// �h�A(��)
		OBJECTX_DOOR_GREEN,	// �h�A(��)
		OBJECTX_DOOR,		// �h�A(���Ȃ�)
		OBJECTX_DOORWALL,	// �h�A�̏�̕�
		OBJECTX_PC,			// �p�\�R��
		OBJECTX_BUTTON,		// �{�^��
		OBJECTX_GOAL,		// �S�[��
		OBJECTX_CEILING,	// �V��
		OBJECTX_BULLET,		// �e�e
		OBJECT_MAX,
		OBJECTX_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void DrawShadow();		// �e
	void Stencil();			// �X�e���V��
	bool CharCollision(CModel::EModel_Type type);	// �L�����̓����蔻��
	bool Collision(D3DXVECTOR3 pos);			// �I�u�W�F�N�g�̓����蔻��
	void CollisionType();	// �^�C�v���Ƃ̔���
	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, int modeltype, ObjectX_Type type);
	void ObjectXCol();		// �F�̐ݒ�
	void DoorMove();		// �h�A�̓���
	bool OpenDoor();		// �h�A���󂢂Ă��邩

	void SetXFileName(const char *FileName) { m_FileName = FileName; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetVecAxis(D3DXVECTOR3 vecAxis) { m_vecAxis = vecAxis; }
	void SetValue(float Value) { m_fValue = Value; }
	void SetType(ObjectX_Type type) { m_nObjectType = type; }
	void SetModelType(int modeltype) { m_nModelType = modeltype; }
	static void SetNumObject(int num) { m_nNumObject = num; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }

	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetMax() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMin() { return m_vtxMinModel; }
	ObjectX_Type GetObjectXType() { return m_nObjectType; }
	int GetModelType() { return m_nModelType; }
	static int GetNumObject() { return m_nNumObject; }
	static bool GetGoalOpen() { return m_bGoalOpen; }

	static const int m_nMax_Model = 128;	// �I�u�W�F�N�gX�̍ő吔
private:
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_posOld;			// �O��̈ʒu
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_Initrot;			// �ŏ��̌���
	D3DXVECTOR3 m_Size;				// �T�C�Y
	D3DXVECTOR3 m_vtxMinModel;		// ���_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;		// ���_�̍ő�l
	D3DXVECTOR3 m_vecDir;			// �v���C���[�̕����x�N�g��
	LPD3DXBUFFER m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	LPD3DXMESH m_pMesh;				// ���b�V�����ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	DWORD m_nNumMat;				// �}�e���A�����̐�
	D3DXMATRIX m_mtxRot;			// ��]�}�g���b�N�X
	D3DXVECTOR3 m_vecAxis;			// ��]��
	ObjectX_Type m_nObjectType;		// �^�C�v
	int m_nModelType;				// ���f���̃^�C�v
	int m_nGoalColCount;			// �S�[���̐F���ς��܂ł̎���
	static int m_nNumObject;		// �I�u�W�F�N�g�̐�
	static int m_nNumHack;			// �n�b�L���O��
	float m_fRadius;				// �I�u�W�F�N�g�̔��a
	float m_fAngle;					// �v���C���[�ƒ��Ԃ̊p�x
	float m_fDoorSpeed;				// �h�A���J���X�s�[�h
	float m_fValue;					// ��]�p
	float m_Alpha;					// �A���t�@�l
	bool m_bAction;					// �A�N�V�����ł��邩
	bool m_bGageFull_Up;			// �Q�[�W�����^���ɂȂ�����
	bool m_bZeroAlpha;				// �Q�[�W�̃A���t�@�l��0�ɂȂ�����
	bool m_bHack;					// �n�b�L���O������
	bool m_bButton_Player;			// �v���C���[���{�^����������
	bool m_bButton_Robot;			// ���{�b�g���{�^����������
	static bool m_bGoalOpen;		// �S�[�����J������
	const char *m_FileName;			// �t�@�C���̖��O
	CGage *m_pGage;					// �Q�[�W�̏��
	CTexture::TEXTURE m_texture;	// �e�N�X�`��
	D3DXCOLOR m_Col;				// �F

	static const int m_nNumDoor = 4;	// �h�A�̎�ނ̐�
	
	bool m_bOpenDoor[m_nNumDoor];					// �h�A���J����
	static bool m_bGetKey[m_nNumDoor - 1];			// �����������
	static bool m_bGetKey_Robot[m_nNumDoor - 1];	// ���{�b�g�������������
};

#endif // !_OBJECTX_H_