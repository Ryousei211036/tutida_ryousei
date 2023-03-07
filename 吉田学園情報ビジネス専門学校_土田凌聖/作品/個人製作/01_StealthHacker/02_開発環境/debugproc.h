//==============================================================================================
//
// �f�o�b�O�\���@�@�@debugproc.h
// Author : �y�c����
//
//==============================================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include <d3dx9.h>
#include <string>

class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	void Init();
	void Uninit();

	static void Print(const char *pFormat, ...);
	static void Draw();

private:
	static LPD3DXFONT m_pFont;			// �t�H���g
	static char m_str[1024];
};

#endif // !_DEBUGPROC_H_