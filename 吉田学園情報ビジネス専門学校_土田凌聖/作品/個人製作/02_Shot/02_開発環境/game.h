//==============================================================================================
//
// �Q�[����ʁ@�@�@game.h
// �y�c����
//
//==============================================================================================
#ifndef _GAME_H_
#define	_GAME_H_

#include"object2d.h"

class CBg;
class CPlayer;
class CEnemy;
class CExp;
class CScore;
class CLevelDisplay;

class CGame : public CObject2D
{
public:
	CGame();
	~CGame() override;

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	
	static CGame *Create();

	void MoveStop(D3DXVECTOR3 InitPos, D3DXVECTOR3 *Pos);
	void CreateExp();
	void CreateEnemy();

	static CBg *GetBg() { return m_pBg; }
	static CPlayer *GetPlayer() { return m_pPlayer; }
	static CEnemy *GetEnemy() { return m_pEnemy; }
	static CExp *GetExp() { return m_pExp; }
	static CScore *GetScore() { return m_pScore; }

	//static void SetExp(int Exp);

private:
	// �a�����p�̃����o�ϐ�
	D3DXVECTOR3 m_ExpCreatePos;				// ��������ʒu
	D3DXVECTOR3 m_ExpSpawnPos[3];			// �������邨���܂��Ȉʒu
	int m_nExpCollTime;						// ��������N�[���^�C��

	// �G�����p�̃����o�ϐ�
	D3DXVECTOR3 m_EnemyCreatePos;			// ��������ʒu
	int m_nEnemyCollTime;					// ��������N�[���^�C��
	int m_nCreateCount;						// ����������
	int m_nMaxNum;							// �ő吔

	CLevelDisplay *m_ELevel;				// �G�̃��x���\���̏��
	D3DXVECTOR3 m_PlusPos;
	D3DXVECTOR3 m_PlusInitPos;
	D3DXVECTOR3 m_PlusZeroPos;
	float m_fPlusPosX, m_fPlusPosY;			// �ړ��ʂ𑫂����ʒu
	float m_fPlusInitPosX, m_fPlusInitPosY;	// �ړ��ʂ𑫂��������ʒu
	float m_fPlusZeroPosX, m_fPlusZeroPosY;	// �ړ��ʂ𑫂���0�̈ʒu
	int m_nCreateLevel;						// ��������G�̃��x��
	int m_nLevelCount;						// ���x�����オ��Ԋu
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CExp *m_pExp;
	static CScore *m_pScore;
};

#endif // !_GAME_H_
