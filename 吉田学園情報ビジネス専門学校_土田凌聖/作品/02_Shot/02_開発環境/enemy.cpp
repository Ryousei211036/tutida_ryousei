//==============================================================================================
//
// 敵　　　enemy.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include<time.h>
#include"enemy.h"
#include"player.h"
#include"hpgage.h"
#include"score.h"
#include"exp.h"
#include"fade.h"
#include"game.h"
#include"result.h"
#include"sound.h"
#include"statuslevel.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
int CEnemy::m_nNumEnemy = 0;
int CEnemy::m_nDeathCount = 0;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CEnemy::CEnemy() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 弾が発射される位置
	m_fSize = 0.0f;									// サイズ
	m_fWidth = 0.0f;								// 幅
	m_fHeight = 0.0f;								// 高さ
	m_fSpeed = 0;									// スピード
	m_fCoolTime = 0.0f;								// 弾のクールタイム
	m_fRadius = 0.0f;								// 半径
	m_fDamageCoolTime = 0.0f;						// 攻撃を受けた後のクールタイム
	m_fTargetDistance = 0.0f;						// ターゲットとの距離
	m_TargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ターゲットの位置
	m_bNearPlayer = false;	// プレイヤーに近いか
	m_fLife = 0;			// 体力
	m_fAttack = 0;			// 攻撃力
	m_fBodyDamage = 0.0f;	// 本体ダメージ
	m_fBulletSpeed = 0;		// 弾のスピード
	m_fBulletLife = 0.0f;	// 弾の体力
	m_nExpSave = 0;			// 経験値の保存先
	m_nLevel = 0;			// 敵の初期レベル
	m_nLevelSave = 0;		// レベルの保存
	m_nGiveExp = 0;			// 経験値
	m_nGrawLevel[3] = {};	// 進化するレベル
	m_bGrawUp = false;		// 進化したか
	m_nLvUpCount = 0;		// レベルが上がった回数
	m_bCreateHp = false;	// 体力ゲージが出たか
	m_fHpAlpha = 0.0f;		// 体力ゲージのアルファ値
	m_fTargetCount = 0;		// プレイヤーを狙う時間
	m_fHealLife = 0.0f;		// 自動回復量
	m_nScore = 0;			// スコア
	m_nNumEnemy++;			// 敵の数

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{
		//ステータスレベル
		m_nStatuslevel[nCnt] = 0;
	}
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	m_Pos = pos;									// 位置
	m_InitPos = pos;								// 初期位置
	m_fSize = ENEMY_SIZE;							// サイズ
	m_fWidth = ENEMY_WIDTH;							// 幅
	m_fHeight = ENEMY_HEIGHT;						// 高さ
	m_fRadius = ENEMY_RADIUS;						// 半径
	m_fTargetDistance = 10000.0f;					// ターゲットとの距離
	m_fLife = ENEMY_DEFAULTLIFE;					// 体力
	m_fMaxLife = ENEMY_DEFAULTMAXLIFE;				// 最大体力
	m_fBodyDamage = ENEMY_DEFAULTBODYDAMAGE;		// 本体ダメージ
	m_fBulletSpeed = ENEMY_BULLETSPEED;				// 弾の速度
	m_fBulletLife = ENEMY_BULLETLIFE;				// 弾の体力
	m_fAttack = ENEMY_DEFAULTATTACK;				// 攻撃力
	m_fReload = ENEMY_DEFAULTRELOATD;				// リロード速度
	m_fSpeed = ENEMY_DEFAULTSPEED;					// 移動スピード
	m_fBulletDecrease = BULLET_DEFAULTDECREASE;		// 弾の減衰係数
	m_nLevelSave = 1;								// レベルの保存
	m_nGiveExp = 40;								// 経験値
	m_nGrawLevel[0] = 5;							// 進化するレベル1
	m_nGrawLevel[1] = 20;							// 進化するレベル2
	m_nGrawLevel[2] = 30;							// 進化するレベル3
	m_EnemyType = ENEMY_TYPE0;						// プレイヤーのタイプ
	m_fTargetCount = TARGET_COUNT;					// プレイヤーを狙う時間
	m_nScore = 0;									// スコア

	CObject2D::Init(pos);
	CObject2D::SetSize(m_fWidth, m_fHeight);
	CObject2D::SetTexture(CTexture::TEXTURE_ENEMY0);	//テクスチャの設定
	
	// 体力ゲージの生成
	m_pHpgage = CHpGage::Create({ m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z }, 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{
		//ステータスレベル
		m_nStatuslevel[nCnt] = 1;
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CEnemy::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CEnemy::Update()
{
	CObject2D::Update();

	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);			// 弾が発射される位置

	m_BulletPos.x += sinf(m_fAngle) * -PLAYER_BULLETPOS;
	m_BulletPos.y += cosf(m_fAngle) * -PLAYER_BULLETPOS;

	// 移動量の減衰
	m_Move.x += (0.0f - m_Move.x) * ENEMY_DECREASE;
	m_Move.y += (0.0f - m_Move.y) * ENEMY_DECREASE;

	D3DXVECTOR3 StopX = CPlayer::StopX(m_InitPos);
	D3DXVECTOR3 StopY = CPlayer::StopY(m_InitPos);

	m_PlayerMove = CPlayer::Move();

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_PlayerMove.x = 0.0f;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_PlayerMove.y = 0.0f;
	}

	m_TotalMove = m_PlayerMove + m_Move;

	//移動量の更新
	m_Pos += m_TotalMove;

	CObject2D::Setposition(m_Pos);

	CFade::FADE FadeMode = CFade::GetFade();
	CApplication::MODE GameMode = CApplication::GetMode();

	if (FadeMode == CFade::FADE_NONE
		&& GameMode == CApplication::MODE_GAME)
	{
		D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPos();
		float PlayerRad = CGame::GetPlayer()->GetRadius();
		float PlayerBodyDamage = CGame::GetPlayer()->GetBodyDamage();

		// 敵とプレイヤーの距離
		float m_fDistance = sqrtf((m_Pos.x - PlayerPos.x) * (m_Pos.x - PlayerPos.x)
			+ (m_Pos.y - PlayerPos.y) * (m_Pos.y - PlayerPos.y));

		m_fDamageCoolTime--;

		// 敵とプレイヤーが当たった
		if (m_fDistance <= PlayerRad + m_fRadius
			&& m_fDamageCoolTime <= 0)
		{
			SetLife(0, PlayerBodyDamage);
			CGame::GetPlayer()->SetLife(0, m_fBodyDamage);

			m_fDamageCoolTime = ENEMY_DAMAGECOOLTIME;
		}

		if (m_fDistance < 500)
		{
			m_bNearPlayer = true;
		}
		else
		{
			m_bNearPlayer = false;
		}
	}

	bool bRelease = false;

	if (m_fLife > 0)
	{
		// 敵の挙動
		Move();

		if (m_fMaxLife > m_fLife && m_fHealLife > 0)
		{
			// 体力の自動回復
			SetLife(1, m_fHealLife);

			m_nHpCoolTime = HP_COOLTIME;

			if (m_fMaxLife <= m_fLife)
			{
				m_fLife = m_fMaxLife;
			}
		}

		// 目的の角度の設定
		m_fAngle = (float)atan2(m_Pos.x - m_TargetPos.x, m_Pos.y - m_TargetPos.y);

		CObject2D::Setrot(m_fAngle);

		// 弾を撃つ処理
		Shot();
	}
	else
	{
		// 敵が消える
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);

		m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (bRelease == false)
	{
		if (m_pHpgage != nullptr)
		{
			if (m_bCreateHp == true)
			{
				// 体力ゲージが消えるまでの時間
				m_nHpCoolTime--;

				if (m_nHpCoolTime <= 0 && m_fLife >= m_fMaxLife)
				{
					m_fHpAlpha = 0.0f;		// 一定時間後体力ゲージを透明にする
					m_bCreateHp = false;
					m_nHpCoolTime = HP_COOLTIME;
				}
			}
			// 体力ゲージの位置
			m_pHpgage->SetPos(D3DXVECTOR3(m_Pos.x - 18.0f, m_Pos.y + 30, m_Pos.z));

			// 体力ゲージの色の設定
			m_pHpgage->SetCol({ 0.0f,1.0f,0.0f, m_fHpAlpha });
		}
	}
	else
	{
		for (int nPri = 0; nPri < MAX_PRI; nPri++)
		{
			for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
			{
				CObject *pObject;
				pObject = CObject::Getobject(nPri, nCnt);

				if (pObject != nullptr)
				{
					CObject::EObjType objType;
					objType = pObject->GetObjType();		// オブジェクトの種類

					if (objType == OBJTYPE_PLAYER)
					{// プレイヤーに経験値を渡す
						// 経験値の設定
						m_nGiveExp += m_nLevel;

						pObject->SetExp(m_nGiveExp);
						m_nScore += 50;

						// スコア加算
						CGame::GetScore()->AddScore(m_nScore);

						Uninit();
						m_nNumEnemy--;			// 敵の数
						m_nDeathCount++;		// 倒した数

						break;
					}
				}
			}
		}
	}

	// レベルアップ
	LevelUp();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CEnemy::Draw()
{
	CObject2D::Draw();

#ifdef _DEBUG
	m_Font = CRenderer::GetFont();

	//Enemy(m_Font);
#endif // _DEBUG
}

//==============================================================================================
// 敵の生成
//==============================================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, int Level)
{
	CEnemy *pEnemy = nullptr;

	pEnemy = new CEnemy;

	if (pEnemy != nullptr)
	{
		pEnemy->Init(pos);
		pEnemy->SetLevel(Level);
	}

	return pEnemy;
}

//==============================================================================================
//移動処理
//==============================================================================================
void CEnemy::Move()
{
	// プレイヤーと餌の位置
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 TrianglePos;
	D3DXVECTOR3 BoxPos;
	D3DXVECTOR3 PentagonPos;

	// プレイヤー、餌との距離
	float fPlayerDistance = 0.0f;
	float fTriangleDistance = 0.0f;
	float fBoxDistance = 0.0f;
	float fPentagonDistance = 0.0f;

	// 餌の体力
	float fPlayerLife = 0.0f;
	float fTrinagleLife = 0.0f;
	float fBoxLife = 0.0f;
	float fPentagonLife = 0.0f;

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

				if (objType == OBJTYPE_PLAYER)
				{// プレイヤーの情報の取得
					PlayerPos = pObject->GetPos();				// 位置
					fPlayerLife = pObject->GetLife();			// 体力

					// 敵とプレイヤーの距離
					fPlayerDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
						+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

					if (m_fTargetDistance > fPlayerDistance)
					{// プレイヤー
						m_fTargetDistance = fPlayerDistance;
						m_TargetPos = PlayerPos;
					}

					if (fPlayerDistance >= 300.0f + 100.0f
						|| fPlayerLife <= 0)
					{
						m_fTargetDistance = 10000.0f;
					}
				}
				else if (objType == OBJTYPE_EXP)
				{
					CApplication::MODE GameMode = CApplication::GetMode();
					CExp::Exptype ExpType = CExp::EXPTYPE_NONE;

					if (GameMode == CApplication::MODE_GAME)
					{
						ExpType = CGame::GetExp()->GetExpType();
					}
					else
					{
						ExpType = CResult::GetExp()->GetExpType();
					}

					if (ExpType == CExp::EXPTYPE_TRIANGLE)
					{// 三角の場合
						int nNum = pObject->GetNumTriangle();	// 三角の数
						TrianglePos = pObject->GetPos();		// 位置
						fTrinagleLife = pObject->GetLife();		// 体力

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// 敵と三角餌の距離
							fTriangleDistance = sqrtf((TrianglePos.x - m_Pos.x) * (TrianglePos.x - m_Pos.x)
								+ (TrianglePos.y - m_Pos.y) * (TrianglePos.y - m_Pos.y));

							if (m_fTargetDistance > fTriangleDistance)
							{// 三角餌
								fTrinagleLife = pObject->GetLife();		// 体力の取得
								m_fTargetDistance = fTriangleDistance;
								m_TargetPos = TrianglePos;
							}

							if (fTrinagleLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
					else if (ExpType == CExp::EXPTYPE_BOX)
					{// 四角の場合
						int nNum = pObject->GetNumBox();		// 四角の数
						BoxPos = pObject->GetPos();				// 位置
						fBoxLife = pObject->GetLife();			// 体力

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// 敵と四角餌の距離
							fBoxDistance = sqrtf((BoxPos.x - m_Pos.x) * (BoxPos.x - m_Pos.x)
								+ (BoxPos.y - m_Pos.y) * (BoxPos.y - m_Pos.y));

							if (m_fTargetDistance > fBoxDistance)
							{// 四角餌
								fBoxLife = pObject->GetLife();		// 体力の取得
								m_fTargetDistance = fBoxDistance;
								m_TargetPos = BoxPos;
							}

							if (fBoxLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
					else if (ExpType == CExp::EXPTYPE_PENTAGON)
					{// 五角形の場合
						int nNum = pObject->GetNumPentagon();	// 五角形の数
						PentagonPos = pObject->GetPos();		// 位置
						fPentagonLife = pObject->GetLife();		// 体力

						for (int nCntExp = 0; nCntExp < nNum; nCntExp++)
						{
							// 敵と五角形餌の距離
							fPentagonDistance = sqrtf((PentagonPos.x - m_Pos.x) * (PentagonPos.x - m_Pos.x)
								+ (PentagonPos.y - m_Pos.y) * (PentagonPos.y - m_Pos.y));

							if (m_fTargetDistance > fPentagonDistance)
							{// 五角形
								fPentagonLife = pObject->GetLife();		// 体力の取得
								m_fTargetDistance = fPentagonDistance;
								m_TargetPos = PentagonPos;
							}

							if (fPentagonLife <= 0)
							{
								m_fTargetDistance = 10000.0f;
							}
						}
					}
				}
			}
		}
	}

	// ターゲットとの距離が離れている場合
	if (m_fTargetDistance >= 300.0f
		&& m_EnemyType != ENEMY_TYPE8)
	{
		// ターゲットに近づく
		m_Move.x = sinf(m_fAngle) * -m_fSpeed;
		m_Move.y = cosf(m_fAngle) * -m_fSpeed;
	}
	else if (m_fTargetDistance >= 30.0f
		&& m_EnemyType == ENEMY_TYPE8)
	{
		// ターゲットに近づく
		m_Move.x = sinf(m_fAngle) * -m_fSpeed;
		m_Move.y = cosf(m_fAngle) * -m_fSpeed;
	}

	if (m_bHit == true)
	{
		m_fTargetDistance = fPlayerDistance;
		m_TargetPos = PlayerPos;

		if (m_fTargetDistance >= 300.0f)
		{
			m_Move.x = sinf(m_fAngle) * -m_fSpeed;
			m_Move.y = cosf(m_fAngle) * -m_fSpeed;
		}
		
		m_fTargetCount--;

		if(m_fTargetCount <= 0)
		{
			m_bHit = false;
			m_fTargetCount = TARGET_COUNT;
		}
	}
}

//==============================================================================================
// 弾を撃つ処理
//==============================================================================================
void CEnemy::Shot()
{
	m_fCoolTime--;

	if (m_fCoolTime <= 0.0f)
	{
		if (m_bNearPlayer == true)
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_SHOT_SE);
		}

		// 弾を撃つ
		if (m_EnemyType == ENEMY_TYPE0)
		{// プレイヤータイプ０の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE1)
		{// プレイヤータイプ１の場合
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI / 2) * 10.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI / 2) * 10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI / 2) * -10.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI / 2) * -10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE2)
		{// プレイヤータイプ２の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.85f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.85f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE3)
		{// プレイヤータイプ３の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed * 1.8f, BULLET_SIZE - 5.0f, m_fAttack * 0.7f, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE4)
		{// プレイヤータイプ４の場合		
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.y - cosf(m_fAngle - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE5)
		{// プレイヤータイプ５の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE6)
		{// プレイヤータイプ６の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE + 7.0f, m_fAttack * 1.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 20.0f;
		}
		else if (m_EnemyType == ENEMY_TYPE7)
		{// プレイヤータイプ７の場合
			CBullet::Create(m_BulletPos, m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI) * 50.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI) * 50.0f, m_BulletPos.z), m_fAngle + D3DX_PI, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fAngle + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.y - cosf(m_fAngle + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.5f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.5f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.75f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.75f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_EnemyType == ENEMY_TYPE8)
		{// プレイヤータイプ８の場合
		}
		else if (m_EnemyType == ENEMY_TYPE9)
		{// プレイヤータイプ９の場合
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle) * -10.0f, m_BulletPos.y + cosf(m_fAngle) * -10.0f, m_BulletPos.z), m_fAngle, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed + 1.0f, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.05f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.05f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.1f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.1f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.15f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.15f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.2f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fAngle + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fAngle - D3DX_PI * 0.25f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fAngle - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fAngle - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fAngle + D3DX_PI * 0.25f, CBullet::BULLET_ENEMY, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 20.0f;
		}

		if (m_EnemyType != ENEMY_TYPE6
			&& m_EnemyType != ENEMY_TYPE9)
		{
			m_fCoolTime = m_fReload;
		}
	}
}

