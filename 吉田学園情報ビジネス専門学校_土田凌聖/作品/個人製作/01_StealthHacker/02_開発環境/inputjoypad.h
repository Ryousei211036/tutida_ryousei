//==============================================================================================
//
// �W���C�p�b�h�@�@�@inputjoypad.h
// �y�c����
//
//==============================================================================================
#ifndef _INPUTJOYPAD_H_
#define	_INPUTJOYPAD_H_

#include"input.h"
#include"object.h"
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")		//���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	//�W���C�p�b�h�����ɕK�v
#include"input.h"
#include"Xinput.h"

class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad() override;

	enum JOYPAD
	{
		JOYKEY_UP = 0,				// �\���{�^����
		JOYKEY_DOWN,				// �\���{�^����
		JOYKEY_LEFT,				// �\���{�^����
		JOYKEY_RIGHT,				// �\���{�^���E
		JOYKEY_START,				// �X�^�[�g�{�^��
		JOYKEY_BACK,				// �o�b�N�{�^��
		JOYKEY_LEFT_THUMB,			// ���X�e�B�b�N������
		JOYKEY_RIGHT_THUMB,			// �E�X�e�B�b�N������
		JOYKEY_LEFT_BUTTON,			// L�P�{�^��
		JOYKEY_RIGHT_BUTTON,		// R�P�{�^��
		JOYKEY_LEFT_TRIGGER,		// L�Q�g���K�[
		JOYKEY_RIGHT_TRIGGER,		// R�Q�g���K�[
		JOYKEY_A,					// A�{�^��
		JOYKEY_B,					// B�{�^��
		JOYKEY_X,					// X�{�^��
		JOYKEY_Y,					// Y�{�^��
		JOYKEY_LEFT_STICK,			// ���X�e�B�b�N
		JOYKEY_RIGHT_STICK,			// �E�X�e�B�b�N
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
	XINPUT_STATE m_JoykeyState;						//�W���C�p�b�h�̃v���X���
	XINPUT_STATE m_JoykeyStateTrigger;				//�W���C�p�b�h�̃g���K�[���
	int m_aJoykeyStateTrigger;						//�W���C�p�b�h�̃g���K�[���
	D3DXVECTOR3 m_JoyStickPos;			//�W���C�X�e�B�b�N�̌X��
};

#endif // !_INPUTJOYPAD_H_
