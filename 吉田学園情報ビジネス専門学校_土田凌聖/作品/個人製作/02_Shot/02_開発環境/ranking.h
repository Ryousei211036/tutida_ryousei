//==============================================================================================
//
// ランキング　　　ranking.h
// 土田凌聖
//
//==============================================================================================
#ifndef _RANKING_H_
#define	_RANKING_H_

#include"score.h"

#define	NUM_RANKING	(5)		// ランキングの数

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
	CScore *m_pScore[NUM_RANKING];		// スコアの情報
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	int m_nScore;						// スコア
	D3DXCOLOR m_Col;					// 色
	int m_aRankingData[NUM_RANKING];	// ランキング
};

#endif // !_RANKING_H_
