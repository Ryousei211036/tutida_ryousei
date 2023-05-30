//==============================================================================================
//
// オブジェクトの配置　　　setobject.cpp
// 土田凌聖
//
//==============================================================================================
#include"setobject.h"
#include"inputkeyboard.h"
#include"game.h"
#include"application.h"
#include"objectX.h"
#include"camera.h"
#include"title.h"
#include"tutorial.h"

#include"debugproc.h"

const char *CSetObject::m_sFileName[] =
{
	"Data/XFILE/wall.x",
	"Data/XFILE/robot_wall.x",
	"Data/XFILE/key.x",
	"Data/XFILE/door_length.x",
	"Data/XFILE/door_width.x",
	"Data/XFILE/doorwall.x",
	"Data/XFILE/PC.x",
	"Data/XFILE/table.x",
	"Data/XFILE/button.x",
	"Data/XFILE/shutter.x",
};

CObjectX **CSetObject::pObject = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CSetObject::CSetObject()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CSetObject::~CSetObject()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CSetObject::Init()
{
	m_Pos = { 0.0f,0.0f,0.0f };
	m_Rot = { 0.0f,0.0f,0.0f };
	m_Size = { 0.0f,0.0f,0.0f };
	m_nObjectNum = 0;
	m_nObjectType = 0;
	m_nIndex = 0;
	m_nControl = 0;

	return S_OK;
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CSetObject::Update()
{
	// オブジェクトの設置
	SetObject();

	// オブジェクトの移動
	Move();

	// ファイルに書き込む
	SaveFile();
}

//==============================================================================================
// オブジェクトの設置
//==============================================================================================
void CSetObject::SetObject()
{
	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// オブジェクトの追加
	if (pKeyboard->GetKeyboardTrigger(DIK_P))
	{
		// オブジェクトの生成
		CObjectX *NewObjectX = CObjectX::Create({ 0.0f,0.0f,0.0f }, m_Rot, { 1.0f,1.0f,1.0f }, m_sFileName[m_nObjectNum], m_nObjectNum, (CObjectX::ObjectX_Type)m_nObjectType);

		CObjectX **pObjectX = nullptr;

		// オブジェクトの情報
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pObjectX = CGame::GetObjectX();
		}
		if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{
			pObjectX = CTitle::GetObjectX();
		}
		if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pObjectX = CTutorial::GetObjectX();
		}

		// オブジェクトを加算
		int nNum = CObjectX::GetNumObject();
		CObjectX::SetNumObject(nNum + 1);

		for (int nCnt = 0; nCnt < 128; nCnt++, pObjectX++)
		{
			if ((*pObjectX) == nullptr)
			{
				(*pObjectX) = NewObjectX;
				break;
			}
		}
	}
	if (pKeyboard->GetKeyboardTrigger(DIK_7) && m_nObjectNum < NAME_MAX - 1)
	{
		m_nObjectNum++;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_8) && m_nObjectNum > 0)
	{
		m_nObjectNum--;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_9) && m_nObjectType < CObjectX::OBJECT_MAX - 1)
	{
		m_nObjectType++;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_0) && m_nObjectType > 0)
	{
		m_nObjectType--;
	}

	CDebugProc::Print("\nEnterで保存", m_sFileName[m_nObjectNum]);
	CDebugProc::Print("\nモデルのタイプ : %s\n", m_sFileName[m_nObjectNum]);
	CDebugProc::Print("処理のタイプ : %d", m_nObjectType);
}

