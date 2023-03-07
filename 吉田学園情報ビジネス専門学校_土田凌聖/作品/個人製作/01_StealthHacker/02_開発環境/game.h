//==============================================================================================
//
// ゲーム　　　game.h
// 土田凌聖
//
//==============================================================================================
#ifndef _GAME_H_
#define	_GAME_H_

#include"object.h"

class CCamera;
class CFile;
class CCharManager;
class CStencilPolygon;
class CObjectX;
class CSetObject;
class CScore;

class CGame : public CObject
{
public:
	CGame(int nPriority = 0);
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CGame *Create();

	EObjType GetObjType() override { return OBJTYPE_MODE; }		// オブジェクトタイプ
	static CCharManager *GetPlayer() { return m_pPlayer; }		// プレイヤー
	static CCharManager *GetRobot() { return m_pRobot; }		// ロボット
	static CCharManager **GetEnemy() { return m_pEnemy; }		// 敵
	static CObjectX **GetObjectX() { return m_pObjectX; }		// オブジェクトX
	static CCamera *GetCamera() { return m_pCamera; }			// カメラ
	static CFile *GetFile() { return m_pFile; }					// ファイル
	static CStencilPolygon *GetStencil() { return m_pStencil; }	// ステンシルバッファ用ポリゴン
	static CSetObject *GetSetObject() { return m_pSetObject; }	// オブジェクトの設定
	static CScore *GetScore() { return m_pScore; }				// スコア

	static void SetRobot(CCharManager *robot) { m_pRobot = robot; }

private:
	static CCharManager *m_pPlayer;
	static CCharManager *m_pRobot;
	static CCharManager **m_pEnemy;
	static CObjectX **m_pObjectX;
	static CCamera *m_pCamera;
	static CFile *m_pFile;
	static CStencilPolygon *m_pStencil;
	static CSetObject *m_pSetObject;
	static CScore *m_pScore;
};

#endif // !_GAME_H_
