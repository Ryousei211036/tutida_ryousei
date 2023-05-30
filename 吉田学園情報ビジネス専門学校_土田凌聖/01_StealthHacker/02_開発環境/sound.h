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


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	enum SOUND_LABEL
	{
		SOUND_LABEL_TITLE = 0,		// �^�C�g��
		SOUND_LABEL_GAME,			// �Q�[��
		SOUND_LABEL_RESULT,			// ���U���g
		SOUND_LABEL_WARK,			// ������
		SOUND_LABEL_SHOT,			// �e��ł�
		SOUND_LABEL_GETKEY,			// ������鉹
		SOUND_LABEL_OPENDOOR,		// �h�A���J����
		SOUND_LABEL_CLOSEDOOR,		// �h�A��߂�
		SOUND_LABEL_LOCKDOOR,		// �������܂��Ă�h�A
		SOUND_LABEL_FINDENEMY,		// �G�Ɍ�������
		SOUND_LABEL_HIT,			// �e�e����������
		SOUND_LABEL_START,			// �X�^�[�g��
		SOUND_LABEL_SELECT,			// �^�C�g����ʂ̑I����
		SOUND_LABEL_OPEN_SHUTTER,	// �o�����J������
		SOUND_LABEL_GOAL,			// �S�[��
		SOUND_LABEL_MAX,
	};

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	void SetVolume(SOUND_LABEL label, float fVolume);
	void SetPitch(SOUND_LABEL label, float pith);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	struct SOUNDPARAM
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};
	static const SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif
