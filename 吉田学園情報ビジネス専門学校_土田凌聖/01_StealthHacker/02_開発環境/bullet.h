//==============================================================================================
//
// �e�@�@�@bullet.h
// �y�c����
//
//==============================================================================================
#ifndef _BULLET_H_
#define	_BULLET_H_

#include"objectX.h"

class CBullet : public CObjectX
{
public:
	CBullet();
	~CBullet();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, ObjectX_Type type, CModel::EModel_Type modeltype);

	void Hit();			// �e����������
	void Death();		// �e�̎���

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetType(ObjectX_Type type) { OBJECTX_BULLET; }
	void SetModelType(CModel::EModel_Type modeltype) { m_nType = modeltype; }

private:
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	CModel::EModel_Type m_nType;
	float m_fRadius;		// ���a
	float m_fSpeed;			// �ړ����x
	int m_nDeathTime;		// ������܂ł̎���
	int m_nAddDamage;		// �^����_���[�W
};

#endif // !_BULLET_H_
