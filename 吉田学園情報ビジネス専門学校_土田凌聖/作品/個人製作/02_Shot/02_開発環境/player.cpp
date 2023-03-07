//==============================================================================================
//
// プレイヤー　　　player.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"fade.h"
#include"player.h"
#include"inputkeyboard.h"
#include"bullet.h"
#include"hpgage.h"
#include"expgage.h"
#include"statusdisplay.h"
#include"statuslevel.h"
#include"evodisplay.h"
#include"leveldisplay.h"
#include"game.h"
#include"sound.h"

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
float CPlayer::m_fLife = 0.0f;										// 体力
D3DXVECTOR3 CPlayer::m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
D3DXVECTOR3 CPlayer::m_BulletPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 弾が発射される位置
float CPlayer::m_fRot = 0.0f;										// 角度
float CPlayer::m_fCoolTime = 0.0f;									// 弾が発射されるまでの時間
D3DXVECTOR3 CPlayer::m_ZeroPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 0の位置
float CPlayer::m_fSpeed = 0.0f;										// 移動速度
bool CPlayer::m_bCollisionX = false;
bool CPlayer::m_bCollisionY = false;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CPlayer::CPlayer() : CObject2D(2)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_fSize = 0.0f;			// サイズ
	m_fWidth = 0.0f;		// 幅
	m_fHeight = 0.0f;		// 高さ
	m_fRadius = 0.0f;		// 当たり判定の半径
	m_fAttack = 0;			// ダメージ
	m_fBodyDamage = 0.0f;	// 本体ダメージ
	m_fMaxLife = 0;			// 体力の最大値
	m_fBulletSpeed = 0;		// 弾の速度
	m_fBulletLife = 0;		// 弾の体力
	m_nExp = 0;				// 経験値
	m_nLevelUpExp = 0;		// レベルアップに必要な経験値
	m_nLevel = 0;			// プレイヤーのレベル
	m_nCntLevel = 0;		// レベルが上がった回数
	m_nLevelSave = 0;		// レベルの保存
	m_nGrawLevel[3] = {};	// 進化レベル
	m_nGrawCount = 0;		// 進化した回数
	m_bLevelUp = false;		// レベルが上がったか
	m_bCreateHp = false;	// 体力ゲージが出たか
	m_nHpCoolTime = 0;		// 体力ゲージが消えるまでの時間
	m_fHpAlpha = 0.0f;		// 体力ゲージのアルファ値
	m_fStatusAlpha = 0.0f;	// ステータス表示のアルファ値
	m_fEvoAlpha = 0.0f;		// 進化先表示のアルファ値
	m_PlayerType = PLAYER_NONE;	// プレイヤータイプ

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{// ステータスレベルの表示
		m_pStatuslevel[nCnt] = nullptr;

		//ステータスレベル
		m_nStatuslevel[nCnt] = 0;
	}
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CPlayer::~CPlayer()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	m_Pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);		// 位置
	m_ZeroPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 0の位置
	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y - 100, 0.0f);			// 弾が発射される位置
	m_fRot = 0.0f;								// 角度
	m_fSize = PLAYER_SIZE;						// サイズ
	m_fWidth = PLAYER_WIDTH;					// 幅
	m_fHeight = PLAYER_HEIGHT;					// 高さ
	m_fCoolTime = 0;							// 弾が発射されるまでの時間
	m_fRadius = PLAYER_RADIUS;					// 当たり判定の半径
	m_fAttack = PLAYER_DEFAULTATTACK;			// プレイヤーの攻撃力
	m_fBodyDamage = PLAYER_DEFAULTBODYDAMAGE;	// 本体ダメージ
	m_fSpeed = PLAYER_DEFAULTSPEED;				// スピード
	m_fReload = PLAYER_DEFAULTRELOATD;			// リロード時間
	m_fLife = PLAYER_DEFAULTLIFE;				// プレイヤーの体力
	m_fMaxLife = PLAYER_DEFAULTMAXLIFE;			// プレイヤーの体力の最大値
	m_fBulletSpeed = PLAYER_BULLETSPEED;		// 弾の速度
	m_fBulletLife = PLAYER_BULLETLIFE;			// 弾の体力
	m_nExp = 0;									// 経験値
	m_nLevelUpExp = 50;							// レベルアップに必要な経験値
	m_nLevel = 1;								// プレイヤーの初期レベル
	m_nCntLevel = 0;							// レベルが上がった回数
	m_nLevelSave = 1;							// レベルの保存
	m_nGrawLevel[0] = 5;						// 進化レベル1
	m_nGrawLevel[1] = 15;						// 進化レベル2
	m_nGrawLevel[2] = 25;						// 進化レベル3
	m_nGrawCount = 0;							// 進化した回数
	m_bLevelUp = false;							// レベルが上がったか
	m_PlayerType = PLAYER_TYPE0;				// プレイヤータイプ
	m_nHpCoolTime = HP_COOLTIME;				// 体力ゲージが消えるまでの時間
	m_fBulletDecrease = BULLET_DEFAULTDECREASE;	// 弾の減衰係数
	bRelease = false;

	CObject2D::SetSize(m_fWidth, m_fHeight);
	CObject2D::Init(pos);
	CObject2D::SetTexture(CTexture::TEXTURE_PLAYER0);	//テクスチャの設定
	
	// 体力ゲージの生成
	m_pHpgage = CHpGage::Create({ m_Pos.x - 17.5f, m_Pos.y + 30, m_Pos.z }, 0.0f, HP, HP_HEIGHT, { 0.0f,1.0f,0.0f,1.0f });

	// 経験値ゲージの生成
	CExpgage::Create({ 1280 / 4, 680.0f, 0.0f }, 0, EXPGAGE_WIDTH, 10, D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_pExpgage = CExpgage::Create({ 1280 / 4, 680.0f, 0.0f }, 0, 0, 10, D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f));

	// ステータス強化画面の生成
	m_pStatus = CStatus::Create({ 1280 / 2, 590.0f, 0.0f }, 500, 70);

	for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
	{// ステータスレベルの表示
		m_pStatuslevel[nCnt] = CStatuslevel::Create({ 335.0f + nCnt * 90.0f, 601.0f, 0.0f }, 20.0f, 20.0f);

		//ステータスレベル
		m_nStatuslevel[nCnt] = 1;
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{// 進化先の表示
		m_pEvodisplay[nCnt] = CEvodisplay::Create({ 500.0f + nCnt * 130.0f, 75.0f, 0.0f }, 80.0f, 80.0f);
	}

	// プレイヤーレベルの表示
	m_plevel = CLevelDisplay::Create({ 640, 650.0f, 0.0f }, 20.0f, 20.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	m_plevel->SetAlpha(0.0f);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CPlayer::Uninit()
{
	CObject2D::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CPlayer::Update()
{
	CObject2D::Update();

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{
		m_fRot -= PLAYER_ROT;
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{
		m_fRot += PLAYER_ROT;
	}
#ifdef _DEBUG
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E) == true)
	{
		SetExp(50);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_F) == true)
	{
		SetLife(0, 1);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_G) == true)
	{
		SetLife(1, 1);
	}
	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_C) == true)
	{
		CGame::GetScore()->AddScore(1353);
	}
