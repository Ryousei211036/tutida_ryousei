//==============================================================================================
//
// 入力処理　　　input.h
// 土田凌聖
//
//==============================================================================================
#ifndef _INPUT_H_
#define	_INPUT_H_

#include"object.h"
#include"dinput.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")		//入力処理に必要

class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDecive;
};

#endif // !_INPUT_H_
