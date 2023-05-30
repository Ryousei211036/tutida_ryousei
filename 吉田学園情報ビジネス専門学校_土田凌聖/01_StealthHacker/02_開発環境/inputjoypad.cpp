//==============================================================================================
//
// ジョイパッド　　　inputjoypad.cpp
// 土田凌聖
//
//==============================================================================================
#include"inputjoypad.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CInputJoypad::CInputJoypad()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CInputJoypad::~CInputJoypad()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Xinputのステート設定(有効にする)
	XInputEnable(true);
	//メモリーのクリア
	memset(&m_JoykeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_JoykeyStateTrigger, 0, sizeof(XINPUT_STATE));

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CInputJoypad::Uninit()
{
	//Xinputのステートを設定(無効にする)
	XInputEnable(false);
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;	//ジョイパッドの入力処理

	//ジョイパッドの状態の取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)		//0番目のジョイパッド
	{
		m_aJoykeyStateTrigger = ~m_JoykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;
		m_JoykeyState = joykeyState;	//ジョイパッドのプレス情報を保存
	}
}

//============================================
// ジョイパッドプレスの取得処理
//============================================
bool CInputJoypad::GetJoypadPress(JOYPAD key)
{
	return (m_JoykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
//============================================
// ジョイパッドトリガーの取得処理
//============================================
bool CInputJoypad::GetJoypadTrigger(JOYPAD key)
{
	return(m_aJoykeyStateTrigger & (0x01 << key)) ? true : false;
}

D3DXVECTOR3 CInputJoypad::GetJoypadStick(JOYPAD Key)
{
	switch (Key)
	{
	case JOYKEY_LEFT_STICK:
		m_JoyStickPos = D3DXVECTOR3(m_JoykeyState.Gamepad.sThumbLX / 32767.0f, -m_JoykeyState.Gamepad.sThumbLY / 32767.0f, 0.0f);
		break;
	case JOYKEY_RIGHT_STICK:
		m_JoyStickPos = D3DXVECTOR3(m_JoykeyState.Gamepad.sThumbRX / 32767.0f, -m_JoykeyState.Gamepad.sThumbRY / 32767.0f, 0.0f);
		break;
	default:
		break;
	}

	return m_JoyStickPos;
}

int CInputJoypad::GetJoypadTriggerPedal(JOYPAD Key)
{
	int nJoypadTriggerPedal = 0;
	switch (Key)
	{
	case JOYKEY_LEFT_TRIGGER:
		nJoypadTriggerPedal = m_JoykeyState.Gamepad.bLeftTrigger;
		break;
	case JOYKEY_RIGHT_TRIGGER:
		nJoypadTriggerPedal = m_JoykeyState.Gamepad.bRightTrigger;
		break;
	default:
		break;
	}

	return nJoypadTriggerPedal;
}