//==============================================================================================
// レベルアップ処理
//==============================================================================================
void CEnemy::LevelUp()
{
	srand((unsigned int)time(0));

	if (m_nLvUpCount + 1 < m_nLevel)
	{
		int nStatus = rand() % 8 + 1;

		// ステータスの強化
		if (nStatus == 1)
		{// 体力の最大値UP
			m_nStatuslevel[0]++;

			if (m_nStatuslevel[0] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				float fOldLife = m_fMaxLife;

				m_fMaxLife += 5;

				float fOldDamagePercent;
				float fDamagePercent;

				// 受けたダメージの割合の計算
				fOldDamagePercent = (m_fTotalDamage / fOldLife) * 100;
				fDamagePercent = (m_fTotalDamage / m_fMaxLife) * 100;

				float nDamage = fOldDamagePercent - fDamagePercent;

				float fLife = m_fMaxLife - fOldLife;

				// 体力ゲージを増やす
				m_pHpgage->AddHpgage(nDamage);

				// 体力を増やす
				m_fLife += fLife;

				m_fHpAlpha = 1.0f;
				m_nHpCoolTime = HP_COOLTIME;
				m_bCreateHp = true;

				m_nScore += 5;
			}
		}
		else if (nStatus == 2)
		{// 自動回復量UP
			m_nStatuslevel[1]++;

			if (m_nStatuslevel[1] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fHealLife += m_fMaxLife * 0.0002f;

				m_nScore += 3;
			}
		}
		else if (nStatus == 3)
		{// 本体の攻撃力UP
			m_nStatuslevel[2]++;

			if (m_nStatuslevel[2] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBodyDamage += 4;

				m_nScore += 5;
			}
		}
		else if (nStatus == 4)
		{// 弾の速度UP
			m_nStatuslevel[3]++;

			if (m_nStatuslevel[3] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBulletSpeed++;

				m_nScore += 3;
			}
		}
		else if (nStatus == 5)
		{// 弾の体力
			m_nStatuslevel[4]++;

			if (m_nStatuslevel[4] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fBulletLife += 50.0f;
				m_fBulletDecrease -= 0.001f;

				m_nScore += 3;
			}
		}
		else if (nStatus == 6)
		{// 攻撃力UP
			m_nStatuslevel[5]++;

			if (m_nStatuslevel[5] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fAttack += 2;

				m_nScore += 5;
			}
		}
		else if (nStatus == 7)
		{// リロード速度UP
			m_nStatuslevel[6]++;

			if (m_nStatuslevel[6] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fReload -= 2.0f;

				m_nScore += 5;
			}
		}
		else if (nStatus == 8)
		{// 移動速度UP
			m_nStatuslevel[7]++;

			if (m_nStatuslevel[7] <= MAX_STATUSLEVLE)
			{
				m_nLvUpCount++;

				m_fSpeed += 0.2f;

				m_nScore += 3;
			}
		}
	}

	int evolution = rand() % 10 + 1;

	// 進化１回目
	if (m_nLvUpCount >= m_nGrawLevel[0]
		&& m_bGrawUp == false)
	{
		if (evolution == 1)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY1);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE1;
		}
		else if (evolution == 2)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY2);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE2;
		}
		else if (evolution == 3)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY3);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE3;
		}
		else if (evolution == 4)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY4);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE4;
		}
		else if (evolution == 5)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY5);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE5;
		}
		else if (evolution == 6)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY6);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE6;

			m_fSpeed /= 1.5;
		}
		else if (evolution == 7)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY7);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE7;
		}
		else if (evolution == 8)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY8);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE8;

			m_fHealLife += 0.5f;
			m_fBodyDamage += 10;
		}
		else if (evolution == 9)
		{
			CObject2D::SetTexture(CTexture::TEXTURE_ENEMY9);	//テクスチャの設定
			m_EnemyType = ENEMY_TYPE9;
		}

		m_bGrawUp = true;
	}
}

