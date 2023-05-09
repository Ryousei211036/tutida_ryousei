//==============================================================================================
//
// �����_�����O�����@�@�@renderer.h
// Author : �y�c����
//
//==============================================================================================
//#include <d3dx9.h>
//#include<xaudio2.h>		//�T�E���h�����ɕK�v

#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include <stdio.h>
#include <assert.h>
#include "dinput.h"	//���͏����ɕK�v
#include"xaudio2.h"						//�T�E���h�����ɕK�v

//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dInput8.lib")		//���͏����ɕK�v

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
	//const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE);	// ���_�t�H�[�}�b�g
	const int SCREEN_WIDTH = 1280;		// �X�N���[���̕�
	const int SCREEN_HEIGHT = 720;		// �X�N���[���̍���
	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Device�I�u�W�F�N�g
	static LPD3DXFONT m_pFont;			// �t�H���g
	void DrawFPS();						//FPS�̕`��
};

#endif // !_RENDERER_H_
