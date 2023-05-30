//==============================================================================================
//
// �����L���O�@�@�@ranking.h
// �y�c����
//
//==============================================================================================
#ifndef _RANKING_H_
#define	_RANKING_H_

#include"score.h"

#define	NUM_RANKING	(5)		// �����L���O�̐�

class CRanking : CScore
{
public:
	CRanking();
	~CRanking();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRanking *Create(D3DXVECTOR3 pos, float width, float height, D3DXCOLOR col);
	void LoadScore();
	void SaveScore();
	void CompareScore(int score);

	void SetScore(int score, int cnt);
	void SetSize(float width, float height) { m_fWidth = width; m_fHeight = height; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }

private:
	CScore *m_pScore[NUM_RANKING];		// �X�R�A�̏��
	float m_fWidth;						// ��
	float m_fHeight;					// ����
	int m_nScore;						// �X�R�A
	D3DXCOLOR m_Col;					// �F
	int m_aRankingData[NUM_RANKING];	// �����L���O
};

#endif // !_RANKING_H_
