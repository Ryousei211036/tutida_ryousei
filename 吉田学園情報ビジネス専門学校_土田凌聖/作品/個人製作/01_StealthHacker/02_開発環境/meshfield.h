//==============================================
//
//メッシュフィールドの処理[meshfield.h]
//Author：土田凌聖
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
	int m_nVertexNum;						// 頂点数
	int m_nIndexNum;						// インデックスバッファ
	int m_nPrimitiveNum;					// プリミティブ数
	int m_nXBlock;							// X方向のブロックの数
	int m_nZBlock;							// Z方向のブロックの数
	int m_nHeight;							// 頂点の高さ
	int m_nVecCount;						// 外積がマイナスになった回数
	int m_nPolyCount;						// 
	float m_fSize;							// サイズ
	D3DXVECTOR3 m_Pos;						// 位置
	D3DXVECTOR3 m_Rot;						// 向き
	D3DXMATRIX m_MtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_NorVec;					// 法線面のベクトル
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファへのポインタ
	CTexture::TEXTURE m_Texture;			// テクスチャ
};

#endif // !_MESHFIELD_H_