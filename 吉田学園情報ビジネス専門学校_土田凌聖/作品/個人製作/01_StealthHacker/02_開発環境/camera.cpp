//==============================================
//
//カメラの処理[camera.cpp]
//Author：土田凌聖
//
//==============================================
#include"camera.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"application.h"
#include"player.h"
#include"game.h"
#include"setobject.h"
#include"objectX.h"
#include"title.h"
#include"tutorial.h"
#include"tutorial_text.h"

#include"debugproc.h"

//==============================================
//コンストラクタ
//==============================================
CCamera::CCamera()
{

}

//==============================================
//デストラクタ
//==============================================
CCamera::~CCamera()
{

}

//==============================================
//カメラの初期化処理
//==============================================
void CCamera::Init(void)
{
	//視点・注視点・上方向を設定する
	m_posV = D3DXVECTOR3(40.0f, 100.0f, -200.0f);		// 視点(オフセット)
	m_InitposV = D3DXVECTOR3(40.0f, 100.0f, -200.0f);	// 視点(オフセット)
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);			// 注視点(オフセット)
	m_InitposR = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);		// 注視点(オフセット)
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	m_fRotSpeed = 0.03f;							// 回転速度
	m_nShiftGoalCount = 250;
	m_nGoalCount = 100;
	m_fPlayerDistance = 0.0f;

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
		m_CPos = pPlayer->GetPos();			// カメラの初期位置(プレイヤーの位置)
	}

	if (CApplication::GetMode() == CApplication::MODE_TITLE)
	{
		m_CPos = { -30.0f,-20.0f,-100.0f };
		m_rot.y = D3DX_PI;
	}
}

//==============================================
//カメラの終了処理
//==============================================
void CCamera::Uninit(void)
{

}

//==============================================
//カメラの更新処理
//==============================================
void CCamera::Update(void)
{
	// 行列を使ったカメラ制御
	Matrix();

	bool Tutorial_Display = CTutorial_Text::GetDisplay();

	if ((CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
		&& !Tutorial_Display)
	{
		// カメラの移動
		Move();

		// 操作のタイプ
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (type != CCharManager::CONTROL_CAMERA
			&& !m_bGoalOpen)
		{
			// カメラの当たり判定
			Collision();
		}

		// ゴールが開いた時のカメラ
		GoalCamera();
	}
}

//==============================================
// カメラのセット処理
//==============================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの生成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_WorldCameraPosV,
		&m_WorldCameraPosR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの生成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)1280 / (float)720,
		10.0f,
		10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==============================================
// 行列を使ったカメラ制御
//==============================================
void CCamera::Matrix()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	// ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

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

	if (CApplication::GetMode() == CApplication::MODE_GAME
		|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		// 操作のタイプ
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (!m_bGoalOpen)
		{
			if (pPlayer != nullptr)
			{
				m_PPos = pPlayer->GetPos();	// プレイヤーの位置
			}
			if (pRobot != nullptr)
			{
				m_RPos = pRobot->GetPos();	// ロボットの位置
			}

			if (type == CCharManager::CONTROL_PLAYER)
			{
				// プレイヤーの位置に移動
				m_CPos += (m_PPos - m_CPos) * 0.1f;
			}
			else if (type == CCharManager::CONTROL_ROBOT)
			{
				// ロボットの位置に移動
				m_CPos += (m_RPos - m_CPos) * 0.1f;
				m_CPos.y += (-80.0f - m_CPos.y) * 0.1f;
			}
		}
	}

	// 行列を回転に反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 行列に移動を反映(プレイヤーの位置)
	D3DXMatrixTranslation(&mtxTrans, m_CPos.x, m_CPos.y, m_CPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&m_WorldCameraPosV, &m_posV, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_WorldCameraPosR, &m_posR, &m_mtxWorld);

	// 視点からプレイヤーまでの距離
	if (m_fPlayerDistance == 0.0f)
	{
		m_fPlayerDistance = sqrtf((m_CPos.x - m_WorldCameraPosV.x) * (m_CPos.x - m_WorldCameraPosV.x)
			+ (m_CPos.z - m_WorldCameraPosV.z) * (m_CPos.z - m_WorldCameraPosV.z));
	}
}

