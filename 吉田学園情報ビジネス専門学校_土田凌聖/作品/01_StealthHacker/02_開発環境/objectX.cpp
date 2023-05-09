//==============================================================================================
//
// オブジェクトX　　　objectX.cpp
// 土田凌聖
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
// コンストラクタ
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
// デストラクタ
//==============================================================================================
CObjectX::~CObjectX()
{
}

//==============================================================================================
// 初期化処理
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

	// オブジェクトの半径
	m_fRadius = 30.0f;

	// ゲージの生成
	if (m_nObjectType == OBJECTX_PC)
	{
		m_pGage = CGage::Create({ m_pos.x ,m_pos.y + 50.0f,m_pos.z }, { 35.0f, 3.0f,0.0f }, { 0.0f,1.0f,0.0f,1.0f }, true);
	}

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_FileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//最小値の取得
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

		//最大値の取得
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

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CObjectX::Uninit()
{
	for (int nCnt = 0; nCnt < m_nMax_Model; nCnt++)
	{
		//メッシュの破棄
		if (m_pMesh != NULL)
		{
			m_pMesh->Release();
			m_pMesh = NULL;
		}

		//マテリアルの破棄
		if (m_pBuffMat != NULL)
		{
			m_pBuffMat->Release();
			m_pBuffMat = NULL;
		}
	}

	Release();
}

//==============================================================================================
//更新処理
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

	// プレイヤーの情報
	D3DXVECTOR3 PPos = { 0.0f, 0.0f, 0.0f };
	float PRadius = 0.0f;
	bool bPPush = false;

	// ロボットの情報
	D3DXVECTOR3 RPos = { 0.0f, 0.0f, 0.0f };
	float RRadius = 0.0f;
	bool bRPush = false;

	if (pPlayer != nullptr)
	{
		PPos = pPlayer->GetPos();		// 位置
		PRadius = pPlayer->GetRadius();	// 半径
		bPPush = pPlayer->GetPushButton();
	}
	if (pRobot != nullptr)
	{
		RPos = pRobot->GetPos();		// 位置
		RRadius = pRobot->GetRadius();	// 半径
		bRPush = pRobot->GetPushButton();
	}

	// オブジェクトとプレイヤーの距離
	float fPDistance = CObject::Distance(PPos, m_pos);
	// オブジェクトとロボットの距離
	float fRDistance = CObject::Distance(RPos, m_pos);

	// ロボットが鍵をとっている場合
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
		// オブジェクトとの距離が近いとアクションが可能
		m_bAction = true;

		if (m_nObjectType == OBJECTX_PC && m_pGage != nullptr)
		{
			m_pGage->Display(true);
		}
	}

	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (m_bAction && !Tutorial_Display)
	{// タイプごとの判定
		CollisionType();
	}

	// ドアの動き
	DoorMove();

	m_bAction = false;

	// ゲージの設定
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

	m_bButton_Player = false;		// プレイヤーがボタンを押した
	m_bButton_Robot = false;		// ロボットがボタンを押した

	if (bPPush)
	{
		// プレイヤーがボタンを押した
		m_bButton_Player = true;
	}
	if (bRPush)
	{
		// ロボットがボタンを押した
		m_bButton_Robot = true;
	}

	// ボタンを同時に押した状態にするとゴールが開く
	if (m_bButton_Player && m_bButton_Robot)
	{
		m_bGoalOpen = true;
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//計算用のマトリックス
	D3DMATERIAL9 matDef;		//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;			//マテリアルデータへのポインタ
	D3DXQUATERNION m_quat;		// クォータニオン

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < m_nNumDoor; nCnt++)
	{
		// ロボットが鍵をとったとき
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

	// 平面投影
	//DrawShadow();

	// ステンシルバッファの設定
	Stencil();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// ステンシル用の描画
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// ステンシルバッファ=>無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 普通の描画
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアル情報の設定
		D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;
		
		// ドア
		if (nCntMat == 0
			&&(m_nObjectType == OBJECTX_KEY_RED
			|| m_nObjectType == OBJECTX_KEY_BLUE
			|| m_nObjectType == OBJECTX_KEY_GREEN
			|| m_nObjectType == OBJECTX_DOOR_RED
			|| m_nObjectType == OBJECTX_DOOR_BLUE
			|| m_nObjectType == OBJECTX_DOOR_GREEN))
		{
			// 色の設定
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
				// 色の設定
				matD3D.Diffuse = m_Col;
				matD3D.Emissive = m_Col;
			}
		}
		
		// アルファ値の設定
		matD3D.Diffuse.a = m_Alpha;
		matD3D.Emissive.a = m_Alpha;

		// マテリアルの設定
		pDevice->SetMaterial(&matD3D);

		//モデルパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================================================
