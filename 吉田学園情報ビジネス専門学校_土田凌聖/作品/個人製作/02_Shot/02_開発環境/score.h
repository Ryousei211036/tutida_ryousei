//==============================================================================================
//
// �X�R�A�@�@�@score.h
// �y�c����
//
//==============================================================================================
#ifndef _SCORE_H_
#define	_SCORE_H_

#include"Object.h"

#define	SCORE_NUM		(8)			// �X�R�A�̌���
#define MAX_SCORE		(99999999)	// �ō��X�R�A

class CNumber;

class CScore : public CObject
{
public:
	CScore();
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CScore *Create(D3DXVECTOR3 pos, float width, float height, D3DXCOLOR col);
	void AddScore(int score);
	void SaveScore();

	void SetSize(float width, float height);
	void SetScore(int score);
	void SetCol(D3DXCOLOR col);

	int GetScore() { return m_nScore; }
	CNumber *GetNumber() { return *m_pNumber; }

private:
	CNumber *m_pNumber[SCORE_NUM];		// �����̏��
	float m_fWidth;						// ��
	float m_fHeight;					// ����
	int m_nScore;						// �X�R�A
};

#endif // !_SCORE_H_
