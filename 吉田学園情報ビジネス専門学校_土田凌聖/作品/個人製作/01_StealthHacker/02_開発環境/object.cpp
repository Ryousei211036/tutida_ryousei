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
#include"tutorial_text.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObject::CObject(int nPriority)
{
	// Topがnullの場合
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = this;
		m_pTop[nPriority]->m_pNext = nullptr;
		m_pTop[nPriority]->m_pPrev = nullptr;
	}

	// Currentがnullじゃない場合
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->m_pNext = this;
		m_pCurrent[nPriority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[nPriority]->m_pNext->m_pPrev = m_pCurrent[nPriority];
	}

	m_pCurrent[nPriority] = this;

	// プライオリティーの設定
	m_nPriority = nPriority;
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
	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				if (pObject->GetObjType() != OBJTYPE_MODE)
				{
					pObject->Uninit();
				}

				pObject = pObjectNext;
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
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// 死亡フラグの確認
				if (!pObject->m_bDeath)
				{
					pObject->Update();
				}

				pObject = pObjectNext;
			}

			pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// 死亡フラグの確認
				if (pObject->m_bDeath)
				{
					pObject->Death(nPri);
				}

				pObject = pObjectNext;
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
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				if (!pObject->m_bDeath)
				{
					pObject->Draw();
				}

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// 死亡フラグを立てる
//==============================================================================================
void CObject::Release()
{
	this->m_bDeath = true;
}

//==============================================================================================
// オブジェクトの解放
//==============================================================================================
void CObject::Death(int nPriority)
{
	// オブジェクト(自分自身)をリストから削除
	// Topが消えた
	if (this == m_pTop[nPriority])
	{
		m_pTop[nPriority] = m_pTop[nPriority]->m_pNext;

		if (m_pTop[nPriority] != nullptr)
		{
			m_pTop[nPriority]->m_pPrev = nullptr;
		}
		else
		{
			m_pCurrent[nPriority] = nullptr;
		}
	}
	// Currentが消えた
	else if (this == m_pCurrent[nPriority])
	{
		m_pCurrent[nPriority] = m_pCurrent[nPriority]->m_pPrev;
		m_pCurrent[nPriority]->m_pNext = nullptr;
	}
	else
	{
		this->m_pPrev->m_pNext = this->m_pNext;
		this->m_pNext->m_pPrev = this->m_pPrev;
	}

	// オブジェクト(自分自身)を破棄
	delete this;
}

//==============================================================================================
// 二つの位置の距離
//==============================================================================================
float CObject::Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float Distance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z));

	return Distance;
}