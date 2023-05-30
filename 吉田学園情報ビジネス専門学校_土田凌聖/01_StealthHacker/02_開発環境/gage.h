//==============================================================================================
//
// ゲージ　　　gage.h
// 土田凌聖
//
//==============================================================================================
#ifndef _GAGE_H_
#define	_GAGE_H_

#include"object3D.h"

class CGage : public CObject3D
{
public:
	CGage(int nPriority = 5);
	~CGage() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CGage *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool billboard);
	bool AddGage(float add);		// ゲージを増やす
	void SubGage(float sub);		// ゲージを減らす
	bool SetAlpha(float alpha);		// アルファ値の設定
	void Display(bool display);		// ゲージの表示
	void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool billboard);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetLength(D3DXVECTOR3 size) { m_fLength = size.x; }
	void SetEnd_Length(D3DXVECTOR3 size) { m_fEnd_Length = size.x; }
	void SetResetAlpha(float alpha) { m_fAlpha = alpha; }
	void SetResetCount(int count) { m_nAlphaCount = count; }

	float GetEnd_Lenght() { return m_fEnd_Length; }

private:
	D3DXVECTOR3 m_Pos;		// 位置
	int m_nAlphaCount;		// ゲージが消えるまでの時間
	float m_fAlpha;			// アルファ値
	float m_fLength;		// 長さ(始)
	float m_fEnd_Length;	// 長さ(終)
	bool m_bDisplay;		// 表示するか
	CObject3D *pObject3D;
};

#endif // !_HACKGAGE_H_
