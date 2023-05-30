//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.h
// Author : �y�c����
//
//==============================================================================================
#ifndef _OBJECT2D_H_
#define	_OBJECT2D_H_

#include <d3dx9.h>
#include"Object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;

class CObject2D : public CObject
{
public:
	CObject2D(int nPriority);
	~CObject2D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Stencil();
	static CObject2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void Setposition(D3DXVECTOR3 pos);												// �ʒu�̐ݒ�
	void SetSize(float width, float height);										// �T�C�Y�̐ݒ�
	void SetGagelength(float width, float width2, float height);					// �Q�[�W�̃T�C�Y�̐ݒ�
	void SetLogoSize(float width, float height, float height2);						// ���S�̃T�C�Y�̐ݒ�
	void AddSize(float add, float max);
	void Setrot(float rot);															// �p�x�̐ݒ�
	void SetTexPos(float top, float row, float right, float left);					// �e�N�X�`�����W�̐ݒ�
	void AnimTexture(int nPattern, int nPatternMax);
	void SetTexture(CTexture::TEXTURE texture);										// �e�N�X�`���̐ݒ�
	bool SetDisappear(float size, float alpha, float sizespeed, float alphaspeed);	// ������ݒ�
	void SetCol(D3DXCOLOR col);														// �F�̐ݒ�
	void SetGageSize(float Gagesize);												// �Q�[�W�̒����̐ݒ�

protected:
	const float SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const float SCREEN_HEIGHT = 720;		// �X�N���[���̍���

private:
	struct VERTEX_2D		// ���_�f�[�^
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// ���_�t�H�[�}�b�g
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	float m_fWidth, m_fWidth2;				// ��
	float m_fHeight, m_fHeight2;			// ����
	float m_fInitWidth, m_fInitWidth2;		// ��(�����l)
	float m_fInitHeight, m_fInitHeight2;	// ����(�����l)
	float m_fAngle;							// �Ίp���̊p�x���Z�o����
	float m_fRot;							// �|���S���̊p�x
	D3DXVECTOR3 m_Pos;						// �|���S���̈ʒu
	D3DXCOLOR m_Col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	EObjType m_ObjType;						//�I�u�W�F�N�g�̃^�C�v
};

#endif // !_OBJECT_H_
