//==============================================================================================
//
// �^�C�g����ʁ@�@ title.h
// �y�c����
//
//==============================================================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

#include"object2d.h"

#define TITLE_WIDTH			(910.0f)		// �^�C�g���̕�
#define TITLE_HEIGHT		(510.0f)		// �^�C�g���̍���

class CBg;

class CTitle : public CObject2D
{
public:
	CTitle();
	~CTitle() override;

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	static CTitle *Create();

private:
	D3DXVECTOR3 m_Pos;				// �ʒu
	float m_fWidth;					// ��
	float m_fHeight;				// ����
};

#endif // !_TITLE_H_
