//==============================================================================================
//
// �i����̕\���@�@�@evodisplay.h
// �y�c����
//
//==============================================================================================
#ifndef _EVODISPLAY_H_
#define _EVODISPLAY_H_

#include"object2D.h"

class CTexture;

class CEvodisplay : public CObject2D
{
public:
	CEvodisplay();
	~CEvodisplay();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CEvodisplay *Create(D3DXVECTOR3 pos, float width, float height);

	void SetTexture(CTexture::TEXTURE Texture);		// �e�N�X�`���̐ݒ�

private:
	CTexture::TEXTURE m_texture;
};

#endif // !_EVODISPLAY_H_
