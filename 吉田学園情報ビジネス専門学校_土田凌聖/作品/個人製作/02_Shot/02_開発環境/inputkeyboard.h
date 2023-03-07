//==============================================================================================
//
// キーボード　　　inputkeyboard.h
// 土田凌聖
//
//==============================================================================================
#ifndef _INPUTKEYBOARD_H_
#define	_INPUTKEYBOARD_H_

#define NUM_KEY_MAX		(256)	//キーの最大数

#include"input.h"
#include"object.h"
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")		//入力処理に必要

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
	BYTE m_aKeyState[NUM_KEY_MAX];			// キーボードのプレス情報
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報
};

#endif // !_KEYBOARD_H_