#endif // _DEBUG

	m_BulletPos = D3DXVECTOR3(m_Pos.x, m_Pos.y, 0.0f);			// 弾が発射される位置

	m_BulletPos.x += sinf(m_fRot) * -PLAYER_BULLETPOS;
	m_BulletPos.y += cosf(m_fRot) * -PLAYER_BULLETPOS;

	// 位置の設定
	CObject2D::Setposition(m_Pos);
	
	D3DXVECTOR3 ZeroPosOld = m_ZeroPos;
	D3DXVECTOR3 PosOld = m_Pos;

	D3DXVECTOR3 StopX = CPlayer::StopX({ 0.0f,0.0f,0.0f });
	D3DXVECTOR3 StopY = CPlayer::StopY({ 0.0f,0.0f,0.0f });

	if (StopX != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_ZeroPos.x = StopX.x;
	}
	if (StopY != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		m_ZeroPos.y = StopY.y;
	}

	m_ZeroPos += m_Move;

	if (m_fRot > D3DX_PI)
	{
		m_fRot -= D3DX_PI * 2;
	}
	else if (m_fRot < -D3DX_PI)
	{
		m_fRot += D3DX_PI * 2;
	}

	// 角度の設定
	CObject2D::Setrot(m_fRot);

	if (m_fLife > 0)
	{
		// 弾を撃つ
		Shot();
	}
	// 体力が0以下の場合
	else
	{
		// プレイヤーが消える
		bRelease = CObject2D::SetDisappear(50.0f, 0.0f, 0.5f, 0.03f);

		if (m_bDeathSound == false)
		{
			//サウンドの再生
				PlaySound(SOUND_LABEL_DEATH_SE);

			m_bDeathSound = true;
		}
	}
	
	if (bRelease == false)
	{
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

			// 体力ゲージの色の設定
			m_pHpgage->SetCol({ 0.0f,1.0f,0.0f, m_fHpAlpha });
		}

		// ステータス表示の色の設定
		m_pStatus->SetCol({ 1.0f, 1.0f, 1.0f, m_fStatusAlpha });

		for (int nCnt = 0; nCnt < PLAYER_NUMSTATUS; nCnt++)
		{// ステータスレベルの表示
			m_pStatuslevel[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fStatusAlpha });
		}

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{// 進化先の表示のアルファ値
			m_pEvodisplay[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fEvoAlpha });
		}
	}
	else
	{
		// 解放
		Uninit();

		// スコアのセーブ
		CGame::GetScore()->SaveScore();

		CFade::SetFade(CApplication::MODE_RESULT, 0.015f);
	}

	// レベルと進化の処理
	CPlayer::LevelUp();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CPlayer::Draw()
{
	CObject2D::Draw();

#ifdef _DEBUG
	m_Font = CRenderer::GetFont();

	//Player(m_Font);
#endif // _DEBUG
}