//==============================================
// カメラの移動
//==============================================
void CCamera::Move()
{
	// 操作のタイプ
	CCharManager::Control_Type type = CCharManager::GetControlType();

	// キーボードの情報
	CInputkeyboard *pKeyboard = CApplication::GetInputKeyboard();

	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	if (!m_bGoalOpen)
	{
		// カメラの移動
		if (pKeyboard->GetKeyboardPress(DIK_RIGHT) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_RIGHT_STICK).x >= 0.3f)
		{
			m_rot.y += m_fRotSpeed;
		}
		if (pKeyboard->GetKeyboardPress(DIK_LEFT) || pJoypad->GetJoypadStick(CInputJoypad::JOYKEY_RIGHT_STICK).x <= -0.3f)
		{
			m_rot.y -= m_fRotSpeed;
		}
	}

#ifdef _DEBUG
	if (type == CCharManager::CONTROL_CAMERA)
	{
		if (pKeyboard->GetKeyboardPress(DIK_W))
		{
			if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_A))
			{// 左前
				m_CPos.x -= sinf(m_rot.y + D3DX_PI * 3 / 4) * 3;
				m_CPos.z -= cosf(m_rot.y + D3DX_PI * 3 / 4) * 3;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D))
			{// 右前
				m_CPos.x += sinf(m_rot.y + D3DX_PI * 1 / 4) * 3;
				m_CPos.z += cosf(m_rot.y + D3DX_PI * 1 / 4) * 3;
			}
			else
			{// 前
				m_CPos.x += sinf(m_rot.y) * 3;
				m_CPos.z += cosf(m_rot.y) * 3;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_S))
		{
			if (pKeyboard->GetKeyboardPress(DIK_A))
			{// 左下
				m_CPos.x -= sinf(m_rot.y + D3DX_PI * 1 / 4) * 3;
				m_CPos.z -= cosf(m_rot.y + D3DX_PI * 1 / 4) * 3;
			}
			else if (pKeyboard->GetKeyboardPress(DIK_D))
			{// 右下
				m_CPos.x += sinf(m_rot.y + D3DX_PI * 3 / 4) * 3;
				m_CPos.z += cosf(m_rot.y + D3DX_PI * 3 / 4) * 3;
			}
			else
			{// 下
				m_CPos.x -= sinf(m_rot.y) * 3;
				m_CPos.z -= cosf(m_rot.y) * 3;
			}
		}
		else if (pKeyboard->GetKeyboardPress(DIK_A))
		{// 左
			m_CPos.x -= sinf(m_rot.y + D3DX_PI * 1 / 2) * 3;
			m_CPos.z -= cosf(m_rot.y + D3DX_PI * 1 / 2) * 3;
		}
		else if (pKeyboard->GetKeyboardPress(DIK_D))
		{// 右
			m_CPos.x += sinf(m_rot.y + D3DX_PI * 1 / 2) * 3;
			m_CPos.z += cosf(m_rot.y + D3DX_PI * 1 / 2) * 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_E))
		{
			m_CPos.y += 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_Q))
		{
			m_CPos.y -= 3;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_UP))
		{
			m_rot.x -= m_fRotSpeed;
		}
		else if (CApplication::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN))
		{
			m_rot.x += m_fRotSpeed;
		}

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
			pPlayer->ChangeControl(CCharManager::CONTROL_PLAYER);
		}
	}
#endif // _DEBUG

	// 角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================
