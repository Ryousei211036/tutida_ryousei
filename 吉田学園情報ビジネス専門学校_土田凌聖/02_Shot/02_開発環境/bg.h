//==============================================================================================
//
// �w�i�@�@�@bg.h
// �y�c����
//
//==============================================================================================
#ifndef _BG_H_
#define	_BG_H_

#include"object2D.h"

#define BG_WIDTH				(3500.0f)		// �w�i�̕�
#define BG_HEIGHT				(3500.0f)		// �w�i�̍���

class CBg : public CObject2D
{
public:
	CBg();
	~CBg();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CBg *Create();

private:
	D3DXVECTOR3 m_Pos;				// �ʒu
	D3DXVECTOR3 m_InitPos;			// �����ʒu
	D3DXVECTOR3 m_Move;				// �ړ���
	float m_fWidth;					// ��
	float m_fHeight;				// ����
};

#endif // !_BG_H_
