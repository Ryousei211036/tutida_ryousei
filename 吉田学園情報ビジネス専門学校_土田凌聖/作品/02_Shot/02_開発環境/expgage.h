//==============================================================================================
//
// �o���l�Q�[�W�@�@�@expgage.h
// �y�c����
//
//==============================================================================================
#ifndef _EXPGAGE_H_
#define	_EXPGAGE_H_

#define	EXPGAGE_WIDTH		(900)		// �o���l�Q�[�W�̕�

#include"object2D.h"

class CExpgage : public CObject2D
{
public:
	CExpgage();
	~CExpgage();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	static CExpgage *Create(D3DXVECTOR3 pos, float width, float width2, float height, D3DXCOLOR col);

	bool AddExpgage(float Exp);
	void ExpGage(LPD3DXFONT pFont);

private:
	float m_fExpWidth;		// �o���l�Q�[�W�̒���
	float m_fTotalExp;		// �o���l�̍��v
	float m_fExtraExp;		// �Q�[�W���͂ݏo����
	int m_LevelUpCount;		// ���x���A�b�v������
};

#endif // !_EXPGAGE_H_
