//==============================================================================================
//
// �^�C�g�����S�@�@�@title_logo.h
// �y�c����
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

	void TitleSelect();		// �I��

	static CTitle_Logo *Create();

private:
	static const int m_nNumLogo = 2;	// ���S�̐�
	int m_nSelect;						// �I��
	CObject2D *m_pObject2D[m_nNumLogo];
};

#endif // !_TITLE_LOGO_H_
