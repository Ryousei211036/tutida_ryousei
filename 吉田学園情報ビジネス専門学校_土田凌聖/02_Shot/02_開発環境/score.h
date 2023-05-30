//==============================================================================================
//
// スコア　　　score.h
// 土田凌聖
//
//==============================================================================================
#ifndef _SCORE_H_
#define	_SCORE_H_

#include"Object.h"

#define	SCORE_NUM		(8)			// スコアの桁数
#define MAX_SCORE		(99999999)	// 最高スコア

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
	CNumber *m_pNumber[SCORE_NUM];		// 数字の情報
	float m_fWidth;						// 幅
	float m_fHeight;					// 高さ
	int m_nScore;						// スコア
};

#endif // !_SCORE_H_
