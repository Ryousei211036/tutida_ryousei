//==============================================================================================
//
// UI　　　UI.h
// 土田凌聖
//
//==============================================================================================
#ifndef _UI_H_
#define	_UI_H_

#include"object2D.h"

class CUI : public CObject2D
{
public:
	CUI();
	~CUI();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CUI *Create(D3DXVECTOR3 pos, CTexture::TEXTURE texture, float width, float height, int flashingtime, int flashingtime2);
	void Flash(int flashingtime);

	void SetFlashigTime(int flashingtime, int flashingtime2);

private:
	D3DXVECTOR3 m_Pos;						// 位置
	float m_fWidth;							// 幅
	float m_fHeight;						// 高さ
	CTexture::TEXTURE *m_pTexture;			// テクスチャ
	int m_nFlashingTime, m_nFlashingTime2;	// 点滅する間隔
	int m_nFlashingCount;					// 点滅のカウント
	bool m_bEnter;							// エンターキーを押したか
};

#endif // !_GAMEUI_H_
