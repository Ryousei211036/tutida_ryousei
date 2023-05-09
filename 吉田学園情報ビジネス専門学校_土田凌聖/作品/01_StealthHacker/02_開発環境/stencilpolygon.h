//==============================================================================================
//
// ステンシル用ポリゴン　　　stencilpolygon.h
// 土田凌聖
//
//==============================================================================================
#ifndef _STENCILPOLYGON_H_
#define	_STENCILPOLYGON_H_

#include"object2D.h"

class CStencilPolygon : public CObject2D
{
public:
	CStencilPolygon();
	~CStencilPolygon();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Stencil();
	static CStencilPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void SetUse(bool use) { m_bUse = use; }

private:
	float m_fAlpha;		// アルファ値
	int m_nAlphaSpeed;	// 点滅するスピード
	bool m_bUse;		// 使うか
};

#endif // !_STENCILPOLYGON_H_
