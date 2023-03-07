//==============================================================================================
//
// �X�e�[�^�X�̃��x���̕\���@�@�@statuslevel.h
// �y�c����
//
//==============================================================================================
#ifndef _STATUSLEVEL_H_
#define	_STATUSLEVEL_H_

#include"score.h"
#include"texture.h"

#define	MAX_STATUSLEVLE		(6)	// �X�e�[�^�X�̍ő僌�x��

class CStatuslevel : public CScore
{
public:
	CStatuslevel();
	~CStatuslevel();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CStatuslevel *Create(D3DXVECTOR3 pos, float width, float height);
	void AddStatuslevel(int statuslevel);

	void SetSize(float width, float height);
	void SetStatuslevel(int statuslevel);
	void SetCol(D3DXCOLOR col);
	
private:
	CNumber *m_pNumber;		// �����̏��
	float m_fWidth;			// ��
	float m_fHeight;		// ����
	int m_nStatuslevel;		// �X�e�[�^�X���x��
	CTexture::TEXTURE m_pTexture;	// �e�N�X�`���ύX�p
};

#endif // !_STATUSLEVEL_H_
