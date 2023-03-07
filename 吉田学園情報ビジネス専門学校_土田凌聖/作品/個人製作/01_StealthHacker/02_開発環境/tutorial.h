//==============================================================================================
//
// �`���[�g���A���@�@�@tutorial.h
// �y�c����
//
//==============================================================================================
#ifndef _TUTORIAL_H_
#define	_TUTORIAL_H_

class CCamera;
class CCharManager;
class CFile;
class CObjectX;
class CRobot;
class CSetObject;
class CObjectSet;
class CScore;
class CStencilPolygon;
class CTutorial_Text;

#include"object.h"

class CTutorial : public CObject
{
public:
	CTutorial(int nPriority = 0);
	~CTutorial();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CTutorial *Create();

	static CCharManager *GetPlayer() { return m_pPlayer; }		// �v���C���[
	static CCharManager **GetEnemy() { return m_pEnemy; }		// �G
	static CCharManager *GetRobot() { return m_pRobot; }		// ���{�b�g
	static CCamera *GetCamera() { return m_pCamera; }			// �J����
	static CFile *GetFile() { return m_pFile; }					// �t�@�C��
	static CObjectX **GetObjectX() { return m_pObjectX; }		// �I�u�W�F�N�gX
	static CStencilPolygon *GetStencil() { return m_pStencil; }	// �X�e���V���o�b�t�@�p�|���S��
	static CSetObject *GetSetObject() { return m_pSetObject; }	// �I�u�W�F�N�g�̐ݒ�
	static CScore *GetScore() { return m_pScore; }				// �X�R�A
	EObjType GetObjType() override { return OBJTYPE_MODE; }

	static void SetRobot(CCharManager *robot) { m_pRobot = robot; }

private:
	static CCharManager *m_pPlayer;
	static CCharManager **m_pEnemy;
	static CCharManager *m_pRobot;
	static CCamera *m_pCamera;
	static CFile *m_pFile;
	static CObjectX **m_pObjectX;
	static CStencilPolygon *m_pStencil;
	static CSetObject *m_pSetObject;
	static CScore *m_pScore;
	CTutorial_Text *m_PTutorial_Text;
};

#endif // !_TUTORIAL_H_
