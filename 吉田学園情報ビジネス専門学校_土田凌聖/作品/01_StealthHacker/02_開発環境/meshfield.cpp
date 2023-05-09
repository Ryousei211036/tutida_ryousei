//==============================================
//
//���b�V���t�B�[���h�̏���[meshfield.cpp]
//Author�F�y�c����
//
//==============================================
#include"meshfield.h"
#include"application.h"
#include"game.h"
#include"player.h"
#include"tutorial.h"
#include<time.h>

CMeshField::CMeshField/*(int nPriority) : CObject3D(nPriority)*/()
{
	m_nVertexNum = 0;		// ���_��
	m_nIndexNum = 0;		// �C���f�b�N�X�o�b�t�@
	m_nPrimitiveNum = 0;	// �v���~�e�B�u��
	m_nXBlock = 0;			// X�����̃u���b�N�̐�
	m_nZBlock = 0;			// Z�����̃u���b�N�̐�
	m_nHeight = 0;			// ���_�̍���
	m_nVecCount = 0;		// �O�ς��v���X�ɂȂ�����
	m_nPolyCount = 0;
	m_fSize = 0.0f;			// �T�C�Y
	m_pVtxBuff = nullptr;	//���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = nullptr;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
}

CMeshField::~CMeshField()
{
}

