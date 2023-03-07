//==============================================================================================
//
// �I�u�W�F�N�gX�@�@�@objectX.cpp
// �y�c����
//
//==============================================================================================
#include"ObjectX.h"
#include"application.h"
#include"game.h"
#include"player.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"fade.h"
#include"light.h"
#include"sound.h"
#include"gage.h"
#include"enemy.h"
#include"camera.h"
#include"score.h"
#include"tutorial.h"
#include"tutorial_text.h"

#include"debugproc.h"

bool CObjectX::m_bGoalOpen = false;
int CObjectX::m_nNumObject = 0;
int CObjectX::m_nNumHack = 0;
bool CObjectX::m_bGetKey[m_nNumDoor - 1] = {};
bool CObjectX::m_bGetKey_Robot[m_nNumDoor - 1] = {};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_vtxMinModel = { 0.0f,0.0f,0.0f };
	m_vtxMaxModel = { 0.0f,0.0f,0.0f };
	m_vecDir = { 0.0f,0.0f,0.0f };
	m_pBuffMat = nullptr;
	m_pMesh = nullptr;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObjectX::~CObjectX()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CObjectX::Init()
{
	D3DXMatrixIdentity(&m_mtxRot);
	m_vecAxis = { 0.0f,0.0f,0.0f };
	m_fValue = 0.0f;
	m_fDoorSpeed = 0.07f;
	m_Initrot = m_rot;
	m_Alpha = 1.0f;
	m_nNumHack = 0;
	m_nGoalColCount = 150;
	m_bHack = false;
	m_bGageFull_Up = false;
	m_bZeroAlpha = false;

	if (m_nObjectType != OBJECTX_BULLET)
	{
		for (int nCnt = 0; nCnt < m_nNumDoor; nCnt++)
		{
			m_bOpenDoor[nCnt] = false;
		}
		for (int nCnt = 0; nCnt < m_nNumDoor - 1; nCnt++)
		{
			m_bGetKey[nCnt] = false;
			m_bGetKey_Robot[nCnt] = false;
		}

		m_bGoalOpen = false;
	}

	// �I�u�W�F�N�g�̔��a
	m_fRadius = 30.0f;

	// �Q�[�W�̐���
	if (m_nObjectType == OBJECTX_PC)
	{
		m_pGage = CGage::Create({ m_pos.x ,m_pos.y + 50.0f,m_pos.z }, { 35.0f, 3.0f,0.0f }, { 0.0f,1.0f,0.0f,1.0f }, true);
	}

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_FileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�ŏ��l�̎擾
		if (vtx.x < m_vtxMinModel.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		else if (vtx.y < m_vtxMinModel.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		else if (vtx.z < m_vtxMinModel.z)
		{
			m_vtxMinModel.z = vtx.z;
		}

		//�ő�l�̎擾
		if (vtx.x > m_vtxMaxModel.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		else if (vtx.y > m_vtxMaxModel.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		else if (vtx.z > m_vtxMaxModel.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CObjectX::Uninit()
{
	for (int nCnt = 0; nCnt < m_nMax_Model; nCnt++)
	{
		//���b�V���̔j��
		if (m_pMesh != NULL)
		{
			m_pMesh->Release();
			m_pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_pBuffMat != NULL)
		{
			m_pBuffMat->Release();
			m_pBuffMat = NULL;
		}
	}

	Release();
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CObjectX::Update()
{
	CCharManager *pPlayer = CGame::GetPlayer();
	CCharManager *pRobot = CGame::GetRobot();

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayer();
		pRobot = CGame::GetRobot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayer();
		pRobot = CTutorial::GetRobot();
	}

	// �v���C���[�̏��
	D3DXVECTOR3 PPos = { 0.0f, 0.0f, 0.0f };
	float PRadius = 0.0f;
	bool bPPush = false;

	// ���{�b�g�̏��
	D3DXVECTOR3 RPos = { 0.0f, 0.0f, 0.0f };
	float RRadius = 0.0f;
	bool bRPush = false;

	if (pPlayer != nullptr)
	{
		PPos = pPlayer->GetPos();		// �ʒu
		PRadius = pPlayer->GetRadius();	// ���a
		bPPush = pPlayer->GetPushButton();
	}
	if (pRobot != nullptr)
	{
		RPos = pRobot->GetPos();		// �ʒu
		RRadius = pRobot->GetRadius();	// ���a
		bRPush = pRobot->GetPushButton();
	}

	// �I�u�W�F�N�g�ƃv���C���[�̋���
	float fPDistance = CObject::Distance(PPos, m_pos);
	// �I�u�W�F�N�g�ƃ��{�b�g�̋���
	float fRDistance = CObject::Distance(RPos, m_pos);

	// ���{�b�g�������Ƃ��Ă���ꍇ
	if (m_bGetKey_Robot[0] && m_nObjectType == OBJECTX_KEY_RED
		|| m_bGetKey_Robot[1] && m_nObjectType == OBJECTX_KEY_BLUE
		|| m_bGetKey_Robot[2] && m_nObjectType == OBJECTX_KEY_GREEN)
	{
		fPDistance = CObject::Distance(PPos, RPos);
	}

	if (m_nObjectType == OBJECTX_PC && m_pGage != nullptr)
	{
		m_pGage->Display(false);
	}

	if (PRadius + m_fRadius >= fPDistance
		|| (RRadius + m_fRadius >= fRDistance && pRobot != nullptr))
	{
		// �I�u�W�F�N�g�Ƃ̋������߂��ƃA�N�V�������\
		m_bAction = true;

		if (m_nObjectType == OBJECTX_PC && m_pGage != nullptr)
		{
			m_pGage->Display(true);
		}
	}

	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (m_bAction && !Tutorial_Display)
	{// �^�C�v���Ƃ̔���
		CollisionType();
	}

	// �h�A�̓���
	DoorMove();

	m_bAction = false;

	// �Q�[�W�̐ݒ�
	if (m_nObjectType == OBJECTX_PC && m_pGage != nullptr)
	{
		m_pGage->SetPos({ m_pos.x ,m_pos.y + 50.0f,m_pos.z });

		if (m_bGageFull_Up)
		{
			m_bZeroAlpha = m_pGage->SetAlpha(0.1f);
		}
		if (m_bZeroAlpha && !m_bHack)
		{
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CGame::GetScore()->AddScore(500);
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				CTutorial::GetScore()->AddScore(500);
			}

			m_nNumHack++;
			m_bHack = true;
		}
	}

	m_bButton_Player = false;		// �v���C���[���{�^����������
	m_bButton_Robot = false;		// ���{�b�g���{�^����������

	if (bPPush)
	{
		// �v���C���[���{�^����������
		m_bButton_Player = true;
	}
	if (bRPush)
	{
		// ���{�b�g���{�^����������
		m_bButton_Robot = true;
	}

	// �{�^���𓯎��ɉ�������Ԃɂ���ƃS�[�����J��
	if (m_bButton_Player && m_bButton_Robot)
	{
		m_bGoalOpen = true;
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;		//���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;			//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXQUATERNION m_quat;		// �N�H�[�^�j�I��

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < m_nNumDoor; nCnt++)
	{
		// ���{�b�g�������Ƃ����Ƃ�
		if (m_bGetKey_Robot[nCnt])
		{
			D3DXMATRIX MtxRobot;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				MtxRobot = CGame::GetRobot()->GetMatrix();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				MtxRobot = CTutorial::GetRobot()->GetMatrix();
			}

			switch (nCnt)
			{
			case 0:
				if (m_nObjectType == OBJECTX_KEY_RED)
				{
					D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &MtxRobot);
				}
				break;

			case 1:
				if (m_nObjectType == OBJECTX_KEY_BLUE)
				{
					D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &MtxRobot);
				}
				break;

			case 2:
				if (m_nObjectType == OBJECTX_KEY_GREEN)
				{
					D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &MtxRobot);
				}
				break;

			default:
				break;
			}
		}
	}

	// ���ʓ��e
	//DrawShadow();

	// �X�e���V���o�b�t�@�̐ݒ�
	Stencil();

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �X�e���V���p�̕`��
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ���ʂ̕`��
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;
		
		// �h�A
		if (nCntMat == 0
			&&(m_nObjectType == OBJECTX_KEY_RED
			|| m_nObjectType == OBJECTX_KEY_BLUE
			|| m_nObjectType == OBJECTX_KEY_GREEN
			|| m_nObjectType == OBJECTX_DOOR_RED
			|| m_nObjectType == OBJECTX_DOOR_BLUE
			|| m_nObjectType == OBJECTX_DOOR_GREEN))
		{
			// �F�̐ݒ�
			matD3D.Diffuse = m_Col;
			matD3D.Emissive = m_Col;
		}
		if (nCntMat == 0
			&& m_nObjectType == OBJECTX_GOAL && m_bGoalOpen)
		{
			m_nGoalColCount--;

			if (m_nGoalColCount == 0)
			{
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_OPEN_SHUTTER);
			}
			if (m_nGoalColCount < 0)
			{
				// �F�̐ݒ�
				matD3D.Diffuse = m_Col;
				matD3D.Emissive = m_Col;
			}
		}
		
		// �A���t�@�l�̐ݒ�
		matD3D.Diffuse.a = m_Alpha;
		matD3D.Emissive.a = m_Alpha;

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ێ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//==============================================================================================
// ���ʓ��e
//==============================================================================================
void CObjectX::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;			// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;		// ����
	D3DXVECTOR3 pos, normal;

	// �A���r�G���g�𖳌��ɂ���
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���C�g�̎擾
	CLight *pLight = CApplication::GetLight();

	// ���C�g�̕���
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// �ʒu�̐ݒ�
	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// �@���̐ݒ�
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ʂ̍쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ���C�g�ƕ��ʂ���e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

		// �F�̐ݒ�
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		// ���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCnt);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �A���r�G���g��߂�
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CObjectX::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@=>�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ�=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ�=>0xff(���ׂĐ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�=>
	// �Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)

	// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1
}

//==============================================================================================
// ��������
//==============================================================================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, int modeltype, ObjectX_Type type)
{
	CObjectX *pObjectX = nullptr;

	pObjectX = new CObjectX;

	if (pObjectX != nullptr)
	{
		pObjectX->SetXFileName(name);
		pObjectX->SetPos(pos);
		pObjectX->SetRot(rot);
		pObjectX->SetSize(size);
		pObjectX->SetModelType(modeltype);
		pObjectX->SetType(type);
		pObjectX->Init();
		pObjectX->ObjectXCol();
	}

	return pObjectX;
}

//==============================================================================================
// �F�̐ݒ�
//==============================================================================================
void CObjectX::ObjectXCol()
{
	if (m_nObjectType == OBJECTX_KEY_RED || m_nObjectType == OBJECTX_DOOR_RED)
	{
		m_Col = { 0.5f,0.0f,0.0f,1.0f };
	}
	else if (m_nObjectType == OBJECTX_KEY_BLUE || m_nObjectType == OBJECTX_DOOR_BLUE)
	{
		m_Col = { 0.0f,0.0f,0.5f,1.0f };
	}
	else if (m_nObjectType == OBJECTX_KEY_GREEN || m_nObjectType == OBJECTX_DOOR_GREEN)
	{
		m_Col = { 0.0f,0.5f,0.0f,1.0f };
	}
	else if (m_nObjectType == OBJECTX_GOAL)
	{
		m_Col = { 0.0f,1.0f,0.0f,1.0f };
	}
}

//==============================================================================================
// �����蔻��
//==============================================================================================
bool CObjectX::CharCollision(CModel::EModel_Type type)
{
	// �L�����N�^�[�̏��
	CCharManager *pChar = nullptr;		// �v���C���[,���{�b�g���
	CCharManager **pEnemy = nullptr;	// �G���

	D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };		// �ʒu
	D3DXVECTOR3 CharPosOld = { 0.0f,0.0f,0.0f };	// �O��̈ʒu
	D3DXVECTOR3 CharSizeMin = { 0.0f,0.0f,0.0f };	// �T�C�Y�̍ŏ��l
	D3DXVECTOR3 CharSizeMax = { 0.0f,0.0f,0.0f };	// �T�C�Y�̍ő�l

	// �v���C���[�����{�b�g�̏ꍇ
	if (type == CModel::MODEL_TYPE_PLAYER || type == CModel::MODEL_TYPE_ROBOT)
	{
		// �h�A���󂢂Ă�Ƃ��͒ʂ�Ȃ�
		if (OpenDoor()
			&& m_nObjectType != OBJECTX_KEY_RED
			&& m_nObjectType != OBJECTX_KEY_BLUE
			&& m_nObjectType != OBJECTX_KEY_GREEN)
		{
			if (type == CModel::MODEL_TYPE_PLAYER)
			{
				if (CApplication::GetMode() == CApplication::MODE_GAME)
				{
					pChar = CGame::GetPlayer();
				}
				else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
				{
					pChar = CTutorial::GetPlayer();
				}
			}
			else if (type == CModel::MODEL_TYPE_ROBOT
				&& m_nObjectType != OBJECTX_ROBOT_WALL)
			{
				if (CApplication::GetMode() == CApplication::MODE_GAME)
				{
					pChar = CGame::GetRobot();
				}
				else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
				{
					pChar = CTutorial::GetRobot();
				}
			}

			if (pChar != nullptr)
			{
				CharPos = pChar->GetPos();			// �ʒu
				CharPosOld = pChar->GetPosOld();	// �O��̈ʒu
				CharSizeMin = pChar->GetSizeMin();	// ���_�̍ŏ��l
				CharSizeMax = pChar->GetSizeMax();	// ���_�̍ő�l

				if (CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z
					&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z
					&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
					&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
				{
					// ������E
					if (CharPosOld.x + CharSizeMax.x / 2 <= m_pos.x + m_vtxMinModel.x * m_Size.x
						&& CharPos.x + CharSizeMax.x / 2 > m_pos.x + m_vtxMinModel.x * m_Size.x)
					{
						if (type == CModel::MODEL_TYPE_PLAYER)
						{
							pChar->SetPos({ m_pos.x + m_vtxMinModel.x * m_Size.x - CharSizeMax.x / 2  , CharPos.y,CharPos.z});
						}
						else if (type == CModel::MODEL_TYPE_ROBOT)
						{
							pChar->SetPos({ m_pos.x + m_vtxMinModel.x * m_Size.x - CharSizeMax.x / 2 , CharPos.y,CharPos.z });
						}
						return true;
					}
					// �E���獶
					else if (CharPosOld.x + CharSizeMin.x / 2 >= m_pos.x + m_vtxMaxModel.x * m_Size.x
						&& CharPos.x + CharSizeMin.x / 2 < m_pos.x + m_vtxMaxModel.x * m_Size.x)
					{
						pChar->SetPos({ m_pos.x + m_vtxMaxModel.x * m_Size.x - CharSizeMin.x / 2 + 0.2f , CharPos.y,CharPos.z });
						return true;
					}
				}
				if (CharPos.x + CharSizeMax.x / 2 > m_pos.x + m_vtxMinModel.x * m_Size.x
					&& CharPos.x + CharSizeMin.x / 2 < m_pos.x + m_vtxMaxModel.x * m_Size.x
					&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
					&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
				{
					// ��O���牜
					if (CharPosOld.z + CharSizeMax.z / 2 <= m_pos.z + m_vtxMinModel.z * m_Size.z
						&& CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z)
					{
						pChar->SetPos({ CharPos.x , CharPos.y, m_pos.z + m_vtxMinModel.z * m_Size.z - CharSizeMax.z / 2 - 1 });
						return true;
					}
					// �������O
					else if (CharPosOld.z + CharSizeMin.z / 2 >= m_pos.z + m_vtxMaxModel.z * m_Size.z
						&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z)
					{
						if (type == CModel::MODEL_TYPE_PLAYER)
						{
							pChar->SetPos({ CharPos.x, CharPos.y, m_pos.z + m_vtxMaxModel.z * m_Size.z + CharSizeMin.z / 2 + 10 });
						}
						else if (type == CModel::MODEL_TYPE_ROBOT)
						{
							pChar->SetPos({ CharPos.x, CharPos.y, m_pos.z + m_vtxMaxModel.z * m_Size.z + CharSizeMin.z / 2 + 18 });
						}
						return true;
					}
				}
			}
		}
	}

	// �G�̏ꍇ
	if (type == CModel::MODEL_TYPE_ENEMY)
	{
		// �h�A���󂢂Ă�Ƃ��͒ʂ�Ȃ�
		if (OpenDoor())
		{
			pEnemy = CGame::GetEnemy();

			// �G�̐�
			int nNum = CEnemy::GetNumEnemy();

			for (int nCnt = 0; nCnt < nNum; nCnt++, pEnemy++)
			{
				if (*pEnemy != nullptr)
				{
					CharPos = (*pEnemy)->GetPos();			// �ʒu
					CharPosOld = (*pEnemy)->GetPosOld();	// �O��̈ʒu
					CharSizeMin = (*pEnemy)->GetSizeMin();	// ���_�̍ŏ��l
					CharSizeMax = (*pEnemy)->GetSizeMax();	// ���_�̍ő�l

					if (CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z
						&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z
						&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
						&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
					{
						// ������E
						if (CharPosOld.x + CharSizeMax.x / 2 <= m_pos.x + m_vtxMinModel.x * m_Size.x
							&& CharPos.x + CharSizeMax.x / 2 > m_pos.x + m_vtxMinModel.x * m_Size.x)
						{
							(*pEnemy)->SetPos({ m_pos.x + m_vtxMinModel.x * m_Size.x - CharSizeMax.x / 2 , CharPos.y,CharPos.z });
							return true;
						}
						// �E���獶
						else if (CharPosOld.x + CharSizeMin.x / 2 >= m_pos.x + m_vtxMaxModel.x * m_Size.x
							&& CharPos.x + CharSizeMin.x / 2 < m_pos.x + m_vtxMaxModel.x * m_Size.x)
						{
							(*pEnemy)->SetPos({ m_pos.x + m_vtxMaxModel.x * m_Size.x - CharSizeMin.x / 2 + 0.2f , CharPos.y,CharPos.z });
							return true;
						}
					}
					if (CharPos.x + CharSizeMax.x / 2 > m_pos.x + m_vtxMinModel.x * m_Size.x
						&& CharPos.x + CharSizeMin.x / 2 < m_pos.x + m_vtxMaxModel.x * m_Size.x
						&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
						&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
					{
						// ��O���牜
						if (CharPosOld.z + CharSizeMax.z / 2 <= m_pos.z + m_vtxMinModel.z * m_Size.z
							&& CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z)
						{
							(*pEnemy)->SetPos({ CharPos.x , CharPos.y, m_pos.z + m_vtxMinModel.z * m_Size.z - CharSizeMax.z / 2 });
							return true;
						}
						// �������O
						else if (CharPosOld.z + CharSizeMin.z / 2 >= m_pos.z + m_vtxMaxModel.z * m_Size.z
							&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z)
						{
							(*pEnemy)->SetPos({ CharPos.x, CharPos.y, m_pos.z + m_vtxMaxModel.z * m_Size.z + CharSizeMin.z / 2 + 10 });
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

//==============================================================================================
// �I�u�W�F�N�g�̓����蔻��
//==============================================================================================
bool CObjectX::Collision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos = pos;

	// �h�A���󂢂Ă�Ƃ��͒ʂ�Ȃ�
	if (OpenDoor())
	{
		// ��Q���ɓ���������
		if (m_pos.x + m_vtxMaxModel.x * m_Size.x > Pos.x
			&& m_pos.x + m_vtxMinModel.x * m_Size.x < Pos.x
			&& m_pos.z + m_vtxMaxModel.z * m_Size.z > Pos.z
			&& m_pos.z + m_vtxMinModel.z * m_Size.z < Pos.z)
		{
			//m_Alpha = 0.1f;
			return true;
		}
	}

	return false;
}

//==============================================================================================
// �^�C�v���Ƃ̔���
//==============================================================================================
void CObjectX::CollisionType()
{
	// �W���C�p�b�h�̏��
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	switch (m_nObjectType)
	{
		// ��
	case CObjectX::OBJECTX_WALL:
		break;

		// �G���ǂ������Ă����
	case CObjectX::OBJECTX_CHASE_WALL:
		break;

		// ���P
	case CObjectX::OBJECTX_KEY_RED:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
		&& !m_bGetKey[0])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[�����������
				m_bGetKey[0] = true;
				m_bGetKey_Robot[0] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if(!m_bGetKey_Robot[1] && !m_bGetKey_Robot[2])
			{
				// ���{�b�g�����������
				m_bGetKey_Robot[0] = true;

				// ���{�b�g����̑��΍��W
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// ���Q
	case CObjectX::OBJECTX_KEY_BLUE:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
			&& !m_bGetKey[1])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[�����������
				m_bGetKey[1] = true;
				m_bGetKey_Robot[1] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if (!m_bGetKey_Robot[0] && !m_bGetKey_Robot[2])
			{
				// ���{�b�g�����������
				m_bGetKey_Robot[1] = true;

				// ���{�b�g����̑��΍��W
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// ���R
	case CObjectX::OBJECTX_KEY_GREEN:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
			&& !m_bGetKey[2])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[�����������
				m_bGetKey[2] = true;
				m_bGetKey_Robot[2] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if (!m_bGetKey_Robot[0] && !m_bGetKey_Robot[1])
			{
				// ���{�b�g�����������
				m_bGetKey_Robot[2] = true;

				// ���{�b�g����̑��΍��W
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_RED:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			if (m_bGetKey[0])
			{
				if (m_bOpenDoor[0])
				{
					m_bOpenDoor[0] = false;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_CLOSEDOOR);
				}
				else
				{
					m_bOpenDoor[0] = true;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_OPENDOOR);
				}
			}
			else
			{
				// �h�A���J���Ȃ���
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_LOCKDOOR);
				CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_LOCKDOOR, 3.0f);
			}
		}

		/*if (!m_bOpenDoor[0] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[0] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}*/
		break;

		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_BLUE:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			if (m_bGetKey[1])
			{
				if (m_bOpenDoor[1])
				{
					m_bOpenDoor[1] = false;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_CLOSEDOOR);
				}
				else
				{
					m_bOpenDoor[1] = true;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_OPENDOOR);
				}
			}
			else
			{
				// �h�A���J���Ȃ���
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_LOCKDOOR);
				CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_LOCKDOOR, 3.0f);
			}
		}

		/*if (!m_bOpenDoor[1] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[1] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}*/
		break;

		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_GREEN:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			if (m_bGetKey[2])
			{
				if (m_bOpenDoor[2])
				{
					m_bOpenDoor[2] = false;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_CLOSEDOOR);
				}
				else
				{
					m_bOpenDoor[2] = true;

					CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_OPENDOOR);
				}
			}
			else
			{
				// �h�A���J���Ȃ���
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_LOCKDOOR);
				CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_LOCKDOOR, 3.0f);
			}
		}

		/*if (!m_bOpenDoor[2] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[2] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}*/
		break;

		// �h�A(���Ȃ�)
	case CObjectX::OBJECTX_DOOR:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			if (m_bOpenDoor[3])
			{
				m_bOpenDoor[3] = false;

				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_CLOSEDOOR);
			}
			else
			{
				m_bOpenDoor[3] = true;

				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_OPENDOOR);
			}
		}

		/*if (!m_bOpenDoor[3] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[3] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}*/
		break;

		// �h�A�̏�̕�
	case CObjectX::OBJECTX_DOORWALL:
		break;

		// �p�\�R��
	case CObjectX::OBJECTX_PC:
		if ((CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_A))
			&& CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
		{
			m_bGageFull_Up = m_pGage->AddGage(1.1f);
		}
		break;

		// �{�^��
	case CObjectX::OBJECTX_BUTTON:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			// ���݂̈ʒu��ۑ�
			D3DXVECTOR3 Pos = m_pos;

			CCharManager *pPlayer = nullptr;
			CCharManager *pRobot = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pPlayer = CGame::GetPlayer();
				pRobot = CGame::GetRobot();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pPlayer = CTutorial::GetPlayer();
				pRobot = CTutorial::GetRobot();
			}

			// �v���C���[���{�b�g�̏��
			D3DXVECTOR3 PPos = { 0.0f,0.0f,0.0f };
			D3DXVECTOR3 RPos = { 0.0f,0.0f,0.0f };
			float PRadius = 0.0f;
			float RRadius = 0.0f;

			if (pPlayer != nullptr)
			{
				PPos = pPlayer->GetPos();
				PRadius = pPlayer->GetRadius();
			}
			if (pRobot != nullptr)
			{
				RPos = pRobot->GetPos();
				RRadius = pRobot->GetRadius();
			}

			// �v���C���[�̏ꍇ
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// �{�^������̈ʒu����
				Pos.x -= sinf(m_rot.y) * 45.0f;
				Pos.z -= cosf(m_rot.y) * 45.0f;

				Pos.x += sinf(m_rot.y + D3DX_PI / 2) * 5.0f;
				Pos.z += cosf(m_rot.y + D3DX_PI / 2) * 5.0f;

				// �I�u�W�F�N�g�ƃv���C���[�̋���
				float Distance = CObject::Distance(PPos, m_pos);

				// �v���C���[�ƃ��{�b�g�̋���
				float PRDistance = CObject::Distance(PPos, RPos);

				// �{�^���ɋ߂��A���{�b�g���牓���Ƃ�
				if (m_fRadius + PRadius > Distance
					&& PRadius + RRadius < PRDistance)
				{
					pPlayer->SetPos({ Pos.x,PPos.y,Pos.z });
					pPlayer->SetRotDest(m_rot);
					pPlayer->SetPushButton(true); 
				}
			}
			// ���{�b�g�̏ꍇ
			if (CCharManager::GetControlType() == CCharManager::CONTROL_ROBOT)
			{
				// �{�^������̈ʒu����
				Pos.x -= sinf(m_rot.y) * 25.0f;
				Pos.z -= cosf(m_rot.y) * 25.0f;

				Pos.x -= sinf(m_rot.y + D3DX_PI / 2) * 13.0f;
				Pos.z -= cosf(m_rot.y + D3DX_PI / 2) * 13.0f;

				// �I�u�W�F�N�g�ƃ��{�b�g�̋���
				float Distance = CObject::Distance(RPos, m_pos);

				// �v���C���[�ƃ��{�b�g�̋���
				float PRDistance = CObject::Distance(PPos, RPos);

				// �{�^���ɋ߂��A�v���C���[���牓���Ƃ�
				if (m_fRadius + RRadius > Distance
					&& PRadius + RRadius < PRDistance)
				{
					pRobot->SetPos({ Pos.x,RPos.y,Pos.z });
					pRobot->SetRotDest(m_rot);
					pRobot->SetPushButton(true);
				}
			}
		}
		break;

		// �S�[��
	case CObjectX::OBJECTX_GOAL:
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			if (m_bGoalOpen
				&& CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				CFade::SetFade(CApplication::MODE_RESULT, 0.01f);
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GOAL);
			}
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			if (m_bGoalOpen
				&& CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				CFade::SetFade(CApplication::MODE_TITLE, 0.01f);
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GOAL);
			}
		}
		break;

	case OBJECTX_CEILING:
		break;

		// �e
	case CObjectX::OBJECTX_BULLET:
		break;

	default:
		break;
	}
}

