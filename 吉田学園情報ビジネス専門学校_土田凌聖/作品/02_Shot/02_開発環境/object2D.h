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

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void Setposition(D3DXVECTOR3 pos);												// 位置の設定
	void SetSize(float width, float height);										// サイズの設定
	void SetGagelength(float width, float width2, float height);					// ゲージのサイズの設定
	void Setrot(float rot);															// 角度の設定
	void SetTexPos(float top, float row, float right, float left);					// テクスチャ座標の設定
	void SetTexture(CTexture::TEXTURE texture);										// テクスチャの設定
	bool SetDisappear(float size, float alpha, float sizespeed, float alphaspeed);	// 消える設定
	void SetCol(D3DXCOLOR col);														// 色の設定
	void SetGageSize(float Gagesize);												// ゲージの長さの設定

	float		GetLife() override { return 0; }									// 体力の取得
	float		GetAttack() override { return 0.0f; }									// ダメージの取得
	float		GetBodyDamage() override { return 0.0f; }							// 本体ダメージの取得
	int			GetBulletLife() override { return 0; }								// 弾の体力の取得
	float		GetBulletSpeed() override { return 0.0f; };							// 弾の速度の取得
	D3DXVECTOR3 GetPos() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }			// 位置の取得
	D3DXVECTOR3 GetMove() override { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		// 移動量の取得
	float		GetSize() override { return 0.0f; }									// サイズの取得
	float		GetRadius() override { return 0.0f; }								// 半径の取得
	int			GetExp() override { return 0; }										// 経験値の取得
	EObjType	GetObjType() override { return OBJTYPE_NONE; }						// オブジェクトの種類の取得
	int			GetNumTriangle() override { return 0; }								// 三角の餌の数
	int			GetNumBox() override { return 0; }									// 四角の餌の数
	int			GetNumPentagon() override { return 0; }								// 五角形の餌の数
	float		GetHpGage() override { return 0.0f; }								// 体力ゲージの長さ

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
	float m_fWidth, m_fWidth2, m_fHeight;	// ポリゴンの幅と高さ
	float m_fAngle;							// 対角線の角度を算出する
	float m_fRot;							// ポリゴンの角度
	D3DXVECTOR3 m_Pos;						// ポリゴンの位置
	D3DXCOLOR m_Col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	EObjType m_ObjType;						//オブジェクトのタイプ
};

#endif // !_OBJECT_H_
