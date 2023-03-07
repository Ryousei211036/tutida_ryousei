//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"object2D.h"
#include"tutorial_text.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject::CObject(int nPriority)
{
	// Top��null�̏ꍇ
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = this;
		m_pTop[nPriority]->m_pNext = nullptr;
		m_pTop[nPriority]->m_pPrev = nullptr;
	}

	// Current��null����Ȃ��ꍇ
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->m_pNext = this;
		m_pCurrent[nPriority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[nPriority]->m_pNext->m_pPrev = m_pCurrent[nPriority];
	}

	m_pCurrent[nPriority] = this;

	// �v���C�I���e�B�[�̐ݒ�
	m_nPriority = nPriority;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObject::~CObject()
{
}

//==============================================================================================
// �S�̂̉������
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
// �S�̂̍X�V����
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

				// ���S�t���O�̊m�F
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

				// ���S�t���O�̊m�F
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
// �S�̂̕`�揈��
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
// ���S�t���O�𗧂Ă�
//==============================================================================================
void CObject::Release()
{
	this->m_bDeath = true;
}

//==============================================================================================
// �I�u�W�F�N�g�̉��
//==============================================================================================
void CObject::Death(int nPriority)
{
	// �I�u�W�F�N�g(�������g)�����X�g����폜
	// Top��������
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
	// Current��������
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

	// �I�u�W�F�N�g(�������g)��j��
	delete this;
}

//==============================================================================================
// ��̈ʒu�̋���
//==============================================================================================
float CObject::Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float Distance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z));

	return Distance;
}