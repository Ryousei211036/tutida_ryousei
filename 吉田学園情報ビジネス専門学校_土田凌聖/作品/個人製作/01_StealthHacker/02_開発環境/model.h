//==============================================================================================
//
// ���f���@�@�@madel.h
// �y�c����
//
//==============================================================================================
#ifndef _MODEL_H_
#define	_MODEL_H_

#include"renderer.h"

class CModel
{
public:
	CModel();
	~CModel();

	// ���f���̃^�C�v
	enum EModel_Type
	{
		MODEL_TYPE_PLAYER = 0,
		MODEL_TYPE_ROBOT,
		MODEL_TYPE_ENEMY,
		MODEL_TYPE_MAX,
		MODEL_TYPE_NONE
	};

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw(D3DXMATRIX mtxworld);

	void DrawShadow();
	void Stencil();
	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename, EModel_Type type);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	void SetXFileIndex(int Index) { m_nIndex = Index; }
	void SetXFileName( const char *filename) { m_XFileName = filename; }
	void SetType(EModel_Type type) { m_nType = type; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	CModel GetParent() { return *m_pParent; }
	D3DXVECTOR3 GetMaxSize() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMinSize() { return m_vtxMinModel; }

private:
	static const int m_nMax_Parts = 128;	// ���f���̍ő吔
	struct Model
	{
		LPD3DXBUFFER m_pBuffMat;	// �}�e���A�����ւ̃|�C���^
		LPD3DXMESH m_pMesh;			// ���b�V�����ւ̃|�C���^
		DWORD m_nNumMat;			//�}�e���A�����̐�
	};

	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_Rot;			// ���݂̊p�x
	D3DXVECTOR3 m_vtxMinModel;	// ���_�̍ŏ��l
	D3DXVECTOR3 m_vtxMaxModel;	// ���_�̍ő�l
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxParent;		// �e�̃}�g���b�N�X
	CModel *m_pParent;			// �e���f���ւ̃|�C���^
	const char *m_XFileName;	// X�t�@�C���̖��O
	int m_nIndex;				// X�t�@�C���̔ԍ�
	Model m_Model[m_nMax_Parts];// ���f���\����
	EModel_Type m_nType;		// ���f���̃^�C�v
};

#endif // !_MODEL_H_
