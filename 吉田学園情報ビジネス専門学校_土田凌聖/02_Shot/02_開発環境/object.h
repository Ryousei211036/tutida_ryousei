//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : �y�c����
//
//==============================================================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_

#include <d3dx9.h>

#define	MAX_OBJECT		(300)		// �I�u�W�F�N�g�̍ő吔
#define	MAX_PRI			(6)			// �D�揇�ʂ̍ő吔

class CBullet;
class CExp;

class CObject
{
public:
	//CObject();
	explicit CObject(int nPriority = 3);
	virtual ~CObject();

	// �I�u�W�F�N�g�̎��
	enum EObjType
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,		// �v���C���[
		OBJTYPE_ENEMY,		// �G
		OBJTYPE_BULLET,		// �e
		OBJTYPE_EXP,		// �a
		OBJTYPE_HP,			// �̗̓Q�[�W
		OBJTYPE_FADE,		// �t�F�[�h
		OBJTYPE_MAX
	};

	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();
	CObject *Getobject(int nPriority, int nNum);				// �I�u�W�F�N�g���̎擾

	virtual void SetLife(int type, float life) { type, life;}	// �̗͂̐ݒ�̐ݒ�
	virtual void SetExp(int exp) { exp; }						// �o���l�̐ݒ�̐ݒ�
	virtual void SetHpGage(float hpgage) { hpgage; }			// �̗̓Q�[�W�̒����̐ݒ�
	virtual void SetHit(bool hit) { hit; }						// �U��������������

	virtual float GetLife() { return 0.0f; }								// �̗͂̎擾
	virtual float GetAttack() { return 0.0f; }								// �U���͂̎擾
	virtual float GetBodyDamage() { return 0.0f; }						// �{�̃_���[�W�̎擾
	virtual int GetBulletLife() { return 0; }							// �e�̗̑͂̎擾
	virtual float GetBulletSpeed() { return 0.0f; }						// �e�̑��x�̎擾
	virtual D3DXVECTOR3 GetPos() { return{}; }							// �ʒu�̎擾
	virtual D3DXVECTOR3 GetMove() { return{}; }							// �ړ��ʂ̎擾
	virtual float GetSize() { return 0.0f; }								// �T�C�Y�̎擾
	virtual float GetRadius() { return 0.0f; }								// ���a�̎擾
	virtual int GetExp() { return 0; }									// �o���l�̎擾
	virtual EObjType GetObjType() { return OBJTYPE_NONE; }							// �I�u�W�F�N�g�^�C�v�̎擾
	virtual int GetNumTriangle() { return 0; }							// �O�p�̉a�̐�
	virtual int GetNumBox() { return 0; }								// �l�p�̉a�̐�
	virtual int GetNumPentagon() { return 0; }							// �܊p�`�̉a�̐�
	virtual float GetHpGage() { return 0.0f; }								// �̗̓Q�[�W�̒����̎擾

private:
	static CObject *m_apObject[MAX_PRI][MAX_OBJECT];
	static int m_nNumaAll;
	int m_nID;
	int m_nPriority;
};

#endif // !_OBJECT_H_
