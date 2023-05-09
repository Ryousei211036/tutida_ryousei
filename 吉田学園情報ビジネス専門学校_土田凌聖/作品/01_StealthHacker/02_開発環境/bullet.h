//==============================================================================================
//
// 弾　　　bullet.h
// 土田凌聖
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

	void Hit();			// 弾が当たった
	void Death();		// 弾の寿命

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetType(ObjectX_Type type) { OBJECTX_BULLET; }
	void SetModelType(CModel::EModel_Type modeltype) { m_nType = modeltype; }

private:
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	CModel::EModel_Type m_nType;
	float m_fRadius;		// 半径
	float m_fSpeed;			// 移動速度
	int m_nDeathTime;		// 消えるまでの時間
	int m_nAddDamage;		// 与えるダメージ
};

#endif // !_BULLET_H_
