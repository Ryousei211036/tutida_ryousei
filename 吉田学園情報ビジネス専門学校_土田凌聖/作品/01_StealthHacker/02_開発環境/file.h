//==============================================================================================
//
// ファイル　　　file.h
// 土田凌聖
//
//==============================================================================================
#ifndef _FILE_H_
#define	_FILE_H_

#include <d3dx9.h>

class CPlayer;
class CEnemy;
class CCharManager;
class CObjectX;

class CFile
{
public:
	CFile();
	~CFile();

	enum TextName
	{
		TXTNAME_PLAYER = 0,
		TXTNAME_ROBOT,
		TXTNAME_ENEMY,
		TXTNAME_ENEMY_TUTORIAL,
		TXTNAME_MAX,
		TXTNAME_NONE
	};

	CCharManager **LoadCharFile(CFile::TextName index, D3DXVECTOR3 pos = {0.0f,0.0f,0.0f});	// キャラの読み込み
	CObjectX **LoadModel();

private:
	char m_aString[0xff];
	
	CCharManager *m_pPlayer;
	CCharManager *m_pRobot;
	CCharManager *m_pEnemy[16];
	CObjectX *m_pObjectX[128];
	static const char *m_FileName[];	// ファイル名
};

#endif // !_FILE_H_