//==============================================================================================
// 体力ゲージが減る処理
//==============================================================================================
void CEnemy::HitDamage(float damage)
{
	float fDamagePercent;

	// 受けたダメージの割合の計算
	fDamagePercent = (damage / m_fMaxLife) * 100;

	// 体力ゲージを減らす
	m_pHpgage->SubHpgage(fDamagePercent);
}

//==============================================================================================
// 体力ゲージが増える処理
//==============================================================================================
void CEnemy::HealHp(float heal)
{
	float fHealPercent;

	// 回復量の割合の計算
	fHealPercent = (heal / m_fMaxLife) * 100;

	// 体力ゲージを増やす
	m_pHpgage->AddHpgage(fHealPercent);
}

//==============================================================================================
// 体力の設定
//==============================================================================================
void CEnemy::SetLife(int type, float life)
{
	CApplication::MODE GameMode = CApplication::GetMode();

	if (GameMode == CApplication::MODE_GAME)
	{
		if (type == 0)
		{// 体力が減る
			float fLife = m_fLife;

			m_fLife -= life;		// 体力を減らす
			m_fTotalDamage += life;	// 受けたダメージの合計

			if (fLife > 0)
			{
				// ダメージを与える処理
				HitDamage(life);

				m_bHit = true;
			}
		}
		else
		{
			if (m_fLife < m_fMaxLife)
			{
				// 体力を回復する
				m_fLife += life;

				// 回復する処理
				HealHp(life);
			}
		}

		m_fHpAlpha = 1.0f;
		m_nHpCoolTime = HP_COOLTIME;
		m_bCreateHp = true;
	}
}

