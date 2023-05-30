//==============================================================================================
//
// チュートリアルテキスト　　　tuotrial_text.h
// 土田凌聖
//
//==============================================================================================
#ifndef _TUTORIAL_TEXT_H_
#define	_TUTORIAL_TEXT_H_

#include"object2D.h"

class CObject2D;

class CTutorial_Text
{
public:
	enum Tutorial_Type
	{
		TYPE_OPENDOOR = 0,
		TYPE_LOCKED_DOOR,
		TYPE_LOCKED_DOOR2,
		TYPE_FIND_ENEMY,
		TYPE_HACKING,
		TYPE_SCOUTING,
		TYPE_GUN,
		TYPE_ROBOT,
		TYPE_BUTTON,
		TYPE_MAX,
		TYPE_NONE
	};

	CTutorial_Text();
	~CTutorial_Text();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CTutorial_Text *Create();

	static bool GetDisplay() { return m_bDisplay_Tutorial; }

private:
	struct Tutorial
	{
		D3DXVECTOR3 Pos;
		CTexture::TEXTURE Texture;
		float nRadius;
		bool bDisplay;
	};
	Tutorial m_nTutorial[TYPE_MAX];
	Tutorial_Type m_nType;
	D3DXVECTOR3 TexturePos;
	CObject2D *pObject2D[TYPE_MAX];
	static bool m_bDisplay_Tutorial;
};

#endif // !_TUTORIAL_TEXT_H_