void CObjectX::DoorMove()
{
	switch (m_nObjectType)
	{
		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_RED:
		if (!m_bOpenDoor[0] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[0] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}
		break;

		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_BLUE:
		if (!m_bOpenDoor[1] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[1] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}
		break;

		// �h�A(��)
	case CObjectX::OBJECTX_DOOR_GREEN:
		if (!m_bOpenDoor[2] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[2] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}
		break;

		// �h�A(���Ȃ�)
	case CObjectX::OBJECTX_DOOR:
		if (!m_bOpenDoor[3] && m_rot.y <= m_Initrot.y)
		{
			m_rot.y += m_fDoorSpeed;

			if (m_rot.y >= m_Initrot.y)
			{
				m_rot.y = m_Initrot.y;
			}
		}
		else if (m_bOpenDoor[3] && m_rot.y >= m_Initrot.y - D3DX_PI / 2)
		{
			m_rot.y -= m_fDoorSpeed;

			if (m_rot.y <= -D3DX_PI / 2)
			{
				m_rot.y = -D3DX_PI / 2;
			}
		}
		break;

	default:
		break;
	}
}

//==============================================================================================
// �h�A���󂢂Ă��邩
//==============================================================================================
bool CObjectX::OpenDoor()
{
	if((m_nObjectType != OBJECTX_DOOR_RED
		|| (m_nObjectType == OBJECTX_DOOR_RED && !m_bOpenDoor[0]))
		&& (m_nObjectType != OBJECTX_DOOR_BLUE
			|| (m_nObjectType == OBJECTX_DOOR_BLUE && !m_bOpenDoor[1]))
		&& (m_nObjectType != OBJECTX_DOOR_GREEN
			|| (m_nObjectType == OBJECTX_DOOR_GREEN && !m_bOpenDoor[2]))
		&& (m_nObjectType != OBJECTX_DOOR
			|| (m_nObjectType == OBJECTX_DOOR && !m_bOpenDoor[3])))
	{
		// �h�A�����Ă���ꍇ
		return true;
	}

	// �h�A���󂢂Ă���ꍇ
	return false;
}