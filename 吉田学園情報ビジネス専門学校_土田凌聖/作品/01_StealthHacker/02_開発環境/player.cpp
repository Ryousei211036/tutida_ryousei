//==============================================================================================
//
// プレイヤー　　　player.cpp
// 土田凌聖
//
//==============================================================================================
#include"player.h"
#include"application.h"
#include"inputkeyboard.h"
#include"inputJoypad.h"
#include"camera.h"
#include"debugproc.h"
#include"model.h"
#include"game.h"
#include"file.h"
#include"sound.h"
#include"stencilpolygon.h"
#include"objectX.h"
#include"bullet.h"
#include"fade.h"
#include"score.h"
#include"enemy.h"
#include"tutorial.h"
#include"tutorial_text.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CPlayer::CPlayer()
{
	m_Pos = { 0.0f,0.0f,0.0f };
	m_Rot = { 0.0f,0.0f,0.0f };
	m_RotDest = { 0.0f,0.0f,0.0f };
	m_SizeMax = { -1000.0f,-1000.0f,-1000.0f };
	m_SizeMin = { 1000.0f, 1000.0f, 1000.0f };
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
HRESULT CPlayer::Init()
{	
	CCharManager::Init();

	m_fRadius = 30.0f;		// 当たり判定の半径
	m_fCameraDis = 80.0f;	// カメラとの距離
	m_nLife = 90;			// 体力
	m_Motion = CCharManager::MOTION_NEUTRAL;	// モーション
	m_bFind = false;
	m_bPlay_Shot = false;
	m_bPlay_StealthKill = false;
	m_bDeath = false;
	m_bPushButton = false;

	m_Pos = CCharManager::GetPos();
	m_InitPos = m_Pos;

	CCharManager::SetRadius(m_fRadius);
	CCharManager::SetLife(m_nLife);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CPlayer::Uninit()
{
	CCharManager::Uninit();

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CPlayer::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCamera *pCamera = nullptr;

		// オブジェクトXの情報
		CObjectX **pObjectX = nullptr;

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			pCamera = CGame::GetCamera();
			pObjectX = CGame::GetObjectX();
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			pCamera = CTutorial::GetCamera();
			pObjectX = CTutorial::GetObjectX();
		}

		if (CApplication::GetMode() == CApplication::MODE_GAME
			|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// 操作のタイプ
			Control_Type ContType = GetControlType();

			// 体力の取得
			m_nLife = GetLife();

			// ステルスキル中か
			m_bPlay_StealthKill = GetPlayStealthKill();

			bool bGoal = pCamera->GoalOpen();

			if (m_bPlay_StealthKill)
			{
				m_Motion = CCharManager::MOTION_KILL;
			}

			// プレイヤー操作中
			if (ContType == CONTROL_PLAYER && !m_bDeath)
			{
				if (!m_bPlay_StealthKill && !bGoal)
				{
					// アクション
					Action();
				}

				// 移動処理
				Move();

				// 足音を鳴らす
				Sound_Footstep();
			}

			// ボタンを押したか
			m_bPushButton = GetPushButton();

			if (m_bPushButton)
			{
				// ボタンを押す
				m_Motion = MOTION_BUTTON;
			}

			// 死亡
			Death();
		}

		// タイプの設定
		SetMotion(m_Motion);

		// 親の更新処理
		CCharManager::Update();

		if (CApplication::GetMode() == CApplication::MODE_GAME
			|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// 回転速度
			m_fRotSpeed = 0.1f;

			// モーション中の再生中の処理
			Process_Motion();

			// オブジェクトの数
			int nNumObject = (*pObjectX)->GetNumObject();

			for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
			{
				if (*pObjectX != nullptr)
				{
					// オブジェクトの当たり判定
					(*pObjectX)->CharCollision(CModel::MODEL_TYPE_PLAYER);
				}
			}
		}
	}
	else
	{
		CApplication::GetSound()->StopSound(CSound::SOUND_LABEL_WARK);

		m_bFootstep = false;
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CPlayer::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->CCharManager::SetPos(pos);
	}

	return pPlayer;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CPlayer::Move()
{
	CCamera *pCamera = nullptr;
	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();

		// カメラの角度
		CRot = pCamera->GetRot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();

		// カメラの角度
		CRot = pCamera->GetRot();
	}

	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// 位置の取得
	m_Pos = CCharManager::GetPos();

	// 前回の位置の設定
	m_PosOld = m_Pos;

	// 目的の角度の設定
	m_RotDest = CCharManager::GetRotDest();

	// カメラがゴールのほうを向いているか
	bool bGoal = pCamera->GoalOpen();

	// ステルスキルモーション中じゃない時
	if (!m_bPlay_StealthKill && !bGoal)
	{
		// プレイヤーの移動
		if (pKeyboard->GetKeyboardPress(DIK_W) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y <= -0.3f)
		{
			if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
			{// 左前
				m_RotDest.y = CRot.y - D3DX_PI * 1 / 4;
				m_Pos.x -= sinf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
			{// 右前
				m_RotDest.y = CRot.y + D3DX_PI * 1 / 4;
				m_Pos.x += sinf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
				m_Pos.z += cosf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
			}
			else
			{// 前
				m_RotDest.y = CRot.y;
				m_Pos.x += sinf(CRot.y) * m_fSpeed;
				m_Pos.z += cosf(CRot.y) * m_fSpeed;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_S) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y >= 0.3f)
		{
			if (pKeyboard->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
			{// 左下
				m_RotDest.y = CRot.y - D3DX_PI * 3 / 4;
				m_Pos.x -= sinf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y + D3DX_PI * 1 / 4) * m_fSpeed;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
			{// 右下
				m_RotDest.y = CRot.y + D3DX_PI * 3 / 4;
				m_Pos.x += sinf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
				m_Pos.z += cosf(CRot.y + D3DX_PI * 3 / 4) * m_fSpeed;
			}
			else
			{// 下
				m_RotDest.y = CRot.y + D3DX_PI;
				m_Pos.x -= sinf(CRot.y) * m_fSpeed;
				m_Pos.z -= cosf(CRot.y) * m_fSpeed;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_A) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f)
		{// 左
			m_RotDest.y = CRot.y - D3DX_PI * 1 / 2;
			m_Pos.x -= sinf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
			m_Pos.z -= cosf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_D) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f)
		{// 右
			m_RotDest.y = CRot.y + D3DX_PI * 1 / 2;
			m_Pos.x += sinf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
			m_Pos.z += cosf(CRot.y + D3DX_PI * 1 / 2) * m_fSpeed;
		}

		if (pKeyboard->GetKeyboardPress(DIK_W)
			|| pKeyboard->GetKeyboardPress(DIK_A)
			|| pKeyboard->GetKeyboardPress(DIK_S)
			|| pKeyboard->GetKeyboardPress(DIK_D)
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x >= 0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).x <= -0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y >= 0.3f
			|| pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_LEFT_STICK).y <= -0.3f)
		{
			// 移動速度(歩き)
			m_fSpeed = 3;

			// 歩き
			m_Motion = CCharManager::MOTION_WALK;

			// 銃をおろす
			m_bHoldGun = false;

			// ステンシルバッファ用のポリゴンを透明にする
			m_bFind = false;

			// ステンシルポリゴンの情報
			CStencilPolygon *pStencil = nullptr;

			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				pStencil = CGame::GetStencil();
			}
			else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
			{
				pStencil = CTutorial::GetStencil();
			}

			if (pStencil != nullptr)
			{
				pStencil->SetUse(false);
			}

			// ボタンを押さない
			SetPushButton(false);
		}
	}

	// 角度の正規化
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	// 親に情報を渡す
	CCharManager::SetPos(m_Pos);		// 位置
	CCharManager::SetPosOld(m_PosOld);	// 前回の位置
}

