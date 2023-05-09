//==============================================================================================
//
// UI�@�@�@UI.h
// �y�c����
//
//==============================================================================================
#ifndef _UI_H_
#define	_UI_H_

#include"object2D.h"

class CUI : public CObject2D
{
public:
	CUI();
	~CUI();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CUI *Create(D3DXVECTOR3 pos, CTexture::TEXTURE texture, float width, float height, int flashingtime, int flashingtime2);
	void Flash(int flashingtime);

	void SetFlashigTime(int flashingtime, int flashingtime2);

private:
	D3DXVECTOR3 m_Pos;						// �ʒu
	float m_fWidth;							// ��
	float m_fHeight;						// ����
	CTexture::TEXTURE *m_pTexture;			// �e�N�X�`��
	int m_nFlashingTime, m_nFlashingTime2;	// �_�ł���Ԋu
	int m_nFlashingCount;					// �_�ł̃J�E���g
	bool m_bEnter;							// �G���^�[�L�[����������
};

#endif // !_GAMEUI_H_
