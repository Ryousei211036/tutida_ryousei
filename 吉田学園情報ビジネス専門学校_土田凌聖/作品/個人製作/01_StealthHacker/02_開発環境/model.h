//==============================================================================================
//
// モデル　　　madel.h
// 土田凌聖
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

	// モデルのタイプ
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
	static const int m_nMax_Parts = 128;	// モデルの最大数
	struct Model
	{
		LPD3DXBUFFER m_pBuffMat;	// マテリアル情報へのポインタ
		LPD3DXMESH m_pMesh;			// メッシュ情報へのポインタ
		DWORD m_nNumMat;			//マテリアル情報の数
	};

	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_Rot;			// 現在の角度
	D3DXVECTOR3 m_vtxMinModel;	// 頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;	// 頂点の最大値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXMATRIX m_mtxParent;		// 親のマトリックス
	CModel *m_pParent;			// 親モデルへのポインタ
	const char *m_XFileName;	// Xファイルの名前
	int m_nIndex;				// Xファイルの番号
	Model m_Model[m_nMax_Parts];// モデル構造体
	EModel_Type m_nType;		// モデルのタイプ
};

#endif // !_MODEL_H_
