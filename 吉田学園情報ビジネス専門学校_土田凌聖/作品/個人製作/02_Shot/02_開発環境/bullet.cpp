//==============================================================================================
//
// 弾　　　bullet.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"bullet.h"
#include"player.h"
#include"exp.h"
#include"sound.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CBullet::CBullet() : CObject2D(1)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 初期位置
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量
	m_PlayerShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// プレイヤーの弾の移動量
	m_EnemyShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 敵の弾の移動量
	m_nType = 0;										// 弾の種類
	m_fWidth = 0.0f;									// 幅
	m_fHeight = 0.0f;									// 高さ
	m_fRot = 0.0f;										// 角度
	m_fPBulletLife = 0.0f;								// プレイヤーの弾の寿命
	m_fEBulletLife = 0.0f;								// 敵の弾の寿命
	m_fPAttack = 0.0f;									// プレイヤーの攻撃力
	m_fEAttack = 0.0f;									// 敵の攻撃力
	m_fPDecrease = 0.0f;								// プレイヤーの減衰係数
	m_fEDecrease = 0.0f;								// 敵の減衰係数
	m_fDistance = 0.0f;									// オブジェクトと弾の距離
	m_fRadius = 0.0f;									// 弾の半径
	m_bHit = false;										// 当たったか
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
HRESULT CBullet::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);
	m_Pos = pos;					// 位置
	m_InitPos = pos;				// 初期位置
	m_fWidth = BULLET_WIDTH;		// 幅
	m_fHeight = BULLET_HEIGHT;		// 高さ

	CObject2D::SetSize(m_fWidth,  m_fHeight);		// サイズと幅の設定

	// テクスチャの設定
	if (m_nType == BULLET_PLAYER)
	{// プレイヤーの弾
		CObject2D::SetTexture(CTexture::TEXTURE_PLAYERBULLET);
	}
	else if (m_nType == BULLET_ENEMY)
	{// 敵の弾
		CObject2D::SetTexture(CTexture::TEXTURE_ENEMYBULLET);
	}
	
	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CBullet::Update()
{
	CObject2D::Update();

	// プレイヤーの情報の格納先
	float fPlayerLife = 0.0f;						// 体力
	float fPlayerRad = 0.0f;						// 半径
	D3DXVECTOR3 PlayerPos = { 0.0f,0.0f,0.0f };		// 位置

	// 敵の情報の格納先
	float fEnemyLife = 0.0f;						// 体力
	float fEnemyRad = 0.0f;							// 半径
	D3DXVECTOR3 EnemyPos = { 0.0f,0.0f,0.0f };		// 位置

	// 餌の情報の格納先
	float fExpLife = 0.0f;							// 体力
	float fExpRad = 0.0f;							// 半径
	D3DXVECTOR3 ExpPos = { 0.0f,0.0f,0.0f };		// 位置

	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CObject *pObject;

			// オブジェクト情報の取得
			pObject = CObject::Getobject(nPri, nCnt);

			if (pObject != nullptr)
			{
				CObject::EObjType objType;
				objType = pObject->GetObjType();				// オブジェクトの種類の取得

				// 情報の取得
				if (objType == OBJTYPE_PLAYER)
				{// プレイヤーの情報の取得
					fPlayerLife = pObject->GetLife();		// 体力
					PlayerPos = pObject->GetPos();			// 位置
					fPlayerRad = pObject->GetRadius();		// 半径
				}
				else if (objType == OBJTYPE_ENEMY)
				{// 敵の情報の取得
					fEnemyLife = pObject->GetLife();		// 体力
					EnemyPos = pObject->GetPos();			// 位置
					fEnemyRad = pObject->GetRadius();		// 半径
				}
				else if (objType == OBJTYPE_EXP)
				{// 餌の情報の取得
					ExpPos = pObject->GetPos();
					fExpRad = pObject->GetRadius();
					fExpLife = pObject->GetLife();
				}

				if (m_nType == BULLET_PLAYER)
				{
					// 敵と弾の距離
					m_fDistance = sqrtf((EnemyPos.x - m_Pos.x) * (EnemyPos.x - m_Pos.x)
						+ (EnemyPos.y - m_Pos.y) * (EnemyPos.y - m_Pos.y));

					// 敵に弾が当たった
					if (fEnemyRad + m_fRadius >= m_fDistance
						&& m_bHit == false)
					{
						if (objType == OBJTYPE_ENEMY
							&& fEnemyLife > 0)
						{
							// 体力が0になる
							m_fPBulletLife = 0;
							m_PlayerShotMove.y = 0.0f;

							//サウンドの再生
							PlaySound(SOUND_LABEL_HIT_SE);

							fEnemyLife -= m_fPAttack;
							pObject->SetLife(0, m_fPAttack);
						}
					}
				}
				else if (m_nType == BULLET_ENEMY)
				{
					// プレイヤーと弾の距離
					m_fDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
						+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

					// プレイヤーに弾が当たった
					if (fPlayerRad + m_fRadius >= m_fDistance
						&& m_bHit == false)
					{
						if (objType == OBJTYPE_PLAYER
							&& fPlayerLife > 0)
						{
							// 体力が0になる
							m_fEBulletLife = 0;
							m_EnemyShotMove.y = 0.0f;

							//サウンドの再生
							PlaySound(SOUND_LABEL_HIT_SE);

							pObject->SetLife(0, m_fEAttack);
						}
					}
				}

				// 餌と弾の距離
				float m_fExpDistance = sqrtf((ExpPos.x - m_Pos.x) * (ExpPos.x - m_Pos.x)
					+ (ExpPos.y - m_Pos.y) * (ExpPos.y - m_Pos.y));

				// 餌に弾が当たった
				if (fExpRad + m_fRadius >= m_fExpDistance
					&& m_bHit == false)
				{
					if (m_nType == BULLET_PLAYER)
					{
						if (objType == OBJTYPE_EXP
							&& fExpLife > 0)
						{
							// 体力が0になる
							m_fPBulletLife = 0;
							m_PlayerShotMove.y = 0.0f;

							//サウンドの再生
							PlaySound(SOUND_LABEL_HIT_SE);

							// ダメージを与える
							pObject->SetLife(0, m_fPAttack);

							CExp::SetExpBreakType(CBullet::BULLET_PLAYER);	// 餌を壊した弾の種類
						}
					}
					else if (m_nType == BULLET_ENEMY)
					{
						if (objType == OBJTYPE_EXP
							&& fExpLife > 0)
						{
							// 体力が0になる
							m_fEBulletLife = 0;
							m_EnemyShotMove.y = 0.0f;

							// ダメージを与える
							pObject->SetLife(0, m_fEAttack);

							CExp::SetExpBreakType(CBullet::BULLET_ENEMY);	// 餌を壊した弾の種類
						}
					}
				}
			}
		}
	}

	D3DXVECTOR3 StopX = CPlayer::StopX(m_InitPos);
	D3DXVECTOR3 StopY = CPlayer::StopY(m_InitPos);

	m_Move = CPlayer::Move();

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_Move.x = 0.0f;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_Move.y = 0.0f;
	}

	bool Release = false;

	if (m_nType == BULLET_PLAYER)
	{// プレイヤーの弾
		Release = BulletMove(&m_fPBulletLife, &m_PlayerShotMove, m_fPDecrease);
	}
	else if (m_nType == BULLET_ENEMY)
	{// 敵の弾
		Release = BulletMove(&m_fEBulletLife, &m_EnemyShotMove, m_fEDecrease);
	}

	if (Release == true)
	{
		Uninit();
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CBullet::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 弾の生成
//==============================================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, float rot, int type, float life, float speed, float size, float attack, float decrease)
{
		CBullet *pBullet = nullptr;

		pBullet = new CBullet;

		if (pBullet != nullptr)
		{
			pBullet->SetType(type);
			pBullet->Init(pos);
			pBullet->SetSize(size, size);
			pBullet->SetRad(size);
			pBullet->SetRot(rot);

			if (type == 1)
			{
				pBullet->SetPBulletLife(life);
				pBullet->SetPBulletSpeed(speed);
				pBullet->SetPBulletAttack(attack);
				pBullet->SetPDecrease(decrease);
			}
			else
			{
				pBullet->SetEBulletLife(life);
				pBullet->SetEBulletSpeed(speed);
				pBullet->SetEBulletAttack(attack);
				pBullet->SetEDecrease(decrease);
			}
		}

		return pBullet;
}

//==============================================================================================
// 弾の移動処理
//==============================================================================================
bool CBullet::BulletMove(float *BulletLife, D3DXVECTOR3 *ShotMove, float Decrease)
{
	m_Pos += m_Move;

	// 位置の更新
	CObject2D::Setposition(m_Pos);

	bool bRelease = false;

	// 体力が0以上の場合
	if (*BulletLife > 0)
	{
		*BulletLife -= 1;

		// 速度の減衰
		ShotMove->y += (0.0f - ShotMove->y) * Decrease;

		D3DXVECTOR3 PosOld = m_Pos;

		// posにmoveを加算
		if (*BulletLife >= 0)
		{
			m_Pos.x += sinf(m_fRot) * -ShotMove->y;
			m_Pos.y += cosf(m_fRot) * -ShotMove->y;
		}
	}
	else
	{
		*ShotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bHit = true;

		// 弾が消える
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);
	}
	
	return bRelease;
}