//==============================================================================================
// 生成処理
//==============================================================================================
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->Init(pPlayer->m_Pos);
	}

	return pPlayer;
}

//==============================================================================================
// レベルアップ処理
//==============================================================================================
void CPlayer::LevelUp()
{
	if (m_bLevelUp == true)
	{
		m_nLevel++;
		m_nCntLevel++;
		m_nLevelUpExp += 3;
		m_plevel->AddLevel(1, PLAYER_MAXLEVEL, { 670.0f, 650.0f, 0.0f }, OBJTYPE_PLAYER);

		if (m_nLevel < 10)
		{
			m_plevel->SetAlpha(0.0f);
		}

		m_bLevelUp = false;
	}

	// ステータスの強化
	if (m_nCntLevel != 0)
	{
		// ステータス表示の色の設定
		m_fStatusAlpha = 1.0f;

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_1))
		{// 体力の最大値UP
			m_nStatuslevel[0]++;

			if (m_nStatuslevel[0] <= MAX_STATUSLEVLE)
			{
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

				m_pStatuslevel[0]->AddStatuslevel(1);		// レベルの表示

				float fOldLife = m_fMaxLife;

				m_fMaxLife += 5;
				m_nCntLevel--;

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
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_2))
		{// 自動回復量UP
			// サウンドの再生
			PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[1]++;

			if (m_nStatuslevel[1] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[1]->AddStatuslevel(1);		// レベルの表示

				m_fHealLife += m_fMaxLife * 0.0001f;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_3))
		{// 本体の攻撃力UP
			//サウンドの再生
			PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[2]++;

			if (m_nStatuslevel[2] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[2]->AddStatuslevel(1);		// レベルの表示

				m_fBodyDamage += 4;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_4))
		{// 弾の速度
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[3]++;

			if (m_nStatuslevel[3] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[3]->AddStatuslevel(1);		// レベルの表示

				m_fBulletSpeed++;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_5))
		{// 弾の体力
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[4]++;

			if (m_nStatuslevel[4] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[4]->AddStatuslevel(1);		// レベルの表示

				m_fBulletLife += 50.0f;
				m_fBulletDecrease -= 0.001f;

				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_6))
		{// 攻撃力
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[5]++;

			if (m_nStatuslevel[5] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[5]->AddStatuslevel(1);		// レベルの表示

				m_fAttack += 2;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_7))
		{// リロード速度
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[6]++;

			if (m_nStatuslevel[6] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[6]->AddStatuslevel(1);		// レベルの表示

				m_fReload -= 2.0f;
				m_nCntLevel--;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_8))
		{// 移動速度UP
				//サウンドの再生
				PlaySound(SOUND_LABEL_POWERUP_SE);

			m_nStatuslevel[7]++;

			if (m_nStatuslevel[7] <= MAX_STATUSLEVLE)
			{
				m_pStatuslevel[7]->AddStatuslevel(1);		// レベルの表示

				m_fSpeed += 0.2f;
				m_nCntLevel--;
			}
		}
	}
	else
	{
		// ステータス表示の色の設定
		m_fStatusAlpha = 0.0f;
	}

	// 進化１回目
	if (m_nLevel >= m_nGrawLevel[0]
		&& m_nGrawCount == 0)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION1);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION2);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION3);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER1);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE1;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER2);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE2;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER3);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE3;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
	}
	// 進化２回目
	else if (m_nLevel >= m_nGrawLevel[1]
		&& m_nGrawCount == 1)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION4);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION5);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION6);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER4);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE4;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER5);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE5;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER6);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE6;
			m_fEvoAlpha = 0.0f;

			m_fSpeed /= 1.5;

			m_nGrawCount++;
		}
	}
	// 進化３回目
	else if (m_nLevel >= m_nGrawLevel[2]
		&& m_nGrawCount == 2)
	{
		m_fEvoAlpha = 1.0f;

		m_pEvodisplay[0]->SetTexture(CTexture::TEXTURE_EVOLUTION7);
		m_pEvodisplay[1]->SetTexture(CTexture::TEXTURE_EVOLUTION8);
		m_pEvodisplay[2]->SetTexture(CTexture::TEXTURE_EVOLUTION9);

		if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F1))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER7);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE7;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F2))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER8);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE8;
			m_fEvoAlpha = 0.0f;

			m_fHealLife += 0.05f;
			m_fBodyDamage += 15;

			m_nGrawCount++;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_F3))
		{
			if (m_PlayerType == PLAYER_TYPE6)
			{
				m_fSpeed *= 1.5;
			}
			//サウンドの再生
			PlaySound(SOUND_LABEL_EVOLUTION_SE);

			CObject2D::SetTexture(CTexture::TEXTURE_PLAYER9);	//テクスチャの設定
			m_PlayerType = PLAYER_TYPE9;
			m_fEvoAlpha = 0.0f;

			m_nGrawCount++;
		}
	}
}

