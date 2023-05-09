//==============================================================================================
//
// �Q�[�W�@�@�@gage.h
// �y�c����
//
//==============================================================================================
#ifndef _GAGE_H_
#define	_GAGE_H_

#include"object3D.h"

class CGage : public CObject3D
{
public:
	CGage(int nPriority = 5);
	~CGage() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CGage *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, bool billboard);
	bool AddGage(float add);		// �Q�[�W�𑝂₷
	void SubGage(float sub);		// �Q�[�W�����炷
	bool SetAlpha(float alpha);		// �A���t�@�l�̐ݒ�
	void Display(bool display);		// �Q�[�W�̕\��
	void SetObject3D(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool billboard);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetLength(D3DXVECTOR3 size) { m_fLength = size.x; }
	void SetEnd_Length(D3DXVECTOR3 size) { m_fEnd_Length = size.x; }
	void SetResetAlpha(float alpha) { m_fAlpha = alpha; }
	void SetResetCount(int count) { m_nAlphaCount = count; }

	float GetEnd_Lenght() { return m_fEnd_Length; }

private:
	D3DXVECTOR3 m_Pos;		// �ʒu
	int m_nAlphaCount;		// �Q�[�W��������܂ł̎���
	float m_fAlpha;			// �A���t�@�l
	float m_fLength;		// ����(�n)
	float m_fEnd_Length;	// ����(�I)
	bool m_bDisplay;		// �\�����邩
	CObject3D *pObject3D;
};

#endif // !_HACKGAGE_H_
