//==============================================================================================
//
// レベルの表示　　　leveldisplay.h
// 土田凌聖
//
//==============================================================================================
#ifndef _LEVELDISPLAY_H_
#define	_LEVELDISPLAY_H_

#include"score.h"
#include"texture.h"

#define	LEVEL_NUM	(2)		// レベルの桁数

class CScore;

class CLevelDisplay : public CScore
{
public:
	CLevelDisplay();
	~CLevelDisplay();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CLevelDisplay *Create(D3DXVECTOR3 pos, float width, float height, D3DXCOLOR col);
	void AddLevel(int statuslevel, int maxlevel, D3DXVECTOR3 maxpos, EObjType type);

	void SetSize(float width, float height);
	void SetLevel(int statuslevel, int maxlevel, D3DXVECTOR3 maxpos);
	void SetCol(D3DXCOLOR col);
	void SetAlpha(float alpha);

private:
	CNumber *m_pNumber[LEVEL_NUM];	// 数字の情報
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	int m_nLevel;					// レベル
	float R = 1.0f, G = 1.0f, B = 1.0f;
	CTexture::TEXTURE m_pTexture;	// テクスチャ変更用
};

#endif // !_LEVELDISPLAY_H_
