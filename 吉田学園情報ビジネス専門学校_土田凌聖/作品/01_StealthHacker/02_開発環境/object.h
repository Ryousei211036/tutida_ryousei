//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : �y�c����
//
//==============================================================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_

#include <d3dx9.h>

#define	MAX_PRI			(6)			// �D�揇�ʂ̍ő吔

class CObject
{
public:
	// �I�u�W�F�N�g�̎��
	enum EObjType
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,		// �v���C���[
		OBJTYPE_ENEMY,		// �G
		OBJTYPE_ROBOT,		// ���{�b�g
		OBJTYPE_OBJECTX,	// �I�u�W�F�N�gX
		OBJTYPE_BULLET,		// �e
		OBJTYPE_EXP,		// �a
		OBJTYPE_HP,			// �̗̓Q�[�W
		OBJTYPE_MODE,		// �Q�[�����[�h
		OBJTYPE_MAX
	};

	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();
	void Death(int nPriority);
	float Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);		// ��̈ʒu�̋���

	CObject *GetNext() { return m_pNext; }
	static CObject *GetTop(int Pri) { return m_pTop[Pri]; }
	virtual EObjType GetObjType() { return OBJTYPE_NONE; }		// �I�u�W�F�N�g�^�C�v�̎擾

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRadius(float radius) { m_fRadius = radius; }
	void SetHp(int hp) { m_nHp = hp; }
	void SetHit(bool hit) { m_bHit = hit; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	float GetRadius() { return m_fRadius; }
	int GetHp() { return m_nHp; }
	bool GetHit() { return m_bHit; }
	bool GetDeathFlag() { return m_bDeath; }

private:
	int m_nPriority;
	CObject *m_pPrev;						// ��O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject *m_pNext;						// ���̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject *m_pTop[MAX_PRI];		// �擪�̃I�u�W�F�N�g�̂ւ̃|�C���^
	static CObject *m_pCurrent[MAX_PRI];	// ����(��Ԍ��)�̃I�u�W�F�N�g�̂ւ̃|�C���^
	bool m_bDeath;							// ���S�t���O
	D3DXVECTOR3 m_Pos;
	float m_fRadius;
	int m_nHp;
	bool m_bHit;
};

#endif // !_OBJECT_H_
