//==============================================================================================
//
// �Q�[���@�@�@game.h
// �y�c����
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

	EObjType GetObjType() override { return OBJTYPE_MODE; }		// �I�u�W�F�N�g�^�C�v
	static CCharManager *GetPlayer() { return m_pPlayer; }		// �v���C���[
	static CCharManager *GetRobot() { return m_pRobot; }		// ���{�b�g
	static CCharManager **GetEnemy() { return m_pEnemy; }		// �G
	static CObjectX **GetObjectX() { return m_pObjectX; }		// �I�u�W�F�N�gX
	static CCamera *GetCamera() { return m_pCamera; }			// �J����
	static CFile *GetFile() { return m_pFile; }					// �t�@�C��
	static CStencilPolygon *GetStencil() { return m_pStencil; }	// �X�e���V���o�b�t�@�p�|���S��
	static CSetObject *GetSetObject() { return m_pSetObject; }	// �I�u�W�F�N�g�̐ݒ�
	static CScore *GetScore() { return m_pScore; }				// �X�R�A

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