// 平面投影
//==============================================================================================
void CObjectX::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;		// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;			// マテリアルデータへのポインタ
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE planeField;		// 平面
	D3DXVECTOR3 pos, normal;

	// アンビエントを無効にする
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// ライトの取得
	CLight *pLight = CApplication::GetLight();

	// ライトの方向
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// 位置の設定
	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 平面の作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ライトと平面から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < (int)m_nNumMat; nCnt++)
	{
		// マテリアル情報の設定
		D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

		// 色の設定
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&matD3D);

		// モデルパーツの描画
		m_pMesh->DrawSubset(nCnt);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// アンビエントを戻す
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// ステンシルバッファの設定
//==============================================================================================
void CObjectX::Stencil()
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
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	// PASS:Zテスト＆ステンシルテスト成功		REPLACE:置き換え
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Zテスト＆ステンシルテスト失敗		KEEP:変更なし
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Zテストのみ失敗					INCR:+1
}

//==============================================================================================
// 生成処理
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
// 色の設定
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
// 当たり判定
//==============================================================================================
bool CObjectX::CharCollision(CModel::EModel_Type type)
{
	// キャラクターの情報
	CCharManager *pChar = nullptr;		// プレイヤー,ロボット情報
	CCharManager **pEnemy = nullptr;	// 敵情報

	D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };		// 位置
	D3DXVECTOR3 CharPosOld = { 0.0f,0.0f,0.0f };	// 前回の位置
	D3DXVECTOR3 CharSizeMin = { 0.0f,0.0f,0.0f };	// サイズの最小値
	D3DXVECTOR3 CharSizeMax = { 0.0f,0.0f,0.0f };	// サイズの最大値

	// プレイヤーかロボットの場合
	if (type == CModel::MODEL_TYPE_PLAYER || type == CModel::MODEL_TYPE_ROBOT)
	{
		// ドアが空いてるときは通らない
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
				CharPos = pChar->GetPos();			// 位置
				CharPosOld = pChar->GetPosOld();	// 前回の位置
				CharSizeMin = pChar->GetSizeMin();	// 頂点の最小値
				CharSizeMax = pChar->GetSizeMax();	// 頂点の最大値

				if (CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z
					&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z
					&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
					&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
				{
					// 左から右
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
					// 右から左
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
					// 手前から奥
					if (CharPosOld.z + CharSizeMax.z / 2 <= m_pos.z + m_vtxMinModel.z * m_Size.z
						&& CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z)
					{
						pChar->SetPos({ CharPos.x , CharPos.y, m_pos.z + m_vtxMinModel.z * m_Size.z - CharSizeMax.z / 2 - 1 });
						return true;
					}
					// 奥から手前
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

	// 敵の場合
	if (type == CModel::MODEL_TYPE_ENEMY)
	{
		// ドアが空いてるときは通らない
		if (OpenDoor())
		{
			pEnemy = CGame::GetEnemy();

			// 敵の数
			int nNum = CEnemy::GetNumEnemy();

			for (int nCnt = 0; nCnt < nNum; nCnt++, pEnemy++)
			{
				if (*pEnemy != nullptr)
				{
					CharPos = (*pEnemy)->GetPos();			// 位置
					CharPosOld = (*pEnemy)->GetPosOld();	// 前回の位置
					CharSizeMin = (*pEnemy)->GetSizeMin();	// 頂点の最小値
					CharSizeMax = (*pEnemy)->GetSizeMax();	// 頂点の最大値

					if (CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z
						&& CharPos.z + CharSizeMin.z / 2 < m_pos.z + m_vtxMaxModel.z * m_Size.z
						&& CharPos.y + CharSizeMin.y / 2 < m_pos.y + m_vtxMaxModel.y * m_Size.y
						&& CharPos.y + CharSizeMax.y / 2 > m_pos.y + m_vtxMinModel.y * m_Size.y)
					{
						// 左から右
						if (CharPosOld.x + CharSizeMax.x / 2 <= m_pos.x + m_vtxMinModel.x * m_Size.x
							&& CharPos.x + CharSizeMax.x / 2 > m_pos.x + m_vtxMinModel.x * m_Size.x)
						{
							(*pEnemy)->SetPos({ m_pos.x + m_vtxMinModel.x * m_Size.x - CharSizeMax.x / 2 , CharPos.y,CharPos.z });
							return true;
						}
						// 右から左
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
						// 手前から奥
						if (CharPosOld.z + CharSizeMax.z / 2 <= m_pos.z + m_vtxMinModel.z * m_Size.z
							&& CharPos.z + CharSizeMax.z / 2 > m_pos.z + m_vtxMinModel.z * m_Size.z)
						{
							(*pEnemy)->SetPos({ CharPos.x , CharPos.y, m_pos.z + m_vtxMinModel.z * m_Size.z - CharSizeMax.z / 2 });
							return true;
						}
						// 奥から手前
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
// オブジェクトの当たり判定
//==============================================================================================
bool CObjectX::Collision(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos = pos;

	// ドアが空いてるときは通らない
	if (OpenDoor())
	{
		// 障害物に当たったか
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
// タイプごとの判定
//==============================================================================================
void CObjectX::CollisionType()
{
	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	switch (m_nObjectType)
	{
		// 壁
	case CObjectX::OBJECTX_WALL:
		break;

		// 敵が追いかけてくる壁
	case CObjectX::OBJECTX_CHASE_WALL:
		break;

		// 鍵１
	case CObjectX::OBJECTX_KEY_RED:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
		&& !m_bGetKey[0])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤーが鍵を取った
				m_bGetKey[0] = true;
				m_bGetKey_Robot[0] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if(!m_bGetKey_Robot[1] && !m_bGetKey_Robot[2])
			{
				// ロボットが鍵を取った
				m_bGetKey_Robot[0] = true;

				// ロボットからの相対座標
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// 鍵２
	case CObjectX::OBJECTX_KEY_BLUE:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
			&& !m_bGetKey[1])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤーが鍵を取った
				m_bGetKey[1] = true;
				m_bGetKey_Robot[1] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if (!m_bGetKey_Robot[0] && !m_bGetKey_Robot[2])
			{
				// ロボットが鍵を取った
				m_bGetKey_Robot[1] = true;

				// ロボットからの相対座標
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// 鍵３
	case CObjectX::OBJECTX_KEY_GREEN:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A))
			&& !m_bGetKey[2])
		{
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤーが鍵を取った
				m_bGetKey[2] = true;
				m_bGetKey_Robot[2] = false;
				m_Col = { 0.0f,0.0f,0.0f,0.0f };
				m_pos = { 0.0f,-50.0f,0.0f };
			}
			else if (!m_bGetKey_Robot[0] && !m_bGetKey_Robot[1])
			{
				// ロボットが鍵を取った
				m_bGetKey_Robot[2] = true;

				// ロボットからの相対座標
				m_pos = { 0.0f,0.0f,-12.0f };
			}

			CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_GETKEY);
			CApplication::GetSound()->SetVolume(CSound::SOUND_LABEL_GETKEY, 5.0f);
		}
		break;

		// ドア(赤)
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
				// ドアが開かない音
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

		// ドア(青)
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
				// ドアが開かない音
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

		// ドア(緑)
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
				// ドアが開かない音
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

		// ドア(鍵なし)
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

		// ドアの上の壁
	case CObjectX::OBJECTX_DOORWALL:
		break;

		// パソコン
	case CObjectX::OBJECTX_PC:
		if ((CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E) || pJoypad->GetJoypadPress(CInputJoypad::JOYKEY_A))
			&& CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
		{
			m_bGageFull_Up = m_pGage->AddGage(1.1f);
		}
		break;

		// ボタン
	case CObjectX::OBJECTX_BUTTON:
		if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_E) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_A)))
		{
			// 現在の位置を保存
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

			// プレイヤーロボットの情報
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

			// プレイヤーの場合
			if (CCharManager::GetControlType() == CCharManager::CONTROL_PLAYER)
			{
				// ボタンからの位置調整
				Pos.x -= sinf(m_rot.y) * 45.0f;
				Pos.z -= cosf(m_rot.y) * 45.0f;

				Pos.x += sinf(m_rot.y + D3DX_PI / 2) * 5.0f;
				Pos.z += cosf(m_rot.y + D3DX_PI / 2) * 5.0f;

				// オブジェクトとプレイヤーの距離
				float Distance = CObject::Distance(PPos, m_pos);

				// プレイヤーとロボットの距離
				float PRDistance = CObject::Distance(PPos, RPos);

				// ボタンに近く、ロボットから遠いとき
				if (m_fRadius + PRadius > Distance
					&& PRadius + RRadius < PRDistance)
				{
					pPlayer->SetPos({ Pos.x,PPos.y,Pos.z });
					pPlayer->SetRotDest(m_rot);
					pPlayer->SetPushButton(true); 
				}
			}
			// ロボットの場合
			if (CCharManager::GetControlType() == CCharManager::CONTROL_ROBOT)
			{
				// ボタンからの位置調整
				Pos.x -= sinf(m_rot.y) * 25.0f;
				Pos.z -= cosf(m_rot.y) * 25.0f;

				Pos.x -= sinf(m_rot.y + D3DX_PI / 2) * 13.0f;
				Pos.z -= cosf(m_rot.y + D3DX_PI / 2) * 13.0f;

				// オブジェクトとロボットの距離
				float Distance = CObject::Distance(RPos, m_pos);

				// プレイヤーとロボットの距離
				float PRDistance = CObject::Distance(PPos, RPos);

				// ボタンに近く、プレイヤーから遠いとき
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

		// ゴール
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

		// 弾
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
		// ドア(赤)
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

		// ドア(青)
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

		// ドア(緑)
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

		// ドア(鍵なし)
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
// ドアが空いているか
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
		// ドアが閉じている場合
		return true;
	}

	// ドアが空いている場合
	return false;
}