//==============================================================================================
// レベルの設定
//==============================================================================================
void CEnemy::SetLevel(int Level)
{
	m_nLevel = Level;
}

void CEnemy::Enemy(LPD3DXFONT pFont)
{
	RECT rect;
	TCHAR str[256];

	for (int nCnt = 0; nCnt < 9; nCnt++)
	{
		rect = { 0, 0 + nCnt * 20, 1280, 720 };

		if (nCnt == 0)
		{
			sprintf(str, ("最大体力　%.1f"), m_fMaxLife);
		}
		else if (nCnt == 1)
		{
			sprintf(str, ("回復量　%.1f"), m_fHealLife);
		}
		else if (nCnt == 2)
		{
			sprintf(str, ("本体ダメージ　%.1f"), m_fBodyDamage);
		}
		else if (nCnt == 3)
		{
			sprintf(str, ("弾の速度　%.1f"), m_fBulletSpeed);
		}
		else if (nCnt == 4)
		{
			sprintf(str, ("弾の体力　%.1f"), m_fBulletLife);
		}
		else if (nCnt == 5)
		{
			sprintf(str, ("攻撃力　%.1f"), m_fAttack);
		}
		else if (nCnt == 6)
		{
			sprintf(str, ("リロード　%.1f"), m_fReload);
		}
		else if (nCnt == 7)
		{
			sprintf(str, ("移動速度　%.1f"), m_fSpeed);
		}
		else if (nCnt == 8)
		{
			sprintf(str, ("レベルをあげる回数　%d"), m_nLevel);
		}
		// テキスト描画
		pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));
	}
}