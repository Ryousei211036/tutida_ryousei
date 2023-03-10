//==============================================================================================
//
// オブジェクト　　　object.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"object2D.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CObject *CObject::m_apObject[MAX_PRI][MAX_OBJECT] = {};
int CObject::m_nNumaAll = 0;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObject::CObject(int nPriority)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[nPriority][nCnt] == nullptr)
		{
			m_apObject[nPriority][nCnt] = this;
			m_nID = nCnt;
			m_nNumaAll++;
			m_nPriority = nPriority;

			break;
		}
	}
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CObject::~CObject()
{
}

//==============================================================================================
// 全体の解放処理
//==============================================================================================
void CObject::ReleaseAll()
{
	//メモリの解放
	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPri][nCnt] != nullptr)
			{
				if (m_apObject[nPri][nCnt]->GetObjType() != OBJTYPE_FADE)
				{
					//終了処理
					m_apObject[nPri][nCnt]->Uninit();

					delete[] m_apObject[nPri][nCnt];
					m_apObject[nPri][nCnt] = nullptr;
				}
			}
		}
	}
}

//==============================================================================================
// 全体の更新処理
//==============================================================================================
void CObject::UpdateAll()
{
	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPri][nCnt] != nullptr)
			{
				m_apObject[nPri][nCnt]->Update();
			}
		}
	}
}

//==============================================================================================
// 全体の描画処理
//==============================================================================================
void CObject::DrawAll()
{
	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPri][nCnt] != nullptr)
			{
				m_apObject[nPri][nCnt]->Draw();
			}
		}
	}
}

//==============================================================================================
// ポリゴンの解放
//==============================================================================================
void CObject::Release()
{
	int nID = m_nID;
	int nPriority = m_nPriority;

	if (m_apObject[nPriority][m_nID] != nullptr)
	{
		delete m_apObject[nPriority][nID];
		m_apObject[nPriority][nID] = nullptr;
		m_nNumaAll--;
	}
}

//==============================================================================================
// オブジェクトの情報の取得
//==============================================================================================
CObject *CObject::Getobject(int nPriority, int nNum)
{
	return m_apObject[nPriority][nNum];
}