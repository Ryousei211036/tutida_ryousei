//==============================================================================================
//
// アプリケーション　　　application.h
// 土田凌聖
//
//==============================================================================================
#ifndef _APPLICATION_H_
#define	_APPLICATION_H_

#include"renderer.h"
#include"texture.h"

//前方宣言
class CInputkeyboard;
class CTitle;
class CGame;
class CResult;
class CScore;
class CSound;

class CApplication
{
public:
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX,
	};

	CApplication();
	~CApplication();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	static void SetMode(MODE mode);

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CInputkeyboard *GetInputKeyboard() { return m_pInputkeyboard; }
	static CTexture *GetTexture() { return m_pTexture; }
	static CSound *GetSound() { return m_pSound; }
	static MODE GetMode() { return m_mode; }

private:
	static CRenderer *m_pRenderer;
	static CInputkeyboard *m_pInputkeyboard;
	static CTexture *m_pTexture;
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CSound *m_pSound;
	static MODE m_mode;
};

#endif // !_APPLICATION_H_
