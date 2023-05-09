//==============================================================================================
//
// �L�[�{�[�h�@�@�@inputkeyboard.h
// �y�c����
//
//==============================================================================================
#ifndef _INPUTKEYBOARD_H_
#define	_INPUTKEYBOARD_H_

#define NUM_KEY_MAX		(256)	//�L�[�̍ő吔

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
	BYTE m_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̃v���X���
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X���
};

#endif // !_KEYBOARD_H_
