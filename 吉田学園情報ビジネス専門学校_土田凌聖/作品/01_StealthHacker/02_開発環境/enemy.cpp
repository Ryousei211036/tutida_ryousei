//==============================================================================================
//
// 敵　　　enemy.cpp
// 土田凌聖
//
//==============================================================================================
#include"enemy.h"
#include"model.h"
#include"application.h"
#include"game.h"
#include"objectX.h"
#include"bullet.h"
#include"object3D.h"
#include"score.h"
#include"sound.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"tutorial.h"
#include"tutorial_text.h"
#include"model.h"

#include"debugproc.h"

int CEnemy::m_nNumEnemy = 0;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CEnemy::CEnemy()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CEnemy::~CEnemy()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CEnemy::Init()
{
	CCharManager::Init();

	m_fRadius = 30.0f;			// 半径
	m_fKillArea = 10.0f;		// ステルスキルできる範囲
	m_fKillDistance = 15.0f;	// ステルスキルできる距離
	m_fRotSpeed = 0.1f;			// 回転速度
	m_nLife = 120;				// 体力
	m_fSpeed = 2;				// 移動速度(歩き)
	m_nBulletCoolTime = 50;		// 弾を撃つ間隔
	m_bFind_Player = false;		// プレイヤーを見つけたか
	m_bFind_Robot = false;		// ロボットを見つけたか
	m_bDeath = false;			// 死亡したか
	HitObject = true;
	m_nAlpha_Exclamation = 50;

	m_fView_Distance = 650.0f;
	m_TargetPos = CCharManager::GetPos();

	m_Pos = CCharManager::GetPos();

	pObject3D = CObject3D::Create({ m_Pos.x,m_Pos.y + 120.0f,m_Pos.z }, { 20.0f,20.0f,20.0f }, { 1.0f,1.0f,1.0f,1.0f }, true);
	pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
	pObject3D->SetTexture(CTexture::TEXTURE_EXCLAMATION);

	CObject *pTop = GetTop(3);

	if (pTop != nullptr)
	{
		CObject *pObject = pTop;

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->GetNext();

			if (pObject->GetObjType() == OBJTYPE_ENEMY
				&& !pObject->GetDeathFlag())
			{
				// 体力の設定
				pObject->SetHp(m_nLife);
			}

			pObject = pObjectNext;
		}
	}

	CCharManager::SetRadius(m_fRadius);
	CCharManager::SetLife(m_nLife);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CEnemy::Uninit()
{
	CCharManager::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CEnemy::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCharManager::Update();

		// 体力の取得
		m_nLife = GetHp();

		if (!m_bDeath)
		{
			// 移動処理
			Move();

			// アクション
			Action();

			if (!m_bStealthDeath)
			{
				// 索敵
				Search(OBJTYPE_PLAYER);		// プレイヤー
				Search(OBJTYPE_ROBOT);		// ロボット
				Search(OBJTYPE_OBJECTX);	// オブジェクトX
			}
		}

		pObject3D->SetPos({ m_Pos.x,m_Pos.y + 120.0f,m_Pos.z });

		// 死亡
		Death();

		// モーションの設定
		SetMotion(m_Motion);

		if (!m_bPlay_Shot)
		{
			// ニュートラル状態にする
			m_Motion = CCharManager::MOTION_NEUTRAL;
		}
	}

	CObject::SetPos(m_Pos);
	CObject::SetRadius(m_fRadius);
	CObject::SetHp(m_nLife);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CEnemy::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->CCharManager::SetPos(pos);
		pEnemy->CCharManager::SetPosOld(pos);
		pEnemy->CCharManager::SetRotDest(rot);
	}

	return pEnemy;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CEnemy::Move()
{
	if (!m_bFind_Player && !m_bFind_Robot && !m_bStealthDeath)
	{
		m_PosOld = m_Pos;

		m_Pos = GetPos();
		m_RotDest = GetRotDest();
		m_PosOld = m_Pos;

		// ターゲットの方を向く
		if (m_bLostTarget)
		{
			m_RotDest.y = (float)atan2(m_TargetPos.x - m_Pos.x, m_TargetPos.z - m_Pos.z);
		}

		float Distance = CObject::Distance(m_TargetPos, m_Pos);

		if (m_fRadius < Distance)
		{
			m_Pos.x += sinf(m_RotDest.y) * m_fSpeed;
			m_Pos.z += cosf(m_RotDest.y) * m_fSpeed;
			
			m_Motion = CCharManager::MOTION_WALK;
		}
		else
		{
			m_bLostTarget = false;
		}
	}

	// 角度の正規化
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	// 親クラスに渡す
	CCharManager::SetPos(m_Pos);
	CCharManager::SetPosOld(m_PosOld);
	CCharManager::SetRotDest(m_RotDest);
}

