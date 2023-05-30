//==============================================================================================
//
// 3D�|���S���@�@�@Object3D.h
// Author : �y�c����
//
//==============================================================================================
#ifndef _OBJECT3D_H_
#define	_OBJECT3D_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;

class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 4);
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool billboard);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; m_SizeX = size.x; }
	void SetSizeX(float sizeX) { m_SizeX = sizeX; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }
	void Setbillboard(bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }

protected:
	const float SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const float SCREEN_HEIGHT = 720;		// �X�N���[���̍���

	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//���_���W
		D3DXVECTOR3 nor;		//�@���x�N�g��
		D3DCOLOR col;			//���_�J���[
		D3DXVECTOR2 tex;		//�e�N�X�`�����W
	};

	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// ���_�t�H�[�}�b�g

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Size;						// �T�C�Y
	float m_SizeX;							// �����ς���p
	D3DXVECTOR3 m_Pos;						// �|���S���̈ʒu
	D3DXCOLOR m_Col;						// �F
	CTexture::TEXTURE m_texture;			// �e�N�X�`��
	bool m_bBillboard;						// �r���{�[�h��
	EObjType m_ObjType;						// �I�u�W�F�N�g�̃^�C�v
};

#endif // !_OBJECT3D_H_