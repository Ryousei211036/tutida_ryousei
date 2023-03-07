//==============================================================================================
//
// ”wŒi@@@bg.h
// “y“c—½¹
//
//==============================================================================================
#ifndef _BG_H_
#define	_BG_H_

#include"object2D.h"

#define BG_WIDTH				(3500.0f)		// ”wŒi‚Ì•
#define BG_HEIGHT				(3500.0f)		// ”wŒi‚Ì‚‚³

class CBg : public CObject2D
{
public:
	CBg();
	~CBg();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CBg *Create();

private:
	D3DXVECTOR3 m_Pos;				// ˆÊ’u
	D3DXVECTOR3 m_InitPos;			// ‰ŠúˆÊ’u
	D3DXVECTOR3 m_Move;				// ˆÚ“®—Ê
	float m_fWidth;					// •
	float m_fHeight;				// ‚‚³
};

#endif // !_BG_H_
