//==============================================
//
//ライトの処理[light.h]
//Author：土田凌聖
//
//==============================================
#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include"renderer.h"

class CLight
{
public:
	CLight();
	~CLight();

	void Init();
	void Uninit();
	void Update();

	D3DLIGHT9 GetLight() { return m_light[0]; }

private:
	static const int m_nMax_Light = 3;		// ライトの最大数
	D3DLIGHT9 m_light[m_nMax_Light];		// ライト情報
};

#endif // !_LIGHT_H_