//==============================================================================================
// 移動処理
//==============================================================================================
D3DXVECTOR3 CPlayer::Move()
{
	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_W) == true)
	{
		m_Move.y = m_fSpeed;

		if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * m_fSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * -m_fSpeed;
		}
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_S) == true)
	{
		m_Move.y = -m_fSpeed;

		if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * m_fSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
		{
			m_Move.x = sinf(D3DX_PI / 3) * -m_fSpeed;
		}
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) == true)
	{
		m_Move.x = m_fSpeed;
	}

	if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_D) == true)
	{
		m_Move.x = -m_fSpeed;
	}

	// 移動量の減衰
	m_Move.x += (0.0f - m_Move.x) * PLAYER_DECREASE;
	m_Move.y += (0.0f - m_Move.y) * PLAYER_DECREASE;
	
	if (m_fLife > 0)
	{
		return m_Move;
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// エリア外に出ないようにする設定　X座標用
//==============================================================================================
D3DXVECTOR3 CPlayer::StopX(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos;

	// X座標
	if (m_ZeroPos.x >= PLAYER_STOPPOS)
	{
		Pos.x = pos.x + PLAYER_STOPPOS;

		m_ZeroPos.x = PLAYER_STOPPOS;
		return{ Pos.x ,0.0f,0.0f };
	}
	else if (m_ZeroPos.x <= -PLAYER_STOPPOS)
	{
		Pos.x = pos.x - PLAYER_STOPPOS;

		m_ZeroPos.x = -PLAYER_STOPPOS;
		return{ Pos.x ,0.0f,0.0f };
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// エリア外に出ないようにする設定　Y座標用
//==============================================================================================
D3DXVECTOR3 CPlayer::StopY(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 Pos;

	// Y座標
	if (m_ZeroPos.y >= PLAYER_STOPPOS)
	{
		Pos.y = pos.y + PLAYER_STOPPOS;

		m_ZeroPos.y = PLAYER_STOPPOS;
		return{ 0.0f ,Pos.y,0.0f };
	}
	else if (m_ZeroPos.y <= -PLAYER_STOPPOS)
	{
		Pos.y = pos.y - PLAYER_STOPPOS;

		m_ZeroPos.y = -PLAYER_STOPPOS;
		return{ 0.0f ,Pos.y,0.0f };
	}

	return{ 0.0f,0.0f,0.0f };
}

//==============================================================================================
// 弾を撃つ設定
//==============================================================================================
void CPlayer::Shot()
{
	m_fCoolTime--;

	if (m_fCoolTime <= 0.0f)
	{
		//サウンドの再生
		PlaySound(SOUND_LABEL_SHOT_SE);

		// 弾を撃つ
		if (m_PlayerType == PLAYER_TYPE0)
		{// プレイヤータイプ０の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE1)
		{// プレイヤータイプ１の場合
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI / 2) * 10.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI / 2) * 10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI / 2) * -10.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI / 2) * -10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE2)
		{// プレイヤータイプ２の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.85f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.85f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE3)
		{// プレイヤータイプ３の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed * 1.8f, BULLET_SIZE - 5.0f, m_fAttack * 0.7f, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE4)
		{// プレイヤータイプ４の場合		
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.2f) * 20.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.y - cosf(m_fRot - D3DX_PI * 0.2f) * -20.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE5)
		{// プレイヤータイプ５の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI * 0.6f) * 30.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.6f) * -30.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE6)
		{// プレイヤータイプ６の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE + 7.0f, m_fAttack * 1.5f, m_fBulletDecrease);

			m_fCoolTime = m_fReload + 50.0f;
		}
		else if (m_PlayerType == PLAYER_TYPE7)
		{// プレイヤータイプ７の場合
			CBullet::Create(m_BulletPos, m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI) * 50.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI) * 50.0f, m_BulletPos.z), m_fRot + D3DX_PI, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x - sinf(m_fRot + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.y - cosf(m_fRot + D3DX_PI * 0.6f) * 25.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.6f) * -25.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.5f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.7f) * -50.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.5f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.75f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 60.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.75f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
		}
		else if (m_PlayerType == PLAYER_TYPE8)
		{// プレイヤータイプ８の場合
		}
		else if (m_PlayerType == PLAYER_TYPE9)
		{// プレイヤータイプ９の場合
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot) * -10.0f, m_BulletPos.y + cosf(m_fRot) * -10.0f, m_BulletPos.z), m_fRot, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed + 1.0f, BULLET_SIZE, m_fAttack, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.05f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.7f) * 10.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.05f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.1f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.4f) * 20.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.1f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.0f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.15f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 30.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.15f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 1.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 35.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.2f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 2.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fRot + D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fRot - D3DX_PI * 0.25f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
			CBullet::Create(D3DXVECTOR3(m_BulletPos.x + sinf(m_fRot - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.y + cosf(m_fRot - D3DX_PI * 0.1f) * 40.0f, m_BulletPos.z), m_fRot + D3DX_PI * 0.25f, CBullet::BULLET_PLAYER, m_fBulletLife, m_fBulletSpeed - 3.5f, BULLET_SIZE - 5.0f, m_fAttack * 0.5f, m_fBulletDecrease);
		
			m_fCoolTime = m_fReload + 20.0f;
		}

		if (m_PlayerType != PLAYER_TYPE6
			&& m_PlayerType != PLAYER_TYPE9)
		{
			m_fCoolTime = m_fReload;
		}
	}
}

//==============================================================================================
// 体力ゲージが減る処理
//==============================================================================================
void CPlayer::HitDamage(float damage)
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
void CPlayer::HealHp(float heal)
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
void CPlayer::SetLife(int type, float life)
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

//==============================================================================================
// 経験値の設定
//==============================================================================================
void CPlayer::SetExp(int exp)
{
	m_nExp += exp;

	// 経験値の上昇量の割合の計算
	float fExpPercent = ((float)exp / m_nLevelUpExp) * 100;

	// 経験値ゲージを増やす
	m_bLevelUp = m_pExpgage->AddExpgage(fExpPercent);
}

void CPlayer::Player(LPD3DXFONT pFont)
{
	RECT rect;
	TCHAR str[256];

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		rect = { 0, 0 + nCnt * 20, 1280, 720 };

		if (nCnt == 0)
		{
			sprintf(str, ("Eキー : 経験値の取得"));
		}
		else if (nCnt == 1)
		{
			sprintf(str, ("Fキー : 自傷"));
		}
		else if (nCnt == 2)
		{
			sprintf(str, ("Cキー : スコアの加算"));
		}
		// テキスト描画
		pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0, 0));
	}
}