//==============================================================================================
// 敵のアクション
//==============================================================================================
void CEnemy::Action()
{
	// プレイヤーの情報
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

	D3DXVECTOR3 PPos = { 0.0f,0.0f,0.0f };			// 位置
	int nPLife = 0;									// 体力
	float fPRadius = 0.0f;							// 半径
	CCharManager::Motion_Type Motion = MOTION_NONE;	// プレイヤーの状態

	// ロボットの情報
	int nRLife = 0;									// 体力
	D3DXVECTOR3 RPos = { 0.0f,0.0f,0.0f };			// 位置
	float fRRadius = 0.0f;							// 半径

	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// プレイヤーの情報
	if (pPlayer != nullptr)
	{
		PPos = pPlayer->GetPos();			// 位置
		nPLife = pPlayer->GetLife();		// 体力
		fPRadius = pPlayer->GetRadius();	// 半径
		Motion = pPlayer->GetMotion_Type();	// 状態

	}
	// ロボットの情報
	if (pRobot != nullptr)
	{
		RPos = pRobot->GetPos();			// 位置
		nRLife = pRobot->GetLife();			// 体力
		fRRadius = pRobot->GetRadius();		// 半径
	}

	// 現在の位置を保存
	D3DXVECTOR3 pos = m_Pos;

	// ステルスキルできる位置
	pos.x -= sinf(m_Rot.y) * m_fKillDistance;
	pos.z -= cosf(m_Rot.y) * m_fKillDistance;

	float PDistance = 0.0f;
	float RDistance = 0.0f;

	// プレイヤーと敵の距離
	PDistance = CObject::Distance(PPos, pos);
	// ロボットと敵の距離
	RDistance = CObject::Distance(RPos, pos);

	// ステルスキルできる距離にプレイヤーがいる
	if (fPRadius + m_fKillArea > PDistance
		&& !m_bFind_Player && !m_bFind_Robot)
	{
		if ((pKeyboard->GetKeyboardTrigger(DIK_F) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_B)))
		{
			m_bStealthDeath = true;
			pPlayer->SetPlayStealthKill(m_bStealthDeath);
		}
	}

	if (m_bStealthDeath)
	{
		m_bFind_Player = false;
		m_bFind_Robot = false;
		pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });

		// ステルスデスモーション
		m_Motion = CCharManager::MOTION_STEALTHDEATH;

		// モーション変更
		ChangeMotion(m_Motion);

		pPlayer->SetPos(pos);
		pPlayer->SetRotDest(m_Rot);
		pPlayer->ChangeMotion(CCharManager::MOTION_KILL);

		bool bMotion = PlayMotion(CCharManager::MOTION_STEALTHDEATH);

		// モーションが終わったら
		if (!bMotion)
		{
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CGame::GetScore()->AddScore(100);
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				CTutorial::GetScore()->AddScore(100);
			}

			pPlayer->SetPlayStealthKill(false);

			// ステルスキル
			Uninit();
		}
	}

	bool Hit = CObject::GetHit();
	
	if (Hit)
	{
		m_bHit = true;
	}
	if (m_bHit)
	{
		// プレイヤーの方を向く
		m_RotDest.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);

		CCharManager::SetRotDest(m_RotDest);
	}

	// 敵に見つかった場合
	if (!m_bStealthDeath && (m_bFind_Player || m_bFind_Robot))
	{
		if (!m_bPlay_Shot)
		{
			// 銃を構える
			m_Motion = CCharManager::MOTION_GUN;

			if (!FindSound)
			{
				CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_FINDENEMY);
				FindSound = true;
			}
		}

		// プレイヤーが見つかった
		if (m_bFind_Player)
		{
			if (pObject3D != nullptr)
			{
				m_nAlpha_Exclamation--;

				if (m_nAlpha_Exclamation > 0)
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,1.0f });


				}
				else
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
				}
			}

			// 一定距離離れたら攻撃をやめる
			if (fPRadius + m_fView_Distance < PDistance)
			{
				FindSound = false;
				m_bFind_Player = false;
				m_nAlpha_Exclamation = 50;
				pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			}

			// プレイヤーの方を向く
			m_RotDest.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);
		}
		// ロボットが見つかった
		else if (m_bFind_Robot)
		{
			if (pObject3D != nullptr)
			{
				m_nAlpha_Exclamation--;

				if (m_nAlpha_Exclamation > 0)
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,1.0f });
				}
				else
				{
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
				}
			}

			// 一定距離離れたら攻撃をやめる
			if (fRRadius + m_fView_Distance < RDistance)
			{
				m_bHit = false;
				FindSound = false;
				m_bFind_Robot = false;
				m_nAlpha_Exclamation = 50;
				pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			}

			// ロボットの方を向く
			m_RotDest.y = (float)atan2(RPos.x - m_Pos.x, RPos.z - m_Pos.z);
		}

		D3DXVECTOR3 m_GunPos = m_Pos;
		m_GunPos.y = 80.0f;

		m_GunPos.x += sinf(m_Rot.y) * 50;
		m_GunPos.z += cosf(m_Rot.y) * 50;

		m_GunPos.x += sinf(m_Rot.y + D3DX_PI / 2) * 10.0f;
		m_GunPos.z += cosf(m_Rot.y + D3DX_PI / 2) * 10.0f;

		m_nBulletCoolTime--;

		if (m_nBulletCoolTime <= 0)
		{
			m_Motion = CCharManager::MOTION_SHOT;

			m_bPlay_Shot = true;

			// 弾を撃つ
			CBullet::Create(m_GunPos, m_Rot, { 0.2f,0.2f,0.2f }, "Data/XFILE/Player/01_head.x", CObjectX::OBJECTX_BULLET, CModel::MODEL_TYPE_ENEMY);

			m_nBulletCoolTime = 50;
		}

		// モーション変更
		ChangeMotion(m_Motion);

		if (!PlayMotion(MOTION_SHOT))
		{
			m_bPlay_Shot = false;
		}

		// プレイヤー、ロボットの体力が0になったとき
		if (nPLife <= 0)
		{
			m_bFind_Player = false;
			m_Motion = CCharManager::MOTION_NEUTRAL;
		}
		if (pRobot != nullptr)
		{
			if (nRLife <= 0)
			{
				m_bFind_Robot = false;
			}
		}
	}
}

