//==============================================================================================
//
// ���U���g��ʁ@�@�@result.h
// �y�c����
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

#include"object2d.h"

#define RESULT_WIDTH		(1280.0f)		// ���U���g�̕�
#define RESULT_HEIGHT		(720.0f)		// ���U���g�̍���

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
	D3DXVECTOR3 m_Pos;				// �ʒu
	float m_fWidth;					// ��
	float m_fHeight;				// ����
	int m_nExpCollTime;				// ��������N�[���^�C��
	static int m_nSaveScore;		// �ۑ�����X�R�A
	static CExp *m_pExp;
	static CRanking *m_pRanking;
	static CEnemy *m_pEnemy;
};

#endif // !_RESULT_H_