//==============================================
//���b�V���t�B�[���h�̏���������
//==============================================
HRESULT CMeshField::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_nVertexNum = ((m_nXBlock + 1) * (m_nZBlock + 1));										// ���_��
	m_nIndexNum = (((m_nXBlock + 1) * 2) * (m_nZBlock  *(m_nZBlock - 1)) * m_nZBlock * 2);	// �C���f�b�N�X�o�b�t�@
	m_nPrimitiveNum = (m_nXBlock * m_nZBlock * 2 + 4 * (m_nZBlock - 1));					// �v���~�e�B�u��
	m_nHeight = 100;		// ���_�̍���(�����_���̍ő�l)

	// �e�N�X�`���̐ݒ�
	m_Texture = CTexture::TEXTURE_TILE;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	VERTEX_3D *pVtx = NULL;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntZ = 0; nCntZ <= m_nZBlock; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_nXBlock; nCntX++)
		{
			pVtx[nCntX + (m_nXBlock + 1) * nCntZ].pos = D3DXVECTOR3(-(m_fSize * m_nXBlock) / 2 + nCntX * m_fSize, 0.0f + m_Pos.y, (m_fSize * m_nZBlock) / 2 - nCntZ * m_fSize);
			pVtx[nCntX + (m_nXBlock + 1) * nCntZ].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[nCntX + (m_nXBlock + 1) * nCntZ].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntX + (m_nXBlock + 1) * nCntZ].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
		}
	}

	srand((unsigned int)time(NULL));

	// ���_�����J��Ԃ�
	//for (int nCntZ = 0; nCntZ <= m_nZBlock; nCntZ++)
	//{
	//	for (int nCntX = 0; nCntX <= m_nXBlock; nCntX++)
	//	{
	//		// ���ʂ�����
	//		int nHeight = rand() % m_nHeight;
	//		pVtx[nCntX + (m_nXBlock + 1) * nCntZ].pos.y = (float)nHeight;
	//	}
	//}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (int nCntZ = 0; nCntZ < m_nZBlock; nCntZ++)
	{
		int nLineTop = nCntZ * ((m_nXBlock + 1) * 2 + 2);

		for (int nCntX = 0; nCntX < (m_nXBlock + 1); nCntX++)
		{
			pIdx[(nCntX * 2 + 1) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(nCntX + (m_nXBlock + 1) * nCntZ);

			pIdx[nCntX * 2 + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(pIdx[(nCntX * 2 + 1) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] + (m_nXBlock + 1));
		}

		if (nCntZ < m_nZBlock)
		{
			pIdx[((m_nXBlock + 1) * 2) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(m_nXBlock + ((m_nXBlock + 1))* nCntZ);
			pIdx[((m_nXBlock + 1) * 2) + nCntZ * ((m_nXBlock + 1) * 2 + 2) + 1] = (WORD)(2 * ((m_nXBlock + 1)) + ((m_nXBlock + 1))* nCntZ);
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================
//���b�V���t�B�[���h�̏I������
//==============================================
void CMeshField::Uninit(void)
{
	//���_�o�b�t�@�̉��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//==============================================
//���b�V���t�B�[���h�̍X�V����
//==============================================
void CMeshField::Update(void)
{
	// ���b�V���̓����蔻��
	MeshCollision();

	// �@���̐ݒ�
	Normal();
}

//==============================================
//���b�V���t�B�[���h�̕`�揈��
//==============================================
void CMeshField::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	// �X�e���V���o�b�t�@�̐ݒ�
	//Stencil();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̏��
	CTexture *pTexture = CApplication::GetTexture();

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_Texture));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPrimitiveNum);

	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CMeshField::Stencil()
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
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==============================================
// ��������
//==============================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, int Xblock, int Zblock, float size)
{
	CMeshField *pMeshField = nullptr;

	pMeshField = new CMeshField;

	if (pMeshField != nullptr)
	{
		pMeshField->SetSize(size);
		pMeshField->SetNumBlock(Xblock, Zblock);
		pMeshField->SetPos(pos);
		pMeshField->Init();
	}

	return pMeshField;
}

//==============================================
// ���b�V���̓����蔻��
//==============================================
void CMeshField::MeshCollision()
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	D3DXVECTOR3 Vec[3];
	D3DXVECTOR3 PVec = { 0.0f,0.0f,0.0f };

	memset(&Vec, 0, sizeof(Vec));

	for (int nCnt = 0; nCnt < m_nIndexNum - 2; nCnt++)
	{
		// �k�ރ|���S���͔��肵�Ȃ�
		if (pVtx[pIdx[nCnt + 1]].pos == pVtx[pIdx[nCnt]].pos) {
			continue;
		}
		else if (pVtx[pIdx[nCnt + 2]].pos == pVtx[pIdx[nCnt + 1]].pos) {
			continue;
		}
		else if (pVtx[pIdx[nCnt]].pos == pVtx[pIdx[nCnt + 2]].pos) {
			continue;
		}

		// ���_���璸�_�̃x�N�g��
		if (m_nPolyCount % 2 == 0)
		{
			Vec[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
			Vec[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt + 1]].pos;
			Vec[2] = pVtx[pIdx[nCnt]].pos - pVtx[pIdx[nCnt + 2]].pos;
		}
		else
		{
			Vec[0] = pVtx[pIdx[nCnt]].pos - pVtx[pIdx[nCnt + 1]].pos;
			Vec[1] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt + 2]].pos;
			Vec[2] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;
		}

		m_nPolyCount++;

		// �@���ʃx�N�g��
		m_NorVec = { 0.0f,0.0f,0.f };
		D3DXVECTOR3 Vector[2];

		// �v�Z�p�x�N�g��
		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		D3DXVec3Cross(&m_NorVec, &Vector[0], &Vector[1]);

		// �@���ʃx�N�g���̐��K��
		D3DXVec3Normalize(&m_NorVec, &m_NorVec);

		// ����̃^�C�v
		CCharManager::Control_Type type = CCharManager::GetControlType();

		CCharManager *pCharacter = nullptr;
		D3DXVECTOR3 PPos = { 0.0f, 0.0f, 0.0f };

		// �Q�[��
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			if (type == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[
				pCharacter = CGame::GetPlayer();
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ���{�b�g
				pCharacter = CGame::GetRobot();
			}
			if (pCharacter != nullptr)
			{
				PPos = CGame::GetPlayer()->GetPos();	// �v���C���[�̈ʒu
			}
		}
		// �`���[�g���A��
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			if (type == CCharManager::CONTROL_PLAYER)
			{
				// �v���C���[
				pCharacter = CTutorial::GetPlayer();
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ���{�b�g
				pCharacter = CTutorial::GetRobot();
			}
			if (pCharacter != nullptr)
			{
				PPos = CTutorial::GetPlayer()->GetPos();	// �v���C���[�̈ʒu
			}
		}

		float fCrossProduct = 0;	// �O�ς̊i�[��
		m_nVecCount = 0;

		for (int nVCnt = 0; nVCnt < 3; nVCnt++)
		{
			// �v���C���[��3���_�̃x�N�g��
			PVec = PPos - pVtx[pIdx[nCnt + nVCnt]].pos;

			// �O�ς̌v�Z
			fCrossProduct = Vec[nVCnt].x * PVec.z - Vec[nVCnt].z * PVec.x;

			if (fCrossProduct < 0)
			{// �O�ς��}�C�i�X�̏ꍇ
				m_nVecCount++;
			}

			// ���̃|���S���̒��ɂ���ꍇ
			if (m_nVecCount == 3)
			{
				float fPposY = 0.0f;

				// �v���C���[��Y���W�̎Z�o
				fPposY = pVtx[pIdx[nCnt]].pos.y - ((PPos.x - pVtx[pIdx[nCnt]].pos.x) * m_NorVec.x + (PPos.z - pVtx[pIdx[nCnt]].pos.z) * m_NorVec.z) / m_NorVec.y;

				// �v���C���[�̈ʒu�̐ݒ�
				//CGame::GetPlayer()->SetPos({ PPos.x, fPposY, PPos.z });
			}
		}
		
		//�C���f�b�N�X�o�b�t�@�̃A�����b�N
		m_pIdxBuff->Unlock();

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//==============================================
// �@���̐ݒ�
//==============================================
void CMeshField::Normal()
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < m_nVertexNum; nCnt++)
	{
		// �v�Z�p�x�N�g��
		D3DXVECTOR3 Vector[2];

		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		// �@���ʃx�N�g��
		if ((nCnt % 2) == 0)
		{
			D3DXVec3Cross(&m_NorVec, &Vector[0], &Vector[1]);
		}
		else
		{
			D3DXVec3Cross(&m_NorVec, &Vector[1], &Vector[0]);
		}

		D3DXVec3Normalize(&m_NorVec, &m_NorVec);

		for (int nCntV = 0; nCntV < 3; nCntV++)
		{
			pVtx[pIdx[nCnt]].nor += m_NorVec;
			
			D3DXVec3Normalize(&pVtx[pIdx[nCnt]].nor, &pVtx[pIdx[nCnt]].nor);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}