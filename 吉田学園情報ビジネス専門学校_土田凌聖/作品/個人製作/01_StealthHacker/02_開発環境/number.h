//==============================================================================================
//
// 数字　　　number.h
// 土田凌聖
//
//==============================================================================================
#ifndef _NUMBER_H_
#define	_NUMBER_H_

#include"object2D.h"

class CNumber : public CObject2D
{
public:
	CNumber();
	~CNumber() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CNumber *Create(D3DXVECTOR3 pos, float width, float height);

	void Settexture(CTexture::TEXTURE *pTexture);
	void Setcollar(D3DXCOLOR *pCol);

private:
	CTexture::TEXTURE *m_pTexture;
	float m_fDisSize;
	float m_fDisAlpha;
	float m_fDisSizeSpeed;
	float m_fDisAlphaSpeed;
};

#endif // !_NUMBER_H_
