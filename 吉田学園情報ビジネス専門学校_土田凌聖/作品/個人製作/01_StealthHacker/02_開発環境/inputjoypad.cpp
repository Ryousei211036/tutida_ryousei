//==============================================================================================
//
// �W���C�p�b�h�@�@�@inputjoypad.cpp
// �y�c����
//
//==============================================================================================
#include"inputjoypad.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CInputJoypad::CInputJoypad()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CInputJoypad::~CInputJoypad()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Xinput�̃X�e�[�g�ݒ�(�L���ɂ���)
	XInputEnable(true);
	//�������[�̃N���A
	memset(&m_JoykeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_JoykeyStateTrigger, 0, sizeof(XINPUT_STATE));

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CInputJoypad::Uninit()
{
	//Xinput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CInputJoypad::Update()
{
	XINPUT_STATE joykeyState;	//�W���C�p�b�h�̓��͏���

	//�W���C�p�b�h�̏�Ԃ̎擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)		//0�Ԗڂ̃W���C�p�b�h
	{
		m_aJoykeyStateTrigger = ~m_JoykeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;
		m_JoykeyState = joykeyState;	//�W���C�p�b�h�̃v���X����ۑ�
	}
}

//============================================
// �W���C�p�b�h�v���X�̎擾����
//============================================
bool CInputJoypad::GetJoypadPress(JOYPAD key)
{
	return (m_JoykeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
//============================================
// �W���C�p�b�h�g���K�[�̎擾����
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