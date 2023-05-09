//==============================================================================================
//
// �X�e���V���p�|���S���@�@�@stencilpolygon.cpp
// �y�c����
//
//==============================================================================================
#include"stencilpolygon.h"
#include"application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CStencilPolygon::CStencilPolygon() : CObject2D(5)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CStencilPolygon::~CStencilPolygon()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CStencilPolygon::Init()
{
	CObject2D::Init();

	m_fAlpha = 0.0f;
	m_bUse = false;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CStencilPolygon::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CStencilPolygon::Update()
{
	CObject2D::Update();

	if (m_bUse)
	{
		m_nAlphaSpeed++;
		m_fAlpha = sinf(m_nAlphaSpeed * 0.05f);

		SetCol({ 1.0f,1.0f,1.0f,m_fAlpha });
	}
	else
	{
		m_nAlphaSpeed = 0;
		SetCol({ 1.0f,1.0f,1.0f,0.0f });
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CStencilPolygon::Draw()
{
	Stencil();

	CObject2D::Draw();
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CStencilPolygon::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@=>�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ�=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ�=>0xff(���ׂĐ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�=>
	// �Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)

	// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1
}

//==============================================================================================
// ��������
//==============================================================================================
CStencilPolygon *CStencilPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CStencilPolygon *pStencilPolygon = nullptr;

	pStencilPolygon = new CStencilPolygon;

	if (pStencilPolygon != nullptr)
	{
		pStencilPolygon->SetSize(size.x, size.y);
		pStencilPolygon->Setposition(pos);
		pStencilPolygon->Init();
	}

	return pStencilPolygon;
}