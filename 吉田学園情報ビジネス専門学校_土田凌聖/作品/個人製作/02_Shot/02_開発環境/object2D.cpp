//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object2D.cpp
// �y�c����
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"main.h"
#include"object2D.h"
#include"renderer.h"
#include"application.h"
#include"inputkeyboard.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_fRot = 0.0f;			// �|���S���̊p�x
	m_pVtxBuff = NULL;
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_texture = CTexture::TEXTURE_NONE;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObject2D::~CObject2D()
{
}

//==============================================================================================
// �|���S���̏�����
//==============================================================================================
HRESULT CObject2D::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���擾
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �|���S���̈ʒu
	m_Pos = pos;

	m_texture = CTexture::TEXTURE_NONE;

	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos.x = m_Pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fWidth;
	pVtx[0].pos.y = m_Pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fWidth2;
	pVtx[1].pos.y = m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * -1.0f)) * m_fWidth;
	pVtx[2].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * -1.0f)) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * 1.0f)) * m_fWidth2;
	pVtx[3].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * 1.0f)) * m_fHeight;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// �|���S���̏I��
//==============================================================================================
void CObject2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==============================================================================================
// �|���S���̍X�V
//==============================================================================================
void CObject2D::Update()
{
	// �Ίp���̊p�x���Z�o����
	m_fAngle = (float)atan2(1.0f, 1.0f);

	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�|���S���̍X�V
	pVtx[0].pos.x = m_Pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fWidth;
	pVtx[0].pos.y = m_Pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fWidth2;
	pVtx[1].pos.y = m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * -1.0f)) * m_fWidth;
	pVtx[2].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * -1.0f)) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * 1.0f)) * m_fWidth2;
	pVtx[3].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * 1.0f)) * m_fHeight;
	pVtx[3].pos.z = 0.0f;

	// �F�̍X�V
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �|���S���̕`��
//==============================================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	 //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	CTexture *pTexture = CApplication::GetTexture();

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,		//���_���̐擪�A�h���X
		2);		//�v���~�e�B�u�̐�
}

//==============================================================================================
// �ʒu�̐ݒ�
//==============================================================================================
void CObject2D::Setposition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//==============================================================================================
// �T�C�Y�̐ݒ�
//==============================================================================================
void CObject2D::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fWidth2 = width;
	m_fHeight = height;
}

//==============================================================================================
// �Q�[�W�̃T�C�Y�̐ݒ�
//==============================================================================================
void CObject2D::SetGagelength(float width, float width2, float height)
{
	m_fWidth = width;
	m_fWidth2 = width2;
	m_fHeight = height;
}

//==============================================================================================
// �p�x�̐ݒ�
//==============================================================================================
void CObject2D::Setrot(float rot)
{
	m_fRot = rot;
}

//==============================================================================================
// �e�N�X�`�����W�̐ݒ�
//==============================================================================================
void CObject2D::SetTexPos(float top, float row, float right, float left)
{
	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, row);
	pVtx[3].tex = D3DXVECTOR2(right, row);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �e�N�X�`���̐ݒ�
//==============================================================================================
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//==============================================================================================
// ������ݒ�
//==============================================================================================
bool CObject2D::SetDisappear(float size, float alpha, float sizespeed, float alphaspeed)
{
	float fSize, fSize1, fSize2;
	fSize = m_fWidth;
	fSize1 = m_fWidth2;
	fSize2 = m_fHeight;

	if (fSize + size >= m_fWidth)
	{
		m_fWidth += sizespeed;		// �T�C�Y�̉��Z
		m_fWidth2 += sizespeed;
		m_fHeight += sizespeed;
	}
	if (m_Col.a >= alpha)
	{
		m_Col.a -= alphaspeed;		// ���l�̌��Z
	}
	if (m_Col.a <= 0)
	{
		return true;
	}

	return false;
}

//==============================================================================================
// �F�̐ݒ�
//==============================================================================================
void CObject2D::SetCol(D3DXCOLOR col)
{
	m_Col = col;
}

//==============================================================================================
// �̗̓Q�[�W�̒���
//==============================================================================================
void CObject2D::SetGageSize(float Gagesize)
{
	m_fWidth2 = Gagesize;
}