// カメラの当たり判定
//==============================================
void CCamera::Collision()
{
	// 操作のタイプ
	CCharManager::Control_Type type = CCharManager::GetControlType();

	CCharManager *pCharacter = nullptr;
	D3DXVECTOR3 CPos = { 0.0f,0.0f,0.0f };

	// オブジェクトXの情報
	CObjectX **pObjectX = nullptr;

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

		// オブジェクトX
		pObjectX = CGame::GetObjectX();
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

		// オブジェクトX
		pObjectX = CTutorial::GetObjectX();
	}

	if (pCharacter != nullptr)
	{
		// キャラクターの位置
		CPos = pCharacter->GetPos();
	}

	// プレイヤーから視点のベクトル
	D3DXVECTOR3 CameraVec = m_WorldCameraPosV - CPos;
	CameraVec.y = 0.0f;

	// 正規化
	D3DXVec3Normalize(&CameraVec, &CameraVec);

	// オブジェクトの数
	int nNumObject = (*pObjectX)->GetNumObject();

	D3DXVECTOR3 ObjPos = { 0.0f,0.0f,0.0f };	// 位置
	D3DXVECTOR3 ObjSize = { 0.0f,0.0f,0.0f };	// サイズ
	D3DXVECTOR3 ObjMin = { 0.0f,0.0f,0.0f };	// 最小値
	D3DXVECTOR3 ObjMax = { 0.0f,0.0f,0.0f };	// 最大値

	for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
	{
		if (*pObjectX != nullptr)
		{
			CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

			if (Type != CObjectX::OBJECTX_KEY_RED
				&& Type != CObjectX::OBJECTX_KEY_BLUE
				&& Type != CObjectX::OBJECTX_KEY_GREEN
				&& Type != CObjectX::OBJECTX_CEILING)
			{
				ObjPos = (*pObjectX)->GetPos();			// 位置
				ObjSize = (*pObjectX)->GetSize();		// サイズ
				ObjMin = (*pObjectX)->GetMin();			// 頂点の最小値
				ObjMax = (*pObjectX)->GetMax();			// 頂点の最大値

				// 最小値にサイズをかける
				ObjMin.x *= ObjSize.x;
				ObjMin.z *= ObjSize.z;

				// 最大値にサイズをかける
				ObjMax.x *= ObjSize.x;
				ObjMax.z *= ObjSize.z;

				// オブジェクトに最大、最少を足す
				D3DXVECTOR3 Min = ObjPos + ObjMin;
				D3DXVECTOR3 Max = ObjPos + ObjMax;

				// X座標のニアとファー
				float fNearX = 0.0f;
				float fFarX = 0.0f;

				// Z座標のニアとファー
				float fNearZ = 0.0f;
				float fFarZ = 0.0f;

				// オブジェクトより左にいる
				if (Min.x > CPos.x)
				{
					fNearX = (Min.x - CPos.x) / CameraVec.x;
					fFarX = (Max.x - CPos.x) / CameraVec.x;
				}
				// オブジェクトより右にいる
				else if (Max.x < CPos.x)
				{
					fNearX = (Max.x - CPos.x) / CameraVec.x;
					fFarX = (Min.x - CPos.x) / CameraVec.x;
				}

				// オブジェクトより手前にいる
				if (Min.z > CPos.z)
				{
					fNearZ = (Min.z - CPos.z) / CameraVec.z;
					fFarZ = (Max.z - CPos.z) / CameraVec.z;
				}
				// オブジェクトより奥にいる
				else if (Max.z < CPos.z)
				{
					fNearZ = (Max.z - CPos.z) / CameraVec.z;
					fFarZ = (Min.z - CPos.z) / CameraVec.z;
				}

				float in = 0.0f;	// ベクトルの入り側
				float out = 0.0f;	// ベクトルの出側

				if (Max.x > CPos.x && Min.x < CPos.x)
				{
					if (CameraVec.x == 0.0f)
					{
						fNearX = CameraVec.x;
						fFarX = 10000;
					}
					else if (CameraVec.x > 0.0f)
					{
						fFarX = (Max.x - CPos.x) / CameraVec.x;

						if (Min.z > CPos.z)
						{
							fNearZ = (Min.z - CPos.z) / CameraVec.z;
						}
						else if (Max.z < CPos.z)
						{
							fNearZ = (Max.z - CPos.z) / CameraVec.z;
						}
					}
					else if (CameraVec.x < 0.0f)
					{
						fFarX = (Min.x - CPos.x) / CameraVec.x;

						if (Min.z > CPos.z)
						{
							fNearZ = (Min.z - CPos.z) / CameraVec.z;
						}
						else if (Max.z < CPos.z)
						{
							fNearZ = (Max.z - CPos.z) / CameraVec.z;
						}
					}
				}
				if (Max.z > CPos.z && Min.z < CPos.z)
				{
					if (CameraVec.z == 0.0f)
					{
						fNearZ = CameraVec.z;
						fFarZ = 10000;
					}
					else if (CameraVec.z > 0.0f)
					{
						fFarZ = (Max.z - CPos.z) / CameraVec.z;

						if (Min.x > CPos.x)
						{
							fNearX = (Min.x - CPos.x) / CameraVec.x;
						}
						else if (Max.x < CPos.x)
						{
							fNearX = (Max.x - CPos.x) / CameraVec.x;
						}
					}
					else if (CameraVec.z < 0.0f)
					{
						fFarZ = (Min.z - CPos.z) / CameraVec.z;

						if (Min.x > CPos.x)
						{
							fNearX = (Min.x - CPos.x) / CameraVec.x;
						}
						else if (Max.x < CPos.x)
						{
							fNearX = (Max.x - CPos.x) / CameraVec.x;
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

				float CameraDistance = sqrtf((m_WorldCameraPosV.x - CPos.x) * (m_WorldCameraPosV.x - CPos.x)
					+ (m_WorldCameraPosV.z - CPos.z) * (m_WorldCameraPosV.z - CPos.z));

				// hitがプラスの場合は当たってる
				if (hit > 0 && in - 35 < m_fPlayerDistance)
				{
					if (CPos.z > Max.z)
					{
						m_WorldCameraPosV.z = Max.z + 30;
					}
					else if (CPos.z < Min.z)
					{
						m_WorldCameraPosV.z = Min.z - 30;
					}

					if (CPos.x > Max.x)
					{
						m_WorldCameraPosV.x = Max.x + 30;
					}
					else if (CPos.x < Min.x)
					{
						m_WorldCameraPosV.x = Min.x - 30;
					}
				}
			}
		}
	}
}

//==============================================
// ゴールを映す
//==============================================
void CCamera::GoalCamera()
{
	if (m_nShiftGoalCount > 0)
	{
		m_bGoalOpen = CObjectX::GetGoalOpen();

		if (m_bGoalOpen)
		{
			m_nShiftGoalCount--;
			m_nGoalCount--;

			if (m_nGoalCount == 0)
			{
				m_Saverot = m_rot;

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

				if (pObjectX != nullptr)
				{
					// オブジェクトの数
					int nNumObject = (*pObjectX)->GetNumObject();

					for (int nCnt = 0; nCnt < nNumObject; nCnt++, pObjectX++)
					{
						// オブジェクトのタイプ
						CObjectX::ObjectX_Type Type = (*pObjectX)->GetObjectXType();

						if (Type == CObjectX::OBJECTX_GOAL)
						{
							D3DXVECTOR3 Pos = (*pObjectX)->GetPos();
							D3DXVECTOR3 Rot = (*pObjectX)->GetRot();

							Pos.x += sinf(Rot.y) * -80;
							Pos.z += cosf(Rot.y) * -80;

							Pos.x += sinf(Rot.y + D3DX_PI / 2) * 80;
							Pos.z += cosf(Rot.y + D3DX_PI / 2) * 80;

							m_CPos = Pos;
							m_rot.y = Rot.y - 1.0f;
						}
					}
				}
			}
		}
	}
	else if(m_nShiftGoalCount == 0)
	{
		m_nShiftGoalCount--;

		// 操作のタイプ
		CCharManager::Control_Type type = CCharManager::GetControlType();

		if (type == CCharManager::CONTROL_PLAYER)
		{
			// プレイヤーの位置に移動
			m_CPos = m_PPos;
			m_CPos.y = 0.0f;
			m_rot = m_Saverot;
		}
		else if (type == CCharManager::CONTROL_ROBOT)
		{
			// ロボットの位置に移動
			m_CPos = m_RPos;
			m_CPos.y = m_RPos.y;
			m_rot = m_Saverot;
		}

		m_bGoalOpen = false;
	}
}