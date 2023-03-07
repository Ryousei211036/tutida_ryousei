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
typedef enum
{
	SOUND_LABEL_TITLE_BGM,		// タイトルBGM
	SOUND_LABEL_GAME_BGM,		// ゲームBGM
	SOUND_LABEL_RESULT_BGM,		// リザルトBGM
	SOUND_LABEL_START_SE,		// スタート音
	SOUND_LABEL_SHOT_SE,		// 弾を撃つ音
	SOUND_LABEL_HIT_SE,			// 弾が当たった音
	SOUND_LABEL_POWERUP_SE,		// 強化音
	SOUND_LABEL_EVOLUTION_SE,	// 進化音
	SOUND_LABEL_DEATH_SE,		// 進化音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
