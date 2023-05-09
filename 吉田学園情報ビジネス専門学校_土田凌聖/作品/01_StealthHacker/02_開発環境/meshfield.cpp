//==============================================
//
//メッシュフィールドの処理[meshfield.cpp]
//Author：土田凌聖
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
	m_nVertexNum = 0;		// 頂点数
	m_nIndexNum = 0;		// インデックスバッファ
	m_nPrimitiveNum = 0;	// プリミティブ数
	m_nXBlock = 0;			// X方向のブロックの数
	m_nZBlock = 0;			// Z方向のブロックの数
	m_nHeight = 0;			// 頂点の高さ
	m_nVecCount = 0;		// 外積がプラスになった回数
	m_nPolyCount = 0;
	m_fSize = 0.0f;			// サイズ
	m_pVtxBuff = nullptr;	//頂点バッファへのポインタ
	m_pIdxBuff = nullptr;	//インデックスバッファへのポインタ
}

CMeshField::~CMeshField()
{
}

//==============================================
//メッシュフィールドの初期化処理
//==============================================
HRESULT CMeshField::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_nVertexNum = ((m_nXBlock + 1) * (m_nZBlock + 1));										// 頂点数
	m_nIndexNum = (((m_nXBlock + 1) * 2) * (m_nZBlock  *(m_nZBlock - 1)) * m_nZBlock * 2);	// インデックスバッファ
	m_nPrimitiveNum = (m_nXBlock * m_nZBlock * 2 + 4 * (m_nZBlock - 1));					// プリミティブ数
	m_nHeight = 100;		// 頂点の高さ(ランダムの最大値)

	// テクスチャの設定
	m_Texture = CTexture::TEXTURE_TILE;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	VERTEX_3D *pVtx = NULL;			//頂点情報へのポインタ

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
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

	// 頂点数分繰り返す
	//for (int nCntZ = 0; nCntZ <= m_nZBlock; nCntZ++)
	//{
	//	for (int nCntX = 0; nCntX <= m_nXBlock; nCntX++)
	//	{
	//		// 凹凸をつける
	//		int nHeight = rand() % m_nHeight;
	//		pVtx[nCntX + (m_nXBlock + 1) * nCntZ].pos.y = (float)nHeight;
	//	}
	//}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
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

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================
//メッシュフィールドの終了処理
//==============================================
void CMeshField::Uninit(void)
{
	//頂点バッファの解放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//==============================================
//メッシュフィールドの更新処理
//==============================================
void CMeshField::Update(void)
{
	// メッシュの当たり判定
	MeshCollision();

	// 法線の設定
	Normal();
}

//==============================================
//メッシュフィールドの描画処理
//==============================================
void CMeshField::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	// ステンシルバッファの設定
	//Stencil();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットに設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの情報
	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_Texture));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPrimitiveNum);

	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// ステンシルバッファの設定
//==============================================================================================
void CMeshField::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ステンシルバッファ=>有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルバッファと比較する参照値設定=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの値に対してのマスク設定=>0xff(すべて真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定=>
	// 参照値 >= ステンシルバッファの参照値なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(以上)	EQUAL:==(等しい)

	// ステンシルテキストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		// PASS:Zテスト＆ステンシルテスト成功		REPLACE:置き換え
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Zテスト＆ステンシルテスト失敗		KEEP:変更なし
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Zテストのみ失敗					INCR:+1

	// ステンシルバッファ=>無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==============================================
// 生成処理
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
// メッシュの当たり判定
//==============================================
void CMeshField::MeshCollision()
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	D3DXVECTOR3 Vec[3];
	D3DXVECTOR3 PVec = { 0.0f,0.0f,0.0f };

	memset(&Vec, 0, sizeof(Vec));

	for (int nCnt = 0; nCnt < m_nIndexNum - 2; nCnt++)
	{
		// 縮退ポリゴンは判定しない
		if (pVtx[pIdx[nCnt + 1]].pos == pVtx[pIdx[nCnt]].pos) {
			continue;
		}
		else if (pVtx[pIdx[nCnt + 2]].pos == pVtx[pIdx[nCnt + 1]].pos) {
			continue;
		}
		else if (pVtx[pIdx[nCnt]].pos == pVtx[pIdx[nCnt + 2]].pos) {
			continue;
		}

		// 頂点から頂点のベクトル
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

		// 法線面ベクトル
		m_NorVec = { 0.0f,0.0f,0.f };
		D3DXVECTOR3 Vector[2];

		// 計算用ベクトル
		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		D3DXVec3Cross(&m_NorVec, &Vector[0], &Vector[1]);

		// 法線面ベクトルの正規化
		D3DXVec3Normalize(&m_NorVec, &m_NorVec);

		// 操作のタイプ
		CCharManager::Control_Type type = CCharManager::GetControlType();

		CCharManager *pCharacter = nullptr;
		D3DXVECTOR3 PPos = { 0.0f, 0.0f, 0.0f };

		// ゲーム
		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			if (type == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤー
				pCharacter = CGame::GetPlayer();
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ロボット
				pCharacter = CGame::GetRobot();
			}
			if (pCharacter != nullptr)
			{
				PPos = CGame::GetPlayer()->GetPos();	// プレイヤーの位置
			}
		}
		// チュートリアル
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			if (type == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤー
				pCharacter = CTutorial::GetPlayer();
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ロボット
				pCharacter = CTutorial::GetRobot();
			}
			if (pCharacter != nullptr)
			{
				PPos = CTutorial::GetPlayer()->GetPos();	// プレイヤーの位置
			}
		}

		float fCrossProduct = 0;	// 外積の格納先
		m_nVecCount = 0;

		for (int nVCnt = 0; nVCnt < 3; nVCnt++)
		{
			// プレイヤーと3頂点のベクトル
			PVec = PPos - pVtx[pIdx[nCnt + nVCnt]].pos;

			// 外積の計算
			fCrossProduct = Vec[nVCnt].x * PVec.z - Vec[nVCnt].z * PVec.x;

			if (fCrossProduct < 0)
			{// 外積がマイナスの場合
				m_nVecCount++;
			}

			// このポリゴンの中にいる場合
			if (m_nVecCount == 3)
			{
				float fPposY = 0.0f;

				// プレイヤーのY座標の算出
				fPposY = pVtx[pIdx[nCnt]].pos.y - ((PPos.x - pVtx[pIdx[nCnt]].pos.x) * m_NorVec.x + (PPos.z - pVtx[pIdx[nCnt]].pos.z) * m_NorVec.z) / m_NorVec.y;

				// プレイヤーの位置の設定
				//CGame::GetPlayer()->SetPos({ PPos.x, fPposY, PPos.z });
			}
		}
		
		//インデックスバッファのアンロック
		m_pIdxBuff->Unlock();

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//==============================================
// 法線の設定
//==============================================
void CMeshField::Normal()
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < m_nVertexNum; nCnt++)
	{
		// 計算用ベクトル
		D3DXVECTOR3 Vector[2];

		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		// 法線面ベクトル
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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}