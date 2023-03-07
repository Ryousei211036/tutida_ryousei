//==============================================================================================
//
// �A�v���P�[�V�����@�@�@application.h
// �y�c����
//
//==============================================================================================
#ifndef _APPLICATION_H_
#define	_APPLICATION_H_

#include"renderer.h"
#include"texture.h"

//�O���錾
class CInputkeyboard;
class CInputJoypad;
class CTitle;
class CGame;
class CResult;
class CSound;
class CLight;
class CPlayer;
class CObject;
class CDebugProc;

class CApplication
{
public:
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		MODE_TUTORIAL,
		MODE_MAX,
	};

	CApplication();
	~CApplication();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	static void SetMode(MODE mode);		// ���[�h�̐ݒ�

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static CInputkeyboard *GetInputKeyboard() { return m_pInputkeyboard; }
	static CInputJoypad *GetInputJoypad() { return m_pInputJoypad; }
	static CTexture *GetTexture() { return m_pTexture; }
	static CSound *GetSound() { return m_pSound; }
	static CLight *GetLight() { return m_pLight; }
	static MODE GetMode() { return m_mode; }

private:
	static CRenderer *m_pRenderer;
	static CInputkeyboard *m_pInputkeyboard;
	static CInputJoypad *m_pInputJoypad;
	static CTexture *m_pTexture;
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	static CSound *m_pSound;
	static CLight *m_pLight;
	static MODE m_mode;
	static CObject *m_pGameMode;

	CDebugProc *m_pDebugProc;
	bool m_bWireFrame;
};

#endif // !_APPLICATION_H_
