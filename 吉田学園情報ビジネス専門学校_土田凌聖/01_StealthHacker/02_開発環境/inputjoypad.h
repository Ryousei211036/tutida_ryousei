//==============================================================================================
//
// ジョイパッド　　　inputjoypad.h
// 土田凌聖
//
//==============================================================================================
#ifndef _INPUTJOYPAD_H_
#define	_INPUTJOYPAD_H_

#include"input.h"
#include"object.h"
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")		//入力処理に必要
#pragma comment(lib,"xinput.lib")	//ジョイパッド処理に必要
#include"input.h"
#include"Xinput.h"

class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad() override;

	enum JOYPAD
	{
		JOYKEY_UP = 0,				// 十字ボタン上
		JOYKEY_DOWN,				// 十字ボタン下
		JOYKEY_LEFT,				// 十字ボタン左
		JOYKEY_RIGHT,				// 十字ボタン右
		JOYKEY_START,				// スタートボタン
		JOYKEY_BACK,				// バックボタン
		JOYKEY_LEFT_THUMB,			// 左スティック押込み
		JOYKEY_RIGHT_THUMB,			// 右スティック押込み
		JOYKEY_LEFT_BUTTON,			// L１ボタン
		JOYKEY_RIGHT_BUTTON,		// R１ボタン
		JOYKEY_LEFT_TRIGGER,		// L２トリガー
		JOYKEY_RIGHT_TRIGGER,		// R２トリガー
		JOYKEY_A,					// Aボタン
		JOYKEY_B,					// Bボタン
		JOYKEY_X,					// Xボタン
		JOYKEY_Y,					// Yボタン
		JOYKEY_LEFT_STICK,			// 左スティック
		JOYKEY_RIGHT_STICK,			// 右スティック
		JOYKEY_MAX,
	};

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;
	virtual void Uninit() override;
	virtual void Update() override;

	bool GetJoypadPress(JOYPAD key);
	bool GetJoypadTrigger(JOYPAD key);
	D3DXVECTOR3 GetJoypadStick(JOYPAD Key);
	int GetJoypadTriggerPedal(JOYPAD Key);
private:
	XINPUT_STATE m_JoykeyState;						//ジョイパッドのプレス情報
	XINPUT_STATE m_JoykeyStateTrigger;				//ジョイパッドのトリガー情報
	int m_aJoykeyStateTrigger;						//ジョイパッドのトリガー情報
	D3DXVECTOR3 m_JoyStickPos;			//ジョイスティックの傾き
};

#endif // !_INPUTJOYPAD_H_
