//==============================================
//
//���b�V���t�B�[���h�̏���[meshfield.h]
//Author�F�y�c����
//
//==============================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include"renderer.h"
#include"Object3D.h"
#include"texture.h"

class CMeshField : CObject3D
{
public:
	CMeshField(/*int nPriority = 3*/);
	~CMeshField() override;

	HRESULT Init() override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Stencil();
	static CMeshField *Create(D3DXVECTOR3 pos, int Xblock, int Zblock, float size);
	void MeshCollision();
	void Normal();

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetNumBlock(int Xblock, int Zblock) { m_nXBlock = Xblock; m_nZBlock = Zblock; }
	void SetSize(float size) { m_fSize = size; }

private:
	int m_nVertexNum;						// ���_��
	int m_nIndexNum;						// �C���f�b�N�X�o�b�t�@
	int m_nPrimitiveNum;					// �v���~�e�B�u��
	int m_nXBlock;							// X�����̃u���b�N�̐�
	int m_nZBlock;							// Z�����̃u���b�N�̐�
	int m_nHeight;							// ���_�̍���
	int m_nVecCount;						// �O�ς��}�C�i�X�ɂȂ�����
	int m_nPolyCount;						// 
	float m_fSize;							// �T�C�Y
	D3DXVECTOR3 m_Pos;						// �ʒu
	D3DXVECTOR3 m_Rot;						// ����
	D3DXMATRIX m_MtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_NorVec;					// �@���ʂ̃x�N�g��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	CTexture::TEXTURE m_Texture;			// �e�N�X�`��
};

#endif // !_MESHFIELD_H_