//==============================================================================================
// 体力が0になった
//==============================================================================================
void CEnemy::Death()
{
	// 体力が0になった
	if (m_nLife <= 0)
	{
		pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });

		m_bFind_Player = false;
		m_bFind_Robot = false;
		m_bDeath = true;

		m_Motion = CCharManager::MOTION_DEATH;

		// モーション変更
		ChangeMotion(m_Motion);

		bool bMotion = PlayMotion(CCharManager::MOTION_DEATH);

		// モーションが終わったら
		if (!bMotion)
		{
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				CGame::GetScore()->AddScore(50);
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				CTutorial::GetScore()->AddScore(50);
			}

			// 死亡
			Uninit();
		}
	}
}

//==============================================================================================
// プレイヤーを索敵する
//==============================================================================================
void CEnemy::Search(CObject::EObjType type)
{
	if (!m_bFind_Player && !m_bFind_Robot)
	{
		// 視線の移動
		if (m_EyeRot.y <= m_Rot.y + D3DX_PI * 1 / 4
			&& !m_bEyeType)
		{
			m_EyeRot.y += 0.05f;
		}
		if (m_EyeRot.y >= m_Rot.y + D3DX_PI * 1 / 4)
		{
			m_bEyeType = true;
		}

		if (m_EyeRot.y >= m_Rot.y - D3DX_PI * 1 / 4
			&& m_bEyeType)
		{
			m_EyeRot.y -= 0.05f;
		}
		if (m_EyeRot.y <= m_Rot.y - D3DX_PI * 1 / 4)
		{
			m_bEyeType = false;
		}
	}

	// 視線ベクトル
	D3DXVECTOR3 EyeVec = { 0.0f,0.0f,0.0f };

	EyeVec.x = sinf(m_EyeRot.y);
	EyeVec.z = cosf(m_EyeRot.y);
	EyeVec.y = 0.0f;

	// ベクトルの正規化
	D3DXVec3Normalize(&EyeVec, &EyeVec);

	D3DXVECTOR3 ObjPos = { 0.0f,0.0f,0.0f };	// 位置
	D3DXVECTOR3 ObjSize = { 0.0f,0.0f,0.0f };	// サイズ
	D3DXVECTOR3 ObjMin = { 0.0f,0.0f,0.0f };	// 最小値
	D3DXVECTOR3 ObjMax = { 0.0f,0.0f,0.0f };	// 最大値
	float CharRadius = 0.0f;	// キャラの半径
	int nLife = 1;				// 体力
	int nNumObject = 1;			// オブジェクトの数
	CObjectX::ObjectX_Type Type = CObjectX::OBJECTX_NONE;	// オブジェクトXのタイプ

	// オブジェクトXの情報
	CObjectX **pObjectX = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pObjectX = CGame::GetObjectX();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pObjectX = CTutorial::GetObjectX();
	}

	if ((*pObjectX) != nullptr && type == OBJTYPE_OBJECTX)
	{
		// オブジェクトの数
		nNumObject = (*pObjectX)->GetNumObject();
	}

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		// オブジェクトXの場合
		if (type == OBJTYPE_OBJECTX && (*pObjectX) != nullptr)
		{
			bool bOpenDoor = (*pObjectX)->OpenDoor();
			CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (bOpenDoor && Type != CObjectX::OBJECTX_CEILING)
			{
				ObjPos = (*pObjectX)->GetPos();			// 位置
				ObjSize = (*pObjectX)->GetSize();		// サイズ
				ObjMin = (*pObjectX)->GetMin();			// 頂点の最小値
				ObjMax = (*pObjectX)->GetMax();			// 頂点の最大値
				Type = (*pObjectX)->GetObjectXType();	// タイプ
				OPos = ObjPos;			// 計算用
			}
		}
		else if (type == OBJTYPE_PLAYER)
		{
			// プレイヤーの情報
			CCharManager *pPlayer = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pPlayer = CGame::GetPlayer();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pPlayer = CTutorial::GetPlayer();
			}

			if (pPlayer != nullptr)
			{
				ObjPos = pPlayer->GetPos();			// 位置
				ObjSize = { 1.0f,1.0f,1.0f };		// サイズ
				ObjMin = pPlayer->GetSizeMin();		// 頂点の最小値
				ObjMax = pPlayer->GetSizeMax();		// 頂点の最大値
				CharRadius = pPlayer->GetRadius();	// 半径
				nLife = pPlayer->GetLife();			// 体力
				PPos = ObjPos;		// 計算用
			}
		}
		else if (type == OBJTYPE_ROBOT)
		{
			// ロボットの情報
			CCharManager *pRobot = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pRobot = CGame::GetRobot();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pRobot = CTutorial::GetRobot();
			}

			if (pRobot != nullptr)
			{
				ObjPos = pRobot->GetPos();			// 位置
				ObjSize = { 1.0f,1.0f,1.0f };		// サイズ
				ObjMin = pRobot->GetSizeMin();		// 頂点の最小値
				ObjMax = pRobot->GetSizeMax();		// 頂点の最大値
				CharRadius = pRobot->GetRadius();	// 半径
				nLife = pRobot->GetLife();			// 体力
				RPos = ObjPos;		// 計算用
			}
		}

		// オブジェクトから敵のベクトル
		D3DXVECTOR3 Vec = ObjPos - m_Pos;

		// 正規化
		D3DXVec3Normalize(&Vec, &Vec);

		// 内積
		float fInner = D3DXVec3Dot(&Vec, &EyeVec);

		// 最小値にサイズをかける
		ObjMin.x *= ObjSize.x;
		ObjMin.z *= ObjSize.z;

		// 最大値にサイズをかける
		ObjMax.x *= ObjSize.x;
		ObjMax.z *= ObjSize.z;

		// オブジェクトに最大、最少を足す
		D3DXVECTOR3 Min = ObjPos + ObjMin;
		D3DXVECTOR3 Max = ObjPos + ObjMax;

		// 敵の前にいる
		if (fInner > 0)
		{
			// X座標のニアとファー
			float fNearX = 0.0f;
			float fFarX = 0.0f;

			// Z座標のニアとファー
			float fNearZ = 0.0f;
			float fFarZ = 0.0f;

			// オブジェクトより左にいる
			if (Min.x > m_Pos.x)
			{
				fNearX = (Min.x - m_Pos.x) / EyeVec.x;
				fFarX = (Max.x - m_Pos.x) / EyeVec.x;
			}
			// オブジェクトより右にいる
			else if (Max.x < m_Pos.x)
			{
				fNearX = (Max.x - m_Pos.x) / EyeVec.x;
				fFarX = (Min.x - m_Pos.x) / EyeVec.x;
			}

			// オブジェクトより手前にいる
			if (Min.z > m_Pos.z)
			{
				fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
				fFarZ = (Max.z - m_Pos.z) / EyeVec.z;
			}
			// オブジェクトより奥にいる
			else if (Max.z < m_Pos.z)
			{
				fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
				fFarZ = (Min.z - m_Pos.z) / EyeVec.z;
			}

			float in = 0.0f;	// ベクトルの入り側
			float out = 0.0f;	// ベクトルの出側

			if (Max.x > m_Pos.x && Min.x < m_Pos.x)
			{
				if (EyeVec.x == 0.0f)
				{
					fNearX = EyeVec.x;
					fFarX = 10000;
				}
				else if (EyeVec.x > 0.0f)
				{
					fFarX = (Max.x - m_Pos.x) / EyeVec.x;

					if (Min.z > m_Pos.z)
					{
						fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
					}
					else if (Max.z < m_Pos.z)
					{
						fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
					}
				}
				else if (EyeVec.x < 0.0f)
				{
					fFarX = (Min.x - m_Pos.x) / EyeVec.x;

					if (Min.z > m_Pos.z)
					{
						fNearZ = (Min.z - m_Pos.z) / EyeVec.z;
					}
					else if (Max.z < m_Pos.z)
					{
						fNearZ = (Max.z - m_Pos.z) / EyeVec.z;
					}
				}
			}
			if (Max.z > m_Pos.z && Min.z < m_Pos.z)
			{
				if (EyeVec.z == 0.0f)
				{
					fNearZ = EyeVec.z;
					fFarZ = 10000;
				}
				else if (EyeVec.z > 0.0f)
				{
					fFarZ = (Max.z - m_Pos.z) / EyeVec.z;

					if (Min.x > m_Pos.x)
					{
						fNearX = (Min.x - m_Pos.x) / EyeVec.x;
					}
					else if (Max.x < m_Pos.x)
					{
						fNearX = (Max.x - m_Pos.x) / EyeVec.x;
					}
				}
				else if (EyeVec.z < 0.0f)
				{
					fFarZ = (Min.z - m_Pos.z) / EyeVec.z;

					if (Min.x > m_Pos.x)
					{
						fNearX = (Min.x - m_Pos.x) / EyeVec.x;
					}
					else if (Max.x < m_Pos.x)
					{
						fNearX = (Max.x - m_Pos.x) / EyeVec.x;
					}
				}
			}

			// 遠い方のニアを代入
			if (fNearX < fNearZ)
			{
				in = fNearZ;
			}
			else
			{
				in = fNearX;
			}

			// 近い方のファーを代入
			if (fFarX < fFarZ)
			{
				out = fFarX;
			}
			else
			{
				out = fFarZ;
			}

			float hit = out - in;

			// hitがプラスの場合は当たってる
			if (hit > 0 && nLife > 0)
			{
				float PDistance = CObject::Distance(m_Pos, PPos);
				float RDistance = CObject::Distance(m_Pos, RPos);

				if (type == OBJTYPE_PLAYER
					&& m_fView_Distance > PDistance)
				{
					// プレイヤーを見つけた
					m_bFind_Player = true;
				}
			
				// プレイヤーの前にオブジェクトがある
				if (type == OBJTYPE_OBJECTX && m_bFind_Player
					&& PDistance > in)
				{
					m_Motion = MOTION_NEUTRAL;

					FindSound = false;
					m_bFind_Player = false;

					m_nAlpha_Exclamation = 50;
					pObject3D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
					CObject *pTop = GetTop(3);

					if (pTop != nullptr)
					{
						CObject *pObject = pTop;

						while (pObject != nullptr)
						{
							CObject *pObjectNext = pObject->GetNext();

							if (pObject->GetObjType() == OBJTYPE_ENEMY
								&& !pObject->GetDeathFlag())
							{
								// 体力の設定
								pObject->SetHit(false);
							}

							pObject = pObjectNext;
						}
					}
					m_bHit = false;

					m_EyeRot.y = m_Rot.y;

					break;
				}
				// オブジェクトの前にプレイヤーがいる
				else if (type == OBJTYPE_OBJECTX && m_bFind_Player
					&& PDistance <= in)
				{
					m_bFind_Player = true;

					// プレイヤーの方を向く
					m_EyeRot.y = (float)atan2(PPos.x - m_Pos.x, PPos.z - m_Pos.z);
				}
			}
		}
	}
}

