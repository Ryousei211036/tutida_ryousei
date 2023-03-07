//==============================================================================================
//
// オブジェクトの配置　　　setobject.h
// 土田凌聖
//
//==============================================================================================
#ifndef _SETOBJECT_H_
#define	_SETOBJECT_H_

#include <d3dx9.h>

class CObjectX;

class CSetObject
{
public:
	CSetObject();
	~CSetObject();

	// ファイルネーム
	enum Object_Name
	{
		NAME_WALL = 0,		// 壁
		NAME_ROBOT_WALL,	// ロボットが通れる壁
		NAME_KEY,			// 鍵
		NAME_DOOR_WIDTH,	// ドア(横)
		NAME_DOOR2_LENGTH,	// ドア(縦)
		NAME_DOORWALL,		// ドアの上の壁
		NAME_PC,			// パソコン				
		NAME_TABLE,			// テーブル
		NAME_BUTTON,		// ボタン
		NAME_GOAL,			// ゴール
		NAME_MAX,
		NAME_NONE
	};

	HRESULT Init();
	void Update();
	void SetObject();
	void Move();
	void SaveFile();

	static CSetObject *Create();

	D3DXVECTOR3 GetPos() { return m_Pos; }
	
private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	D3DXVECTOR3 m_Size;
	int m_nIndex;
	int m_nObjectNum;			// オブジェクトの番号
	int m_nObjectType;			// オブジェクトのタイプ
	int m_nNumObject;			// オブジェクトの数
	static const char *m_sFileName[];	// ファイルパス
	static CObjectX **pObject;
	int m_nControl;
};

#endif // !_SETOBJECT_H_
