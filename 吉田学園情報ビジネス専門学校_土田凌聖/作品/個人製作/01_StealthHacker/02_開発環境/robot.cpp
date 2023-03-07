//==============================================================================================
//
// ロボット　　　robot.cpp
// 土田凌聖
//
//==============================================================================================
#include"robot.h"
#include"game.h"
#include"camera.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"
#include"objectX.h"
#include"gage.h"
#include"tutorial.h"
#include"tutorial_text.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CRobot::CRobot()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CRobot::~CRobot()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CRobot::Init()
{
	CCharManager::Init();
	
	m_fSpeed = 2;				// 移動速度
	m_fRotSpeed = 0.1f;			// 回転速度
	m_nConChange_CoolTime = 60;	// 操作変更のクールタイム
	m_fRadius = 30.0f;			// 当たり判定の半径
	m_nLife = 900;				// 体力
	m_bDeath = false;
	m_bGageFull_Up = false;
	m_bResetGage = false;

	// 親クラスに渡す
	CCharManager::SetRadius(m_fRadius);		// 半径
	CCharManager::SetLife(m_nLife);			// 体力

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CRobot::Uninit()
{
	CCharManager::Uninit();
	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CRobot::Update()
{
	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if (!Tutorial_Display)
	{
		CCharManager::Update();

		// 体力の取得
		m_nLife = GetLife();

		// タイプの設定
		SetMotion(m_Motion);

		m_Motion = CCharManager::MOTION_NEUTRAL;	// タイプの設定

		Control_Type ContType = CCharManager::GetControlType();

		// ロボット操作中
		if (ContType == CONTROL_ROBOT && !m_bDeath)
		{
			// 移動処理
			Move();

			// アクション
			Action();
		}

		// ボタンを押したか
		m_bPushButton = GetPushButton();

		if (m_bPushButton)
		{
			// ボタンを押す
			m_Motion = MOTION_BUTTON;
		}

		Death();

		if (m_bGageFull_Up)
		{
			m_pGage->SetAlpha(0.1f);

			m_nLife = 60;
			SetLife(m_nLife);
			m_bResetGage = false;
			m_bDeath = false;
		}

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

		int nNumObject = (*pObjectX)->GetNumObject();

		for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
		{
			if (*pObjectX != nullptr)
			{
				// 当たり判定
				(*pObjectX)->CharCollision(CModel::MODEL_TYPE_ROBOT);
			}
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CRobot::Draw()
{
	CCharManager::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CRobot *CRobot::Create(D3DXVECTOR3 pos)
{
	CRobot *pRobot = nullptr;

	pRobot = new CRobot;

	if (pRobot != nullptr)
	{
		pRobot->CCharManager::SetPos(pos);
	}

	return pRobot;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CRobot::Move()
{
	// カメラの情報
	CCamera *pCamera = nullptr;
	// カメラの角度
	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pCamera = CGame::GetCamera();
		CRot = pCamera->GetRot();
	}
	else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pCamera = CTutorial::GetCamera();
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

	if (!bGoal)
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
			// 歩き
			m_Motion = CCharManager::MOTION_WALK;

			// ボタンを押さない
			SetPushButton(false);
		}
	}

	// 角度の正規化
	m_Rot.y = CCharManager::NormalizeRot(m_fRotSpeed);

	CCharManager::SetPos(m_Pos);
	CCharManager::SetPosOld(m_PosOld);
	CCharManager::SetRotDest(m_RotDest);
}

//==============================================================================================
// アクション
//==============================================================================================
void CRobot::Action()
{
	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// プレイヤー操作に切り替える
	ChangeControl(CONTROL_PLAYER);
}

//==============================================================================================
// 死亡
//==============================================================================================
void CRobot::Death()
{
	if (m_nLife <= 0)
	{
		m_Motion = CCharManager::MOTION_DEATH;
		m_bDeath = true;
		m_bGageFull_Up = false;

		if (!m_bResetGage && m_pGage != nullptr)
		{
			float fLength = m_pGage->GetEnd_Lenght();
			m_pGage->SubGage(fLength * 3);
			m_pGage->SetResetAlpha(1.0f);
			m_pGage->SetResetCount(30);
			m_bResetGage = true;
		}

		// キーボードの情報
		CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

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

		D3DXVECTOR3 PPos = { 1230.0f, 0.0f, 855.0f };
		float fPRadius = 0.0f;

		if (pPlayer != nullptr)
		{
			PPos = pPlayer->GetPos();
			fPRadius = pPlayer->GetRadius();
		}

		// プレイヤーとロボットの距離
		float Distance = CObject::Distance(m_Pos, PPos);

		if (m_pGage != nullptr)
		{
			if (m_fRadius + fPRadius > Distance)
			{
				// 距離が近いときはゲージを表示する
				m_pGage->Display(true);

				if (pKeyboard->GetKeyboardPress(DIK_E))
				{
					// ゲージを増やす
					m_bGageFull_Up = m_pGage->AddGage(1.1f);
				}
			}
			else
			{
				m_pGage->Display(false);
			}
		}

		// モーション変更
		ChangeMotion(m_Motion);

		// モーションが終わったら
		if (!PlayMotion(CCharManager::MOTION_DEATH))
		{
			// 操作を切り替える
			CCharManager::SetControl(CONTROL_PLAYER);

			// ロボットの場所を移す
			m_Pos = { 1220.0f,0.0f,840.0f };

			SetPos(m_Pos);
			SetPosOld(m_Pos);

			// ゲージの生成
			if (m_pGage == nullptr)
			{
				m_pGage = CGage::Create({ m_Pos.x ,m_Pos.y + 60.0f,m_Pos.z }, { 35.0f, 3.0f,0.0f }, { 0.0f,1.0f,0.0f,1.0f }, true);
			}

			m_pGage->SetPos({ m_Pos.x,m_Pos.y + 60.0f,m_Pos.z });
		}
		else
		{
			if (m_pGage != nullptr)
			{
				m_pGage->Display(false);
			}
		}
	}
}

//==============================================================================================
// モーション中の処理
//==============================================================================================
void CRobot::Process_Motion()
{

}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CRobot::SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	CCharManager::SetMotionData(maxkey, key, parts, motion, frame, pos, rot, loop);
}

//==============================================================================================
// モーションの設定
//==============================================================================================
void CRobot::Motion()
{
	CCharManager::Motion();
}

//==============================================================================================
// モーションの変更
//==============================================================================================
void CRobot::ChangeMotion(int nummotion)
{
	CCharManager::ChangeMotion(nummotion);
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel *CRobot::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot,
	char *Xfilename, CModel::EModel_Type type)
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