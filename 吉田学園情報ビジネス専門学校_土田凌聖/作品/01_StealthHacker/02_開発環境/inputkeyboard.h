//==============================================================================================
//
// キーボード　　　inputkeyboard.h
// 土田凌聖
//
//==============================================================================================
#ifndef _INPUTKEYBOARD_H_
#define	_INPUTKEYBOARD_H_

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
	static const int m_nNum_KeyMax = 256;		// キーの最大数
	BYTE m_aKeyState[m_nNum_KeyMax];			// キーボードのプレス情報
	BYTE m_aKeyStateTrigger[m_nNum_KeyMax];	// キーボードのトリガー情報
	BYTE m_aKeyStateRelease[m_nNum_KeyMax];	// キーボードのリリース情報
};

#endif // !_KEYBOARD_H_
