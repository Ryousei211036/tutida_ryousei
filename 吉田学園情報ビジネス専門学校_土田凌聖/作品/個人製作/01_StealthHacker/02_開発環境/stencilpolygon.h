//==============================================================================================
//
// �X�e���V���p�|���S���@�@�@stencilpolygon.h
// �y�c����
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
	float m_fAlpha;		// �A���t�@�l
	int m_nAlphaSpeed;	// �_�ł���X�s�[�h
	bool m_bUse;		// �g����
};

#endif // !_STENCILPOLYGON_H_