//==============================================================================================
// プレイヤーのアクション
//==============================================================================================
void CPlayer::Action()
{
	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// ステンシルポリゴンの情報
	CStencilPolygon *pStencil = nullptr;

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pStencil = CGame::GetStencil();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pStencil = CTutorial::GetStencil();
	}

	// 銃を構える
	if ((pKeyboard->GetKeyboardTrigger(DIK_UP) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_LEFT_BUTTON))
		&& !m_bHoldGun && !m_bPushButton)
	{
		m_bHoldGun = true;
	}
	//// 銃をおろす
	//else if ((pKeyboard->GetKeyboardTrigger(DIK_UP) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_LEFT_BUTTON)) && m_bHoldGun)
	//{
	//	m_bHoldGun = false;
	//}

	// 銃を撃つ
	if ((pKeyboard->GetKeyboardTrigger(DIK_SPACE) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_RIGHT_BUTTON)) && m_bHoldGun)
	{
		m_Motion = CCharManager::MOTION_SHOT;

		if (!m_bPlay_Shot)
		{
			D3DXVECTOR3 m_GunPos = m_Pos;
			m_GunPos.y = 80.0f;

			m_GunPos.x += sinf(m_Rot.y) * 50;
			m_GunPos.z += cosf(m_Rot.y) * 50;

			m_GunPos.x += sinf(m_Rot.y + D3DX_PI / 2) * 10.0f;
			m_GunPos.z += cosf(m_Rot.y + D3DX_PI / 2) * 10.0f;

			// 弾を撃つ
			CBullet::Create(m_GunPos, m_Rot, { 0.2f,0.2f,0.2f }, "Data/XFILE/Player/01_head.x", CObjectX::OBJECTX_BULLET, CModel::MODEL_TYPE_PLAYER);
		}
	}

	// 索敵する
	if ((pKeyboard->GetKeyboardTrigger(DIK_Q) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_Y))
		&& !m_bFind && pStencil != nullptr && !m_bPushButton && !m_bHoldGun)
	{
		m_bFind = true;				// 索敵中か
		m_Motion = CCharManager::MOTION_FIND;
		pStencil->SetUse(true);		// ステンシルポリゴンの使用
	}
	else if ((pKeyboard->GetKeyboardTrigger(DIK_Q) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_Y))
		&& m_bFind && pStencil != nullptr)
	{
		m_bFind = false;			// 索敵中か
		pStencil->SetUse(false);	// ステンシルポリゴンの使用
	}

	if (m_Motion == MOTION_NEUTRAL || m_Motion == MOTION_BUTTON)
	{
		// ロボット操作に切り替える
		ChangeControl(CONTROL_ROBOT);
	}
}

