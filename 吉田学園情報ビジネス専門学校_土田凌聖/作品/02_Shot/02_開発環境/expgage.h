//==============================================================================================
//
// 経験値ゲージ　　　expgage.h
// 土田凌聖
//
//==============================================================================================
#ifndef _EXPGAGE_H_
#define	_EXPGAGE_H_

#define	EXPGAGE_WIDTH		(900)		// 経験値ゲージの幅

#include"object2D.h"

class CExpgage : public CObject2D
{
public:
	CExpgage();
	~CExpgage();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	static CExpgage *Create(D3DXVECTOR3 pos, float width, float width2, float height, D3DXCOLOR col);

	bool AddExpgage(float Exp);
	void ExpGage(LPD3DXFONT pFont);

private:
	float m_fExpWidth;		// 経験値ゲージの長さ
	float m_fTotalExp;		// 経験値の合計
	float m_fExtraExp;		// ゲージをはみ出た分
	int m_LevelUpCount;		// レベルアップした回数
};

#endif // !_EXPGAGE_H_
