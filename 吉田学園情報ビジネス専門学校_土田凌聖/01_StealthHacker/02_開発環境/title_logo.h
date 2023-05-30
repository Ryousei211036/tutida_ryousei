//==============================================================================================
//
// タイトルロゴ　　　title_logo.h
// 土田凌聖
//
//==============================================================================================
#ifndef _TITLE_LOGO_H_
#define	_TITLE_LOGO_H_

#include"object2D.h"

class CTitle_Logo : public CObject2D
{
public:
	CTitle_Logo(int nPriority = 4);
	~CTitle_Logo();

	enum Select
	{
		SELECT_START = 0,
		SELECT_TUTORIAL,
		SELECT_MAX,
		SELECT_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void TitleSelect();		// 選択

	static CTitle_Logo *Create();

private:
	static const int m_nNumLogo = 2;	// ロゴの数
	int m_nSelect;						// 選択
	CObject2D *m_pObject2D[m_nNumLogo];
};

#endif // !_TITLE_LOGO_H_
