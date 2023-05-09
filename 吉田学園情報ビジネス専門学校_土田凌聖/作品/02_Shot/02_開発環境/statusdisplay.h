//==============================================================================================
//
// �X�e�[�^�X�\���̏����@�@�@statusdisplay.h
// �y�c����
//
//==============================================================================================
#ifndef _STATUS_H_
#define	_STATUS_H_

#include"object2d.h"

class CStatus : public CObject2D
{
public:
	CStatus();
	~CStatus();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CStatus *Create(D3DXVECTOR3 pos, float width, float height);

private:
};

#endif // !_LEVELUP_H_
