//==============================================================================================
//
// レンダリング処理　　　renderer.h
// Author : 土田凌聖
//
//==============================================================================================
//#include <d3dx9.h>
//#include<xaudio2.h>		//サウンド処理に必要

#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include <stdio.h>
#include <assert.h>
#include "dinput.h"	//入力処理に必要
#include"xaudio2.h"						//サウンド処理に必要

//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dInput8.lib")		//入力処理に必要

#ifndef _RENDERER_H_
#define	_RENDERER_H_

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();

	int GetFPS();

	static LPD3DXFONT GetFont() { return m_pFont; }
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }

private:
	//const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);	// 頂点フォーマット
	const int SCREEN_WIDTH = 1280;		// スクリーンの幅
	const int SCREEN_HEIGHT = 720;		// スクリーンの高さ
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト
	static LPD3DXFONT m_pFont;			// フォント
	void DrawFPS();						//FPSの描画
};

#endif // !_RENDERER_H_
