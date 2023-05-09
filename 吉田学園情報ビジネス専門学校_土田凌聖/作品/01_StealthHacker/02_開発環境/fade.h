//==============================================================================================
//
// フェード　　　fade.h
// 土田凌聖
//
//==============================================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include"object2D.h"
#include"application.h"

class CFade : CObject2D
{
public:
	//フェードの状態
	enum FADE
	{
		FADE_NONE = 0,		//何もしてない状態
		FADE_IN,			//フェードイン
		FADE_OUT,			//フェードアウト
		FADE_MAX
	};

	CFade(int nPriority = 5);
	~CFade();
	 
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CFade *CFade::Create();
	static void SetFade(CApplication::MODE mode, float fadespeed);

	EObjType GetObjType() override { return OBJTYPE_MODE; }
	static FADE GetFade() { return FadeMode; }

private:
	D3DXVECTOR3 m_Pos;					// 位置
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	float m_fAlpha;						// アルファ
	static bool m_bCreate;				// 生成したか
	static CFade *m_pFade;
	static float m_fFadeSpeed;			// フェードの速度
	static FADE FadeMode;				// フェードの状態
	static CApplication::MODE NextMode;	// 次の画面
};

#endif // !_FADE_H_
