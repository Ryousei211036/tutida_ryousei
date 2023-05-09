//==============================================================================================
//
// オブジェクト　　　object.h
// Author : 土田凌聖
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

	void Setposition(D3DXVECTOR3 pos);												// 位置の設定
	void SetSize(float width, float height);										// サイズの設定
	void SetGagelength(float width, float width2, float height);					// ゲージのサイズの設定
	void SetLogoSize(float width, float height, float height2);						// ロゴのサイズの設定
	void AddSize(float add, float max);
	void Setrot(float rot);															// 角度の設定
	void SetTexPos(float top, float row, float right, float left);					// テクスチャ座標の設定
	void AnimTexture(int nPattern, int nPatternMax);
	void SetTexture(CTexture::TEXTURE texture);										// テクスチャの設定
	bool SetDisappear(float size, float alpha, float sizespeed, float alphaspeed);	// 消える設定
	void SetCol(D3DXCOLOR col);														// 色の設定
	void SetGageSize(float Gagesize);												// ゲージの長さの設定

protected:
	const float SCREEN_WIDTH = 1280;		// スクリーンの幅
	const float SCREEN_HEIGHT = 720;		// スクリーンの高さ

private:
	struct VERTEX_2D		// 頂点データ
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	float m_fWidth, m_fWidth2;				// 幅
	float m_fHeight, m_fHeight2;			// 高さ
	float m_fInitWidth, m_fInitWidth2;		// 幅(初期値)
	float m_fInitHeight, m_fInitHeight2;	// 高さ(初期値)
	float m_fAngle;							// 対角線の角度を算出する
	float m_fRot;							// ポリゴンの角度
	D3DXVECTOR3 m_Pos;						// ポリゴンの位置
	D3DXCOLOR m_Col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	EObjType m_ObjType;						//オブジェクトのタイプ
};

#endif // !_OBJECT_H_
