//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "renderer.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE_BGM,		// �^�C�g��BGM
	SOUND_LABEL_GAME_BGM,		// �Q�[��BGM
	SOUND_LABEL_RESULT_BGM,		// ���U���gBGM
	SOUND_LABEL_START_SE,		// �X�^�[�g��
	SOUND_LABEL_SHOT_SE,		// �e������
	SOUND_LABEL_HIT_SE,			// �e������������
	SOUND_LABEL_POWERUP_SE,		// ������
	SOUND_LABEL_EVOLUTION_SE,	// �i����
	SOUND_LABEL_DEATH_SE,		// �i����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
