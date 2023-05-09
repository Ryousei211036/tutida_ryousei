//==============================================================================================
//
// タイトル画面　　 title.h
// 土田凌聖
//
//==============================================================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

#include"object2d.h"

#define TITLE_WIDTH			(910.0f)		// タイトルの幅
#define TITLE_HEIGHT		(510.0f)		// タイトルの高さ

class CBg;

class CTitle : public CObject2D
{
public:
	CTitle();
	~CTitle() override;

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CTitle *Create();

private:
	D3DXVECTOR3 m_Pos;				// 位置
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
};

#endif // !_TITLE_H_
