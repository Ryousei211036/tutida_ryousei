//==============================================
//
//���C�g�̏���[light.cpp]
//Author�F�y�c����
//
//==============================================
#include"light.h"
#include"application.h"

//==============================================
//�R���X�g���N�^
//==============================================
CLight::CLight()
{

}

//==============================================
//�f�X�g���N�^
//==============================================
CLight::~CLight()
{

}

//==============================================
//���C�g�̏���������
//==============================================
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir[m_nMax_Light];		//���C�g�̕����x�N�g��

	//���C�g���N���A����
	ZeroMemory(&m_light[0],sizeof(D3DLIGHT9));

	for (int nCnt = 0; nCnt < m_nMax_Light; nCnt++)
	{
		//���C�g�̎��
		m_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//���C�g�̕�����ݒ�
		vecDir[0] = D3DXVECTOR3(-10.0f, -8.0f, 1.0f);
		vecDir[1] = D3DXVECTOR3(10.0f, 8.0f, 0.0f);
		vecDir[2] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		/*vecDir[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vecDir[1] = D3DXVECTOR3(0.0f, 2.0f, 0.0f);*/
		
		//���K������(�傫��1�̃x�N�g���ɂ���)
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_light[nCnt].Direction = vecDir[nCnt];

		//���C�g��ݒ�
		pDevice->SetLight(nCnt, &m_light[nCnt]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCnt, TRUE);

		////���C�g�̎��
		//m_light[0].Type = D3DLIGHT_DIRECTIONAL;
		//m_light[1].Type = D3DLIGHT_SPOT;

		////���C�g�̊g�U����ݒ�
		//m_light[0].Diffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 10.0f);
		//m_light[1].Diffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 10.0f);

		////���C�g�̕�����ݒ�
		//vecDir[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//vecDir[1] = D3DXVECTOR3(0.0f, -1.0f, 5.0f);

		////���K������(�傫��1�̃x�N�g���ɂ���)
		//D3DXVec3Normalize(&vecDir[0], &vecDir[0]);
		//m_light[0].Direction = vecDir[0];

		//m_light[1].Position = { 0.0f, 50.0f, -50.0f };

		//m_light[1].Falloff = 100.0f;

		//m_light[1].Specular = { 1.0f,1.0f,1.0f,1.0f };

		//m_light[1].Attenuation0 = 0.0f;
		//m_light[1].Attenuation1 = 1.0f;
		//m_light[1].Attenuation2 = 0.0f;

		//m_light[1].Theta = 100.0f;
		//m_light[1].Phi = 200.0f;

		////���C�g��ݒ�
		//pDevice->SetLight(nCnt, &m_light[nCnt]);

		////���C�g��L���ɂ���
		//pDevice->LightEnable(nCnt, TRUE);
	}
}

//==============================================
//���C�g�̏I������
//==============================================
void CLight::Uninit(void)
{

}

//==============================================
//���C�g�̍X�V����
//==============================================
void CLight::Update(void)
{

}