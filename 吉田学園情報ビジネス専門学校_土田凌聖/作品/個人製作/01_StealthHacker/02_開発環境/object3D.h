//==============================================================================================
//
// 3Dポリゴン　　　Object3D.h
// Author : 土田凌聖
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
	const float SCREEN_WIDTH = 1280;		// スクリーンの幅
	const float SCREEN_HEIGHT = 720;		// スクリーンの高さ

	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//頂点座標
		D3DXVECTOR3 nor;		//法線ベクトル
		D3DCOLOR col;			//頂点カラー
		D3DXVECTOR2 tex;		//テクスチャ座標
	};

	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_Size;						// サイズ
	float m_SizeX;							// 長さ変える用
	D3DXVECTOR3 m_Pos;						// ポリゴンの位置
	D3DXCOLOR m_Col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	bool m_bBillboard;						// ビルボードか
	EObjType m_ObjType;						// オブジェクトのタイプ
};

#endif // !_OBJECT3D_H_