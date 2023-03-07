//==============================================================================================
//
// リザルト画面　　　result.h
// 土田凌聖
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

#include"object2d.h"

#define RESULT_WIDTH		(1280.0f)		// リザルトの幅
#define RESULT_HEIGHT		(720.0f)		// リザルトの高さ

class CBg;
class CExp;
class CRanking;
class CEnemy;

class CResult : public CObject2D
{
public:
	CResult();
	~CResult() override;

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CResult *Create();
	void CreateExp();

	static void SetScore(int score) { m_nSaveScore = score; }

	static CExp *GetExp() { return m_pExp; }
	static CRanking *GetRanking() { return m_pRanking; }
	static CEnemy *GetEnemy() { return m_pEnemy; }

private:
	D3DXVECTOR3 m_Pos;				// 位置
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	int m_nExpCollTime;				// 生成するクールタイム
	static int m_nSaveScore;		// 保存するスコア
	static CExp *m_pExp;
	static CRanking *m_pRanking;
	static CEnemy *m_pEnemy;
};

#endif // !_RESULT_H_
