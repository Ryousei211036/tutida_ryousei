//==============================================================================================
//
// �I�u�W�F�N�g�̔z�u�@�@�@setobject.h
// �y�c����
//
//==============================================================================================
#ifndef _SETOBJECT_H_
#define	_SETOBJECT_H_

#include <d3dx9.h>

class CObjectX;

class CSetObject
{
public:
	CSetObject();
	~CSetObject();

	// �t�@�C���l�[��
	enum Object_Name
	{
		NAME_WALL = 0,		// ��
		NAME_ROBOT_WALL,	// ���{�b�g���ʂ���
		NAME_KEY,			// ��
		NAME_DOOR_WIDTH,	// �h�A(��)
		NAME_DOOR2_LENGTH,	// �h�A(�c)
		NAME_DOORWALL,		// �h�A�̏�̕�
		NAME_PC,			// �p�\�R��				
		NAME_TABLE,			// �e�[�u��
		NAME_BUTTON,		// �{�^��
		NAME_GOAL,			// �S�[��
		NAME_MAX,
		NAME_NONE
	};

	HRESULT Init();
	void Update();
	void SetObject();
	void Move();
	void SaveFile();

	static CSetObject *Create();

	D3DXVECTOR3 GetPos() { return m_Pos; }
	
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	D3DXVECTOR3 m_Size;
	int m_nIndex;
	int m_nObjectNum;			// �I�u�W�F�N�g�̔ԍ�
	int m_nObjectType;			// �I�u�W�F�N�g�̃^�C�v
	int m_nNumObject;			// �I�u�W�F�N�g�̐�
	static const char *m_sFileName[];	// �t�@�C���p�X
	static CObjectX **pObject;
	int m_nControl;
};

#endif // !_SETOBJECT_H_
