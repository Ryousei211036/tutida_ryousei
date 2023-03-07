//==============================================================================================
//
// ���f���@�@�@madel.cpp
// �y�c����
//
//==============================================================================================
#include"model.h"
#include"application.h"
#include"light.h"
#include"game.h"
#include"player.h"

#include"debugproc.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CModel::CModel()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CModel::~CModel()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_XFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_Model[m_nType].m_pBuffMat,
		NULL,
		&m_Model[m_nType].m_nNumMat,
		&m_Model[m_nType].m_pMesh);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = m_Model[m_nType].m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_Model[m_nType].m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_Model[m_nType].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

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
	m_Model[m_nType].m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CModel::Uninit()
{
	for (int nCnt = 0; nCnt < m_nMax_Parts; nCnt++)
	{
		//���b�V���̔j��
		if (m_Model[nCnt].m_pMesh != NULL)
		{
			m_Model[nCnt].m_pMesh->Release();
			m_Model[nCnt].m_pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (m_Model[nCnt].m_pBuffMat != NULL)
		{
			m_Model[nCnt].m_pBuffMat->Release();
			m_Model[nCnt].m_pBuffMat = NULL;
		}
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CModel::Update()
{

}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CModel::Draw(D3DXMATRIX mtxworld)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;				//���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{// �p�[�c���e�̏ꍇ
		m_mtxParent = m_pParent->GetMatrix();
	}
	else
	{// ���݂�Matrix(Player��Matrix)���擾
		pDevice->GetTransform(D3DTS_WORLD, &m_mtxParent);
	}

	// �e�̃}�g���b�N�X�Ƃ�����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ���ʓ��e
	DrawShadow();

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_nType == MODEL_TYPE_ENEMY)
	{
		// �X�e���V���o�b�t�@�̐ݒ�
		Stencil();
	}

	// ���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_Model[m_nType].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_Model[m_nType].m_nNumMat; nCntMat++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;

		// �G�̐F��Ԃ�����
		if (m_nType == MODEL_TYPE_ENEMY)
		{
			// �F�̐ݒ�
			matD3D.Diffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
			matD3D.Emissive = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		}

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		// ���f���p�[�c�̕`��
		m_Model[m_nType].m_pMesh->DrawSubset(nCntMat);
	}

	// �ێ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==============================================================================================
// ���ʓ��e
//==============================================================================================
void CModel::DrawShadow()
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
	pMat = (D3DXMATERIAL*)m_Model[m_nType].m_pBuffMat->GetBufferPointer();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < (int)m_Model[m_nType].m_nNumMat; nCnt++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;
		
		// �F�̐ݒ�
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		// ���f���p�[�c�̕`��
		m_Model[m_nType].m_pMesh->DrawSubset(nCnt);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �A���r�G���g��߂�
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CModel::Stencil()
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
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)   REF >= ���̒l

	// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1
}

//==============================================================================================
// ��������
//==============================================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename, EModel_Type type)
{
	CModel *pModel = nullptr;

	pModel = new CModel;

	if (pModel != nullptr)
	{
		pModel->SetPos(pos);
		pModel->SetRot(rot);
		pModel->SetParent(parent);
		pModel->SetXFileName(Xfilename);
		pModel->SetType(type);
		pModel->Init();
	}

	return pModel;
}