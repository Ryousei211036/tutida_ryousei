//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "renderer.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CSound
{
public:
	CSound();
	~CSound();

	enum SOUND_LABEL
	{
		SOUND_LABEL_TITLE = 0,		// タイトル
		SOUND_LABEL_GAME,			// ゲーム
		SOUND_LABEL_RESULT,			// リザルト
		SOUND_LABEL_WARK,			// 歩く音
		SOUND_LABEL_SHOT,			// 銃を打つ音
		SOUND_LABEL_GETKEY,			// 鍵を取る音
		SOUND_LABEL_OPENDOOR,		// ドアを開ける
		SOUND_LABEL_CLOSEDOOR,		// ドアを閉める
		SOUND_LABEL_LOCKDOOR,		// 鍵がしまってるドア
		SOUND_LABEL_FINDENEMY,		// 敵に見つかった
		SOUND_LABEL_HIT,			// 銃弾が当たった
		SOUND_LABEL_START,			// スタート音
		SOUND_LABEL_SELECT,			// タイトル画面の選択音
		SOUND_LABEL_OPEN_SHUTTER,	// 出口が開いた音
		SOUND_LABEL_GOAL,			// ゴール
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
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	};
	static const SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
};

#endif
