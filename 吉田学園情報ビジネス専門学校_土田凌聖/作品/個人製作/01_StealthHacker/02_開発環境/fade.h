//==============================================================================================
//
// �t�F�[�h�@�@�@fade.h
// �y�c����
//
//==============================================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include"object2D.h"
#include"application.h"

class CFade : CObject2D
{
public:
	//�t�F�[�h�̏��
	enum FADE
	{
		FADE_NONE = 0,		//�������ĂȂ����
		FADE_IN,			//�t�F�[�h�C��
		FADE_OUT,			//�t�F�[�h�A�E�g
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
	D3DXVECTOR3 m_Pos;					// �ʒu
	float m_fWidth;						// ��
	float m_fHeight;					// ����
	float m_fAlpha;						// �A���t�@
	static bool m_bCreate;				// ����������
	static CFade *m_pFade;
	static float m_fFadeSpeed;			// �t�F�[�h�̑��x
	static FADE FadeMode;				// �t�F�[�h�̏��
	static CApplication::MODE NextMode;	// ���̉��
};

#endif // !_FADE_H_