//==============================================================================================
// モーション中の処理
//==============================================================================================
void CEnemy::Process_Motion()
{
}

//==============================================================================================
// モーション値の設定
//==============================================================================================
void CEnemy::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// モーションする
//==============================================================================================
void CEnemy::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// 生成処理
//==============================================================================================
void CEnemy::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel *CEnemy::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
{
	CModel *pModel = CCharManager::SetModel(index, parent, pos, rot, Xfilename, type);

	// サイズの取得
	D3DXVECTOR3 fMax = pModel->GetMaxSize();
	D3DXVECTOR3 fMin = pModel->GetMinSize();

	// 親の位置
	D3DXVECTOR3 ParentPos = { 0.0f,0.0f,0.0f };

	// 親がいる場合
	if (parent >= 0)
	{
		ParentPos = pModel->GetParent().GetPos();
	}

	// オフセットと親の位置を反映
	// 最大値
	fMax.x += pModel->GetPos().x + ParentPos.x;
	fMax.y += pModel->GetPos().y + ParentPos.y;
	fMax.z += pModel->GetPos().z + ParentPos.z;

	// 最小値
	fMin.x += pModel->GetPos().x + ParentPos.x;
	fMin.y += pModel->GetPos().y + ParentPos.y;
	fMin.z += pModel->GetPos().z + ParentPos.z;

	// 最大サイズの設定
	if (m_SizeMax.x < fMax.x)
	{
		m_SizeMax.x = fMax.x;	// X
	}
	if (m_SizeMax.y < fMax.y)
	{
		m_SizeMax.y = fMax.y;	// Y
	}
	if (m_SizeMax.z < fMax.z)
	{
		m_SizeMax.z = fMax.z;	// Z
	}

	// 最小サイズの設定
	if (m_SizeMin.x > fMin.x)
	{
		m_SizeMin.x = fMin.x;	// X
	}
	if (m_SizeMin.y > fMin.y)
	{
		m_SizeMin.y = fMin.y;	// Y
	}
	if (m_SizeMin.z > fMin.z)
	{
		m_SizeMin.z = fMin.z;	// Z
	}

	CCharManager::SetSizeMax(m_SizeMax);
	CCharManager::SetSizeMin(m_SizeMin);

	return nullptr;
}