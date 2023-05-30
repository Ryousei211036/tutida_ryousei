//==============================================================================================
//
// �L�[�{�[�h�@�@�@inputkeyboard.h
// �y�c����
//
//==============================================================================================
#ifndef _INPUTKEYBOARD_H_
#define	_INPUTKEYBOARD_H_

#include"input.h"
#include"object.h"
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")		//���͏����ɕK�v

class CInputkeyboard : public CInput
{
public:
	CInputkeyboard();
	~CInputkeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	void Uninit() override;
	void Update() override;

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);

private:
	static const int m_nNum_KeyMax = 256;		// �L�[�̍ő吔
	BYTE m_aKeyState[m_nNum_KeyMax];			// �L�[�{�[�h�̃v���X���
	BYTE m_aKeyStateTrigger[m_nNum_KeyMax];	// �L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[m_nNum_KeyMax];	// �L�[�{�[�h�̃����[�X���
};

#endif // !_KEYBOARD_H_