//==============================================================================================
// オブジェクトの移動
//==============================================================================================
void CSetObject::Move()
{
	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// オブジェクトの情報
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pObject = CGame::GetObjectX();
	}
	if (CApplication::GetMode() == CApplication::MODE_TITLE)
	{
		pObject = CTitle::GetObjectX();
	}
	if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pObject = CTutorial::GetObjectX();
	}
	int nNum = CObjectX::GetNumObject();

	for (int nCnt = 0; nCnt < nNum; nCnt++, pObject++)
	{
		if (pObject != nullptr)
		{
			(*pObject)->SetAlpha(1.0f);
		}
	}

	// オブジェクトの情報
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pObject = CGame::GetObjectX();
	}
	if (CApplication::GetMode() == CApplication::MODE_TITLE)
	{
		pObject = CTitle::GetObjectX();
	}
	if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pObject = CTutorial::GetObjectX();
	}

	if (pKeyboard->GetKeyboardTrigger(DIK_1) && m_nIndex < nNum - 1)
	{
		m_nIndex++;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_2) && m_nIndex > 0)
	{
		m_nIndex--;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_3))
	{
		m_nIndex = nNum - 1;
	}

	for (int nCnt = 0; nCnt < m_nIndex; nCnt++)
	{
		if (pObject != nullptr)
		{
			pObject++;
		}
	}

	if (pObject != nullptr)
	{
		m_Pos = (*pObject)->GetPos();
		m_Rot = (*pObject)->GetRot();
		m_Size = (*pObject)->GetSize();
		(*pObject)->SetAlpha(0.5f);
	}

	if (pKeyboard->GetKeyboardTrigger(DIK_Z))
	{
		m_nControl++;

		if (m_nControl == 3)
		{
			m_nControl = 0;
		}
	}

	switch (m_nControl)
	{
	case 0:
		// 長押し
		if (pKeyboard->GetKeyboardPress(DIK_T))
		{
			m_Pos.z += 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_G))
		{
			m_Pos.z -= 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_F))
		{
			m_Pos.x -= 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_H))
		{
			m_Pos.x += 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_Y))
		{
			m_Pos.y += 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_R))
		{
			m_Pos.y -= 3;
		}

		// 少し移動
		if (pKeyboard->GetKeyboardTrigger(DIK_I))
		{
			m_Pos.z += 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_K))
		{
			m_Pos.z -= 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_J))
		{
			m_Pos.x -= 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_L))
		{
			m_Pos.x += 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_O))
		{
			m_Pos.y += 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_U))
		{
			m_Pos.y -= 1;
		}
		break;
	case 1:
		if (pKeyboard->GetKeyboardTrigger(DIK_G))
		{
			m_Rot.y += D3DX_PI / 2;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_H))
		{
			m_Rot.y -= D3DX_PI / 2;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_T))
		{
			m_Rot.y += 0.01f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_Y))
		{
			m_Rot.y -= 0.01f;
		}
		break;
	case 2:
		if (pKeyboard->GetKeyboardPress(DIK_T))
		{
			m_Size.x += 0.1f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_Y))
		{
			m_Size.x -= 0.1f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_G))
		{
			m_Size.y += 0.1f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_H))
		{
			m_Size.y -= 0.1f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_B))
		{
			m_Size.z += 0.1f;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_N))
		{
			m_Size.z -= 0.1f;
		}
		break;

	default:
		break;
	}
	CDebugProc::Print("\n操作タイプ%d\n", m_nControl);

	if (pObject != nullptr)
	{
		(*pObject)->SetPos(m_Pos);
		(*pObject)->SetRot(m_Rot);
		(*pObject)->SetSize(m_Size);
	}
}

//==============================================================================================
// ファイルに書き込む
//==============================================================================================
void CSetObject::SaveFile()
{
	CDebugProc::Print("ENTERで保存");

	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN))
	{
		// ファイルポインタ
		FILE *pFile = nullptr;

		// オブジェクトの情報
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pObject = CGame::GetObjectX();

			// ファイル名
			pFile = fopen("Data/TXT/Map/setmap save.txt", "w");
		}
		if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{
			pObject = CTitle::GetObjectX();

			// ファイル名
			pFile = fopen("Data/TXT/Map/settitle save.txt", "w");
		}
		if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pObject = CTutorial::GetObjectX();

			// ファイル名
			pFile = fopen("Data/TXT/Map/settutorial save.txt", "w");
		}

		int nNum = CObjectX::GetNumObject();

		if (pFile != nullptr)
		{
			fprintf(pFile, "SCRIPT\n\n");

			// ファイルネームの読み込み
			for (int nCnt = 0; nCnt < NAME_MAX; nCnt++)
			{
				fprintf(pFile, "FILE_NAME = %s\n", m_sFileName[nCnt]);
			}
			fprintf(pFile, "\n");

			for (int nCnt = 0; nCnt < nNum; nCnt++, pObject++)
			{
				if (pObject != nullptr)
				{
					D3DXVECTOR3 Pos = (*pObject)->GetPos();
					D3DXVECTOR3 Rot = (*pObject)->GetRot();
					D3DXVECTOR3 Size = (*pObject)->GetSize();
					int  ModelType = (*pObject)->GetModelType();
					CObjectX::ObjectX_Type ObjType = (*pObject)->GetObjectXType();

					fprintf(pFile, "MODELSET\n");
					fprintf(pFile, "	POS = %.1f %.1f %.1f\n", Pos.x, Pos.y, Pos.z);
					fprintf(pFile, "	ROT = %.1f %.1f %.1f\n", Rot.x, Rot.y, Rot.z);
					fprintf(pFile, "	SCALE = %.1f %.1f %.1f\n", Size.x, Size.y, Size.z);
					fprintf(pFile, "	MODELTYPE = %d\n", ModelType);
					fprintf(pFile, "	OBJTYPE = %d\n", ObjType);
					fprintf(pFile, "END_MODELSET\n\n");
				}
			}

			fprintf(pFile, "END_SCRIPT");

			//ファイルを閉じる
			fclose(pFile);
		}
	}
}

//==============================================================================================
// オブジェクトの移動の生成
//==============================================================================================
CSetObject *CSetObject::Create()
{
	CSetObject *pSetObject = nullptr;

	pSetObject = new CSetObject;

	if (pSetObject != nullptr)
	{
		pSetObject->Init();
	}

	return pSetObject;
}