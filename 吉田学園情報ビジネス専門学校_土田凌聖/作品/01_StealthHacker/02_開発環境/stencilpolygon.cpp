//==============================================================================================
//
// ステンシル用ポリゴン　　　stencilpolygon.cpp
// 土田凌聖
//
//==============================================================================================
#include"stencilpolygon.h"
#include"application.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CStencilPolygon::CStencilPolygon() : CObject2D(5)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CStencilPolygon::~CStencilPolygon()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CStencilPolygon::Init()
{
	CObject2D::Init();

	m_fAlpha = 0.0f;
	m_bUse = false;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CStencilPolygon::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
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
// 描画処理
//==============================================================================================
void CStencilPolygon::Draw()
{
	Stencil();

	CObject2D::Draw();
}

//==============================================================================================
// ステンシルバッファの設定
//==============================================================================================
void CStencilPolygon::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ステンシルバッファ=>有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルバッファと比較する参照値設定=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの値に対してのマスク設定=>0xff(すべて真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定=>
	// 参照値 >= ステンシルバッファの参照値なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(以上)	EQUAL:==(等しい)

	// ステンシルテキストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Zテスト＆ステンシルテスト成功		REPLACE:置き換え
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Zテスト＆ステンシルテスト失敗		KEEP:変更なし
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Zテストのみ失敗					INCR:+1
}

//==============================================================================================
// 生成処理
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