//==============================================================================================
// 死亡
//==============================================================================================
void CPlayer::Death()
{
	// 体力が0になった
	if (m_nLife <= 0)
	{
		m_Motion = CCharManager::MOTION_DEATH;
		m_bDeath = true;
		m_bFind = false;
		m_bHoldGun = false;

		// モーション変更
		ChangeMotion(m_Motion);

		if (CApplication::GetMode() == CApplication::MODE_GAME)
		{
			// モーションが終わったら
			if (!PlayMotion(CCharManager::MOTION_DEATH))
			{
				CFade::SetFade(CApplication::MODE_RESULT, 0.01f);
			}
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		{
			// モーションが終わったら
			if (!PlayMotion(CCharManager::MOTION_DEATH))
			{
				CCharManager::SetPos(m_InitPos);
				SetPosOld(m_InitPos);

				m_RotDest = { 0.0f,0.0f,0.0f };
				SetRotDest(m_RotDest);

				m_nLife = 120;
				SetLife(m_nLife);

				m_bDeath = false;
				m_Motion = CCharManager::MOTION_NEUTRAL;

				// モーション変更
				ChangeMotion(m_Motion);
			}
		}
	}
}

//==============================================================================================
// モーション中の処理
//==============================================================================================
void CPlayer::Process_Motion()
{
	// カメラの情報
	CCamera *pCamera = nullptr;
	D3DXVECTOR3 CPosV = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();
		CPosV = pCamera->GetPosV();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();
		CPosV = pCamera->GetPosV();
	}

	if (CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		if (m_CPosV.z == 0.0f)
		{
			m_CPosV.z = CPosV.z;
		}

		// 射撃モーションが再生中か
		m_bPlay_Shot = PlayMotion(CCharManager::MOTION_SHOT);

		// ニュートラル状態にする
		if (!m_bHoldGun
			&& !m_bPlay_StealthKill
			&& !m_bFind)
		{// 銃を構えてないとき、索敵していないとき
			m_Motion = CCharManager::MOTION_NEUTRAL;	// タイプの設定

			// カメラを遠ざける
			CPosV.z += ((m_CPosV.z) - CPosV.z) * 0.1f;
			pCamera->SetPosV(CPosV);
		}

		if (m_bHoldGun && !m_bPlay_Shot)
		{// 銃を構えてるとき
			m_Motion = CCharManager::MOTION_GUN;		// タイプの設定
			m_RotDest = pCamera->GetRot();	// 正面を向く
			m_fRotSpeed = 0.5f;				// 回転速度

			// カメラを近づける
			CPosV.z += ((m_CPosV.z + m_fCameraDis) - CPosV.z) * 0.5f;
			pCamera->SetPosV(CPosV);
		}
		else if (m_bPlay_Shot)
		{// 射撃モーション中
			m_RotDest = pCamera->GetRot();	// 正面を向く
			m_fRotSpeed = 0.5f;				// 回転速度
		}

		CCharManager::SetRotDest(m_RotDest);// 目的の角度
	}
}

//==============================================================================================
// モーションの値の設定
//==============================================================================================
void CPlayer::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// モーションの設定
//==============================================================================================
void CPlayer::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// モーションの切り替え
//==============================================================================================
void CPlayer::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel *CPlayer::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
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

//==============================================================================================
// 足音を鳴らす
//==============================================================================================
void CPlayer::Sound_Footstep()
{
	CSound *pSound = CApplication::GetSound();

	if (m_Motion == CCharManager::MOTION_WALK && !m_bFootstep)
	{// 歩き状態
		pSound->PlaySound(CSound::SOUND_LABEL_WARK);
		m_bFootstep = true;
	}
	else if(m_Motion != CCharManager::MOTION_WALK && m_bFootstep)
	{// 足音を消す
		pSound->StopSound(CSound::SOUND_LABEL_WARK);

		m_bFootstep = false;
	}
}