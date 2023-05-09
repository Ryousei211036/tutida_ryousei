//==============================================
//
//ライトの処理[light.cpp]
//Author：土田凌聖
//
//==============================================
#include"light.h"
#include"application.h"

//==============================================
//コンストラクタ
//==============================================
CLight::CLight()
{

}

//==============================================
//デストラクタ
//==============================================
CLight::~CLight()
{

}

//==============================================
//ライトの初期化処理
//==============================================
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir[m_nMax_Light];		//ライトの方向ベクトル

	//ライトをクリアする
	ZeroMemory(&m_light[0],sizeof(D3DLIGHT9));

	for (int nCnt = 0; nCnt < m_nMax_Light; nCnt++)
	{
		//ライトの種類
		m_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_light[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		vecDir[0] = D3DXVECTOR3(-10.0f, -8.0f, 1.0f);
		vecDir[1] = D3DXVECTOR3(10.0f, 8.0f, 0.0f);
		vecDir[2] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		/*vecDir[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vecDir[1] = D3DXVECTOR3(0.0f, 2.0f, 0.0f);*/
		
		//正規化する(大きさ1のベクトルにする)
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_light[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_light[nCnt]);

		//ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);

		////ライトの種類
		//m_light[0].Type = D3DLIGHT_DIRECTIONAL;
		//m_light[1].Type = D3DLIGHT_SPOT;

		////ライトの拡散光を設定
		//m_light[0].Diffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 10.0f);
		//m_light[1].Diffuse = D3DXCOLOR(10.0f, 10.0f, 10.0f, 10.0f);

		////ライトの方向を設定
		//vecDir[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		//vecDir[1] = D3DXVECTOR3(0.0f, -1.0f, 5.0f);

		////正規化する(大きさ1のベクトルにする)
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

		////ライトを設定
		//pDevice->SetLight(nCnt, &m_light[nCnt]);

		////ライトを有効にする
		//pDevice->LightEnable(nCnt, TRUE);
	}
}

//==============================================
//ライトの終了処理
//==============================================
void CLight::Uninit(void)
{

}

//==============================================
//ライトの更新処理
//==============================================
void CLight::Update(void)
{

}