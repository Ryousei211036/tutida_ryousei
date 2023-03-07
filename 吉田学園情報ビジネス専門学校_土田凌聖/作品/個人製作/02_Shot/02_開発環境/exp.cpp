//==============================================================================================
//
// 餌　　　exp.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"exp.h"
#include"player.h"
#include"hpgage.h"
#include"score.h"
#include"game.h"
#include"fade.h"
#include"sound.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
D3DXVECTOR3 CExp::m_CreatePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CExp::m_nNumExp = 0;
int CExp::m_nNumTriangle = 0;
int CExp::m_nNumBox = 0;
int CExp::m_nNumPentagon = 0;
CBullet::BULLET_TYPE CExp::m_BulletType = CBullet::BULLET_NONE;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CExp::CExp() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 初期位置
	m_fSize = 0.0f;					// サイズ
	m_fWidth = 0.0f;				// 幅
	m_fHeight = 0.0f;				// 高さ
	m_nType = EXPTYPE_NONE;			// 種類
	m_fLife = 0;					// 体力
	m_fRadius = 0.0f;				// 半径
	m_nExp = 0;						// 経験値
	m_fBodyDamage = 0;				// 本体ダメージ
	m_fDamageCoolTime = 0;			// 攻撃をくらった後のクールタイム
	m_bDamage = false;				// 攻撃をくらったか
	m_bCreateHp = false;			// 体力ゲージが出たか
	m_nHpCoolTime = 0;				// 体力ゲージが消えるまでの時間
	m_fHpAlpha = 0.0f;				// 体力ゲージのアルファ値
	m_fHpGage = 0.0f;				// 体力ゲージの長さ
	m_nBodyDamageType = 0;			// プレイヤーと敵どっちに当たったか
	m_nNumExp++;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CExp::~CExp()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CExp::Init(D3DXVECTOR3 pos)
{
	CObject2D::Init(pos);

	m_Pos = pos;								// 位置
	m_fRadius = EXP_RADIUS;						// 半径
	m_nExp = 0;									// 経験値
	m_fBodyDamage = EXP_BODYDAMAGE;				// 本体ダメージ
	m_fDamageCoolTime = EXP_DAMAGECOOLTIME;		// 攻撃をくらった後のクールタイム
	m_bDamage = false;							// 攻撃をくらったか
	m_fWidth = EXP_WIDTH;						// 幅
	m_fHeight = EXP_HEIGHT;						// 高さ
	m_nHpCoolTime = HP_COOLTIME;				// 体力ゲージが消えるまでの時間
	m_fHpGage = HP_WIDTH;						// 体力ゲージの長さ

	// 体力ゲージの生成
	m_pHpgage = CHpGage::Create(D3DXVECTOR3(m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z), 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	if (m_nType == EXPTYPE_TRIANGLE)
	{// 三角
		m_fLife = EXP_TRIANGLELIFE;								// 体力の設定
		m_fMaxLife = EXP_TRIANGLELIFE;							// 最大体力の設定
		m_fSize = EXP_TRIANGLESIZE;								// サイズの設定
		CObject2D::SetTexture(CTexture::TEXTURE_EXTRIANGLE);	// テクスチャの設定
		m_nNumTriangle++;										// 数
	}
	else if (m_nType == EXPTYPE_BOX)
	{// 四角
		m_fLife = EXP_BOXLIFE;									// 体力の設定
		m_fMaxLife = EXP_BOXLIFE;								// 最大体力の設定
		m_fSize = EXP_BOXSIZE;									// サイズの設定
		CObject2D::SetTexture(CTexture::TEXTURE_EXBOX);			// テクスチャの設定
		m_nNumBox++;											// 数
	}
	else if (m_nType == EXPTYPE_PENTAGON)
	{// 五角形
		m_fLife = EXP_PENTAGONLIFE;								// 体力の設定
		m_fMaxLife = EXP_PENTAGONLIFE;							// 最大体力の設定
		m_fSize = EXP_PENTAGONSIZE;								// サイズの設定
		CObject2D::SetTexture(CTexture::TEXTURE_EXPENTAGON);	// テクスチャの設定
		m_nNumPentagon++;										// 数
	}

	CObject2D::SetSize(m_fSize, m_fSize);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CExp::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CExp::Update()
{
	CObject2D::Update();

	// プレイヤーの情報の格納先
	D3DXVECTOR3 PlayerPos = { 0.0f,0.0f,0.0f };		// 位置
	float fPlayerSize = 0.0f;						// サイズ
	float fPlayerBodyDamage = 0.0f;					// 本体攻撃力
	float fPlayerDistance = 0.0f;					// 距離
	float fPlayerAngle = 0.0f;						// 角度

	// 敵の情報の格納先
	D3DXVECTOR3 EnemyPos = { 0.0f,0.0f,0.0f };		// 位置
	float fEnemySize = 0.0f;						// サイズ
	float fEnemyBodyDamage = 0.0f;					// 本体攻撃力
	float fEnemyDistance = 0.0f;					// 距離
	float fEnemyAngle = 0.0f;						// 角度

	for (int nPri = 0; nPri < MAX_PRI; nPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			CObject *pObject;
			pObject = CObject::Getobject(nPri, nCnt);

			if (pObject != nullptr)
			{
				CObject::EObjType objType;
				objType = pObject->GetObjType();			// オブジェクトの種類

				if (objType == OBJTYPE_PLAYER)
				{
					PlayerPos = pObject->GetPos();					// プレイヤーの位置
					fPlayerSize = pObject->GetSize();				// プレイヤーのサイズ
					fPlayerBodyDamage = pObject->GetBodyDamage();	// プレイヤーの本体攻撃力

					if (m_fLife >= 0)
					{
						// プレイヤーと餌の距離
						fPlayerDistance = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x)
							+ (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y));

						// 当たり判定
						if (m_fSize + fPlayerSize >= fPlayerDistance)
						{
							// プレイヤーと餌の角度
							fPlayerAngle = (float)atan2(m_Pos.x - PlayerPos.x, m_Pos.y - PlayerPos.y);

							// プレイヤーに押し出される
							m_PushMove.x = sinf(fPlayerAngle);
							m_PushMove.y = cosf(fPlayerAngle);

							if (m_bDamage == false)
							{
								//サウンドの再生
								PlaySound(SOUND_LABEL_HIT_SE);

								// 餌の体力の設定
								SetLife(0, fPlayerBodyDamage);

								// プレイヤーに当たった
								m_nBodyDamageType = 1;

								// プレイヤーの体力の設定
								pObject->SetLife(0, EXP_BODYDAMAGE);
							}

							m_bDamage = true;
						}
					}
				}
				else if (objType == OBJTYPE_ENEMY)
				{
					EnemyPos = pObject->GetPos();					// 敵の位置
					fEnemySize = pObject->GetSize();				// 敵のサイズ
					fEnemyBodyDamage = pObject->GetBodyDamage();	// 敵の本体攻撃力

					if (m_fLife >= 0)
					{
						// 敵と餌の距離
						fEnemyDistance = sqrtf((EnemyPos.x - m_Pos.x) * (EnemyPos.x - m_Pos.x)
							+ (EnemyPos.y - m_Pos.y) * (EnemyPos.y - m_Pos.y));

						if (m_fSize + fEnemySize >= fEnemyDistance)
						{
							// 敵と餌の角度
							fEnemyAngle = (float)atan2(m_Pos.x - EnemyPos.x, m_Pos.y - EnemyPos.y);

							// 敵に押し出される
							m_PushMove.x = sinf(fEnemyAngle);
							m_PushMove.y = cosf(fEnemyAngle);

							if (m_bDamage == false)
							{
								// 餌の体力の設定
								SetLife(0, fEnemyBodyDamage);

								// 敵に当たった
								m_nBodyDamageType = 2;

								// 敵の体力の設定
								pObject->SetLife(0, EXP_BODYDAMAGE);
							}

							m_bDamage = true;
						}
					}
				}
			}
		}
	}

	bool bRelease = false;

	if (m_fLife > 0)
	{
		if (m_bDamage == true)
		{
			m_fDamageCoolTime--;

			if (m_fDamageCoolTime <= 0)
			{
				m_fDamageCoolTime = EXP_DAMAGECOOLTIME;
				m_bDamage = false;
			}
		}

		// 移動量の減衰
		m_PushMove.x += (0.0f - m_PushMove.x) * EXP_DECREASE;
		m_PushMove.y += (0.0f - m_PushMove.y) * EXP_DECREASE;

		m_Pos += m_PushMove;

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
		// 餌が消える
		bRelease = CObject2D::SetDisappear(30.0f, 0.0f, 1.0f, 0.05f);

		m_PushMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (bRelease == true)
	{
		// 経験値の設定
		if (m_nType == EXPTYPE_TRIANGLE)
		{// 三角
			m_nExp = 15;
			m_nScore = 20;
		}
		else if (m_nType == EXPTYPE_BOX)
		{// 四角
			m_nExp = 10;
			m_nScore = 10;
		}
		else if (m_nType == EXPTYPE_PENTAGON)
		{// 五角形
			m_nExp = 20;
			m_nScore = 30;
		}

		if (m_nBodyDamageType == 1
			|| m_BulletType == CBullet::BULLET_PLAYER)
		{
			// プレイヤーに経験値を渡す
			CGame::GetPlayer()->SetExp(m_nExp);

			// スコア加算
			CGame::GetScore()->AddScore(m_nScore);
		}

		m_nNumExp--;

		// 解放
		Uninit();
	}

	CFade::FADE FadeMode = CFade::GetFade();
	CApplication::MODE GameMode = CApplication::GetMode();

	if (FadeMode == CFade::FADE_NONE
		&& GameMode == CApplication::MODE_GAME)
	{
		float PLife = CGame::GetPlayer()->GetLife();

		if (PLife <= 0)
		{
			m_nNumExp = 0;
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

	m_TotalMove = m_PushMove + m_Move;

	// 位置の更新
	m_Pos += m_TotalMove;

	// 位置の設定
	CObject2D::Setposition(m_Pos);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CExp::Draw()
{
	CObject2D::Draw();
}

//==============================================================================================
// 餌の生成
//==============================================================================================
CExp *CExp::Create(D3DXVECTOR3 pos, int type, float rot)
{
	CExp *pExp = nullptr;

	pExp = new CExp;

	if (pExp != nullptr)
	{
		pExp->SetType(type);
		pExp->Init(pos);
		pExp->Setrot(rot);
	}

	return pExp;
}

//==============================================================================================
// 体力の設定
//==============================================================================================
void CExp::SetLife(int type, float life)
{
	float fLife = 0;

	if (type == 0)
	{// 体力が減る
		fLife = m_fLife;
		m_fLife -= life;		// 体力を減らす
	}
	else
	{// 体力が増える
		m_fLife += life;		// 体力を回復する
	}

	m_fHpAlpha = 1.0f;
	m_nHpCoolTime = HP_COOLTIME;
	m_bCreateHp = true;

	if (fLife > 0)
	{
		HitDamage(life);
	}
}

//==============================================================================================
// 受けたダメージの割合の取得
//==============================================================================================
float CExp::HitDamage(float damage)
{
	float fDamagePercent = 0;

	// 受けたダメージの割合の計算
	if (m_nType == EXPTYPE_TRIANGLE)
	{// 三角
		fDamagePercent = (damage / EXP_TRIANGLELIFE) * 100;
	}
	if (m_nType == EXPTYPE_BOX)
	{// 四角
		fDamagePercent = (damage / EXP_BOXLIFE) * 100;
	}
	if (m_nType == EXPTYPE_PENTAGON)
	{// 五角形
		fDamagePercent = (damage / EXP_PENTAGONLIFE) * 100;
	}

	// 体力ゲージを減らす
	m_pHpgage->SubHpgage(fDamagePercent);

	return fDamagePercent;
}

//==============================================================================================
// 経験値の種類の取得
//==============================================================================================
CExp::Exptype CExp::GetExpType()
{
	if (m_nType == EXPTYPE_TRIANGLE)
	{// 三角
		return CExp::EXPTYPE_TRIANGLE;
	}
	else if (m_nType == EXPTYPE_BOX)
	{// 四角
		return CExp::EXPTYPE_BOX;
	}
	else if (m_nType == EXPTYPE_PENTAGON)
	{// 五角形
		return CExp::EXPTYPE_PENTAGON;
	}
	else
	{
		return CExp::EXPTYPE_NONE;
	}
}