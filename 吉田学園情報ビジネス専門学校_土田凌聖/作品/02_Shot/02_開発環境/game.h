//==============================================================================================
//
// ゲーム画面　　　game.h
// 土田凌聖
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
	// 餌生成用のメンバ変数
	D3DXVECTOR3 m_ExpCreatePos;				// 生成する位置
	D3DXVECTOR3 m_ExpSpawnPos[3];			// 生成するおおまかな位置
	int m_nExpCollTime;						// 生成するクールタイム

	// 敵生成用のメンバ変数
	D3DXVECTOR3 m_EnemyCreatePos;			// 生成する位置
	int m_nEnemyCollTime;					// 生成するクールタイム
	int m_nCreateCount;						// 生成した数
	int m_nMaxNum;							// 最大数

	CLevelDisplay *m_ELevel;				// 敵のレベル表示の情報
	D3DXVECTOR3 m_PlusPos;
	D3DXVECTOR3 m_PlusInitPos;
	D3DXVECTOR3 m_PlusZeroPos;
	float m_fPlusPosX, m_fPlusPosY;			// 移動量を足した位置
	float m_fPlusInitPosX, m_fPlusInitPosY;	// 移動量を足した初期位置
	float m_fPlusZeroPosX, m_fPlusZeroPosY;	// 移動量を足した0の位置
	int m_nCreateLevel;						// 生成する敵のレベル
	int m_nLevelCount;						// レベルが上がる間隔
	static CBg *m_pBg;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CExp *m_pExp;
	static CScore *m_pScore;
};

#endif // !_GAME_H_
