//==============================================================================================
//
// �I�u�W�F�N�g�̔z�u�@�@�@setobject.cpp
// �y�c����
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
// �R���X�g���N�^
//==============================================================================================
CSetObject::CSetObject()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CSetObject::~CSetObject()
{
}

//==============================================================================================
// ����������
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
// �X�V����
//==============================================================================================
void CSetObject::Update()
{
	// �I�u�W�F�N�g�̐ݒu
	SetObject();

	// �I�u�W�F�N�g�̈ړ�
	Move();

	// �t�@�C���ɏ�������
	SaveFile();
}

//==============================================================================================
// �I�u�W�F�N�g�̐ݒu
//==============================================================================================
void CSetObject::SetObject()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �I�u�W�F�N�g�̒ǉ�
	if (pKeyboard->GetKeyboardTrigger(DIK_P))
	{
		// �I�u�W�F�N�g�̐���
		CObjectX *NewObjectX = CObjectX::Create({ 0.0f,0.0f,0.0f }, m_Rot, { 1.0f,1.0f,1.0f }, m_sFileName[m_nObjectNum], m_nObjectNum, (CObjectX::ObjectX_Type)m_nObjectType);

		CObjectX **pObjectX = nullptr;

		// �I�u�W�F�N�g�̏��
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

		// �I�u�W�F�N�g�����Z
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

	CDebugProc::Print("\nEnter�ŕۑ�", m_sFileName[m_nObjectNum]);
	CDebugProc::Print("\n���f���̃^�C�v : %s\n", m_sFileName[m_nObjectNum]);
	CDebugProc::Print("�����̃^�C�v : %d", m_nObjectType);
}

//==============================================================================================
// �I�u�W�F�N�g�̈ړ�
//==============================================================================================
void CSetObject::Move()
{
	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// �I�u�W�F�N�g�̏��
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

	// �I�u�W�F�N�g�̏��
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
		// ������
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

		// �����ړ�
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
	CDebugProc::Print("\n����^�C�v%d\n", m_nControl);

	if (pObject != nullptr)
	{
		(*pObject)->SetPos(m_Pos);
		(*pObject)->SetRot(m_Rot);
		(*pObject)->SetSize(m_Size);
	}
}

//==============================================================================================
// �t�@�C���ɏ�������
//==============================================================================================
void CSetObject::SaveFile()
{
	CDebugProc::Print("ENTER�ŕۑ�");

	// �L�[�{�[�h�̏��
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN))
	{
		// �t�@�C���|�C���^
		FILE *pFile = nullptr;

		// �I�u�W�F�N�g�̏��
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pObject = CGame::GetObjectX();

			// �t�@�C����
			pFile = fopen("Data/TXT/Map/setmap save.txt", "w");
		}
		if (CApplication::GetMode() == CApplication::MODE_TITLE)
		{
			pObject = CTitle::GetObjectX();

			// �t�@�C����
			pFile = fopen("Data/TXT/Map/settitle save.txt", "w");
		}
		if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pObject = CTutorial::GetObjectX();

			// �t�@�C����
			pFile = fopen("Data/TXT/Map/settutorial save.txt", "w");
		}

		int nNum = CObjectX::GetNumObject();

		if (pFile != nullptr)
		{
			fprintf(pFile, "SCRIPT\n\n");

			// �t�@�C���l�[���̓ǂݍ���
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

			//�t�@�C�������
			fclose(pFile);
		}
	}
}

//==============================================================================================
// �I�u�W�F�N�g�̈ړ��̐���
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