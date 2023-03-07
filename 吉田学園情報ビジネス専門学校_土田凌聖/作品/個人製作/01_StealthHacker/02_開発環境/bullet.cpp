//==============================================================================================
//
// 弾　　　bullet.cpp
// 土田凌聖
//
//==============================================================================================
#include"bullet.h"
#include"player.h"
#include"game.h"
#include"application.h"
#include"enemy.h"
#include"sound.h"
#include"tutorial.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CBullet::CBullet()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CBullet::~CBullet()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CBullet::Init()
{
	CObjectX::Init();

	CApplication::GetSound()->PlaySound(CSound::SOUND_LABEL_SHOT);

	m_nDeathTime = 25;		// 弾の寿命
	m_fSpeed = 20.0f;		// 移動速度
	m_nAddDamage = 30;		// 与えるダメージ
	m_fRadius = 5.0f;		// 半径

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CBullet::Uninit()
{
	CObjectX::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CBullet::Update()
{
	CObjectX::Update();

	// 弾が当たる
	Hit();

	// 寿命
	Death();

	// 弾が飛んでいく角度
	m_Pos.x += sinf(m_Rot.y) * m_fSpeed;
	m_Pos.z += cosf(m_Rot.y) * m_fSpeed;

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

	// オブジェクトの数
	int nNumObject = (*pObjectX)->GetNumObject();

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		if (*pObjectX != nullptr)
		{
			// オブジェクトとの当たり判定
			bool Hit = (*pObjectX)->Collision(m_Pos);
			ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (Hit && Type != OBJECTX_CEILING)
			{
				// 消す
				Uninit();
			}
		}
	}

	// 親クラスに渡す
	CObjectX::SetPos(m_Pos);
	CObjectX::SetRot(m_Rot);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CBullet::Draw()
{
	CObjectX::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, ObjectX_Type type, CModel::EModel_Type modeltype)
{
	CBullet *pBullet = nullptr;

	pBullet = new CBullet;

	if (pBullet != nullptr)
	{
		pBullet->SetModelType(modeltype);
		pBullet->SetPos(pos);
		pBullet->SetRot(rot);
		pBullet->CObjectX::SetSize(size);
		pBullet->CObjectX::SetXFileName(name);
		pBullet->CObjectX::SetType(type);
		pBullet->Init();
	}

	return pBullet;
}

//==============================================================================================
// 弾の寿命
//==============================================================================================
void CBullet::Death()
{
	m_nDeathTime--;

	if (m_nDeathTime <= 0)
	{
		Uninit();
	}
}

//==============================================================================================
// 弾が当たった
//==============================================================================================
void CBullet::Hit()
{
	D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };		// 位置
	float CharRadius = 0.0f;	// 半径
	int CharHp = 0;				// 体力

	// 敵の情報
	CCharManager **pEnemy = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pEnemy = CGame::GetEnemy();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pEnemy = CTutorial::GetEnemy();
	}

	if (m_nType == CModel::MODEL_TYPE_PLAYER)
	{
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
					D3DXVECTOR3 EPos = { 0.0f,0.0f,0.0f };
					float fERadius = 0.0f;
					int nEHp = 0;

					EPos = pObject->GetPos();
					fERadius = pObject->GetRadius();
					nEHp = pObject->GetHp();

					// 敵と弾の距離
					float fDistance = CObject::Distance(EPos, m_Pos);

					if (m_fRadius + fERadius >= fDistance)
					{
						// 体力の設定
						pObject->SetHp(nEHp - m_nAddDamage);
						pObject->SetHit(true);

						Uninit();
					}
				}

				pObject = pObjectNext;
			}
		}
	}
	else if (m_nType == CModel::MODEL_TYPE_ENEMY)
	{
		// プレイヤーの情報
		CCharManager *pPlayer = nullptr;
		// ロボットの情報
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
		
		if (pPlayer != nullptr)
		{
			CharPos = pPlayer->GetPos();		// 位置
			CharRadius = pPlayer->GetRadius();	// 半径
			CharHp = pPlayer->GetLife();		// 体力

			// プレイヤーと弾の距離
			float fPDistance = CObject::Distance(CharPos, m_Pos);

			if (m_fRadius + CharRadius >= fPDistance)
			{
				// 体力の設定
				pPlayer->SetLife(pPlayer->Damage(CharHp, m_nAddDamage));

				Uninit();
			}
		}

		if (pRobot != nullptr)
		{
			CharPos = pRobot->GetPos();			// 位置
			CharRadius = pRobot->GetRadius();	// 半径
			CharHp = pRobot->GetLife();			// 体力

			// ロボットと弾の距離
			float fRDistance = CObject::Distance(CharPos, m_Pos);

			if (m_fRadius + CharRadius >= fRDistance)
			{
				// 体力の設定
				pRobot->SetLife(pRobot->Damage(CharHp, m_nAddDamage));

				Uninit();
			}
		}
	}
}