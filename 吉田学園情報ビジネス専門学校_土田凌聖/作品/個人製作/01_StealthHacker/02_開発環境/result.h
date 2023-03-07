//==============================================================================================
//
// ���U���g�@�@�@result.h
// �y�c����
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

#include"object.h"

class CRanking;

class CResult : public CObject
{
public:
	CResult(int nPriority = 0);
	~CResult();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CResult *Create();

	static void SetScore(int score) { m_nSaveScore = score; }

	EObjType GetObjType() override { return OBJTYPE_MODE; }

private:
	static CRanking *m_pRanking;
	static int m_nSaveScore;		// �ۑ�����X�R�A
};

#endif // !_GAME_H_
