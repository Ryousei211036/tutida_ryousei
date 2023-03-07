//==============================================================================================
// 
// キャラクターマネージャー　　　charmanager.cpp
// 土田凌聖
// 
//==============================================================================================
#include"charmanager.h"
#include"model.h"
#include"application.h"
#include"file.h"
#include"game.h"
#include"inputkeyboard.h"
#include"inputjoypad.h"
#include"objectX.h"
#include"tutorial.h"

CCharManager::Control_Type CCharManager::m_Control_Type = CONTROL_PLAYER;
int CCharManager::m_nControlChange_CoolTime = 60;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CCharManager::CCharManager()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCharManager::~CCharManager()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CCharManager::Init()
{
	m_Control_Type = CONTROL_PLAYER;	// 操作のタイプ

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			// パーツの初期位置
			InitPos[nCnt] = m_pModel[nCnt]->GetPos();
			InitRot[nCnt] = m_pModel[nCnt]->GetRot();
		}
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CCharManager::Uninit()
{
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
		}
	}

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCharManager::Update()
{
	// モーション番号の設定
	ChangeMotion(m_Motion_Type);

	// モーション
	Motion();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CCharManager::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			// モデルの描画処理
			m_pModel[nCnt]->Draw(m_mtxWorld);
		}
	}
}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CCharManager::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// キーフレームの設定
	m_MotionSet[motion].aKeySet[key].nFrame = frame;

	// 位置と角度の設定
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyRot = rot;

	// キーの最大数
	m_MotionSet[motion].nMaxKey = maxkey;

	// ループするか
	m_MotionSet[motion].bLoop = loop;
}

//==============================================================================================
// モーションの再生
//==============================================================================================
void CCharManager::Motion()
{
	// モーションカウンター
	m_nCountMotion++;

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr && !m_MotionSet[m_nCurrentMotion].bStop)
		{
			// 相対値
			float fRelative = (float)m_nCountMotion / (float)m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame;

			// 差分
			D3DXVECTOR3 fPosDifference;		// 位置用
			D3DXVECTOR3 fRotDifference;		// 回転用

			// 差分
			// 最後のキーじゃない場合
			if (m_nCurrentKey != m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// 終了値 - 開始値

				// 角度
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// 終了値 - 開始値
			}
			// 最後のキーの場合
			else if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// 終了値 - 開始値

				// 角度
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// 終了値 - 開始値
			}

			// 現在値
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// 位置
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// 回転

			POS[nCnt] = Pos;
			ROT[nCnt] = Rot;

			m_pModel[nCnt]->SetPos(Pos);	// 位置の設定
			m_pModel[nCnt]->SetRot(Rot);	// 回転の設定
		}
	}

	// 最後のフレームまで進んだ
	if (m_nCountMotion >= m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame)
	{
		// フレームを0に戻す
		m_nCountMotion = 0;

		// キーを進める
		m_nCurrentKey++;

		// ループする
		if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey
			&& m_MotionSet[m_nCurrentMotion].bLoop)
		{
			// キーを戻してループする
			m_nCurrentKey = 0;
		}

		// ループしない
		else if (m_nCurrentKey >= m_MotionSet[m_nCurrentMotion].nMaxKey
			&& !m_MotionSet[m_nCurrentMotion].bLoop)
		{
			m_MotionSet[m_nCurrentMotion].bPlay = false;
			m_MotionSet[m_nCurrentMotion].bStop = true;

			for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
			{
				m_pModel[nCnt]->SetPos(POS[nCnt]);	// 位置の設定
				m_pModel[nCnt]->SetRot(ROT[nCnt]);	// 回転の設定
			}
		}
	}
}

//==============================================================================================
// モーションが中か
//==============================================================================================
bool CCharManager::PlayMotion(Motion_Type motion)
{
	return m_MotionSet[motion].bPlay;
}

//==============================================================================================
// モーション変更
//==============================================================================================
void CCharManager::ChangeMotion(int nummotion)
{
	// 現在のモーションから変わった場合
	if (m_nCurrentMotion != nummotion)
	{
		// 前回のモーションのプレイ中をfalseにする
		m_MotionSet[m_nCurrentMotion].bPlay = false;
		
		// 前回の止まったモーションをfalseにする
		m_MotionSet[m_nCurrentMotion].bStop = false;

		m_nCurrentMotion = nummotion;	// モーションを切り替える
		m_nCurrentKey = 0;				// キーを0にする
		m_nCountMotion = 0;				// モーションカウントを0にする

		// 次のモーションのプレイ中をtrueにする
		m_MotionSet[m_nCurrentMotion].bPlay = true;
	}
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel *CCharManager::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type)
{
	if (m_pModel[index] == nullptr)
	{
		if (parent >= 0)
		{// 親がいる場合
			m_pModel[index] = CModel::Create(pos, rot, m_pModel[parent], Xfilename, type);
		}
		else
		{// 親がいない場合
			m_pModel[index] = CModel::Create(pos, rot, nullptr, Xfilename, type);
		}
	}

	return m_pModel[index];
}

//==============================================================================================
// 操作の変更
//==============================================================================================
void CCharManager::ChangeControl(Control_Type type)
{
	// ジョイパッドの情報
	CInputJoypad *pJoypad = CApplication::GetInputJoypad();

	// 操作変更のクールタイム
	bool bChange = ChangeCoolTime();

	if ((CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_V) || pJoypad->GetJoypadTrigger(CInputJoypad::JOYKEY_X))
		&& bChange)
	{
		if (CApplication::GetMode() == CApplication::MODE_GAME
			&& CGame::GetRobot() == nullptr)
		{
			CCharManager *pRobot;

			pRobot = *CGame::GetFile()->LoadCharFile(CFile::TXTNAME_ROBOT, m_Pos);

			CGame::SetRobot(pRobot);
		}
		else if (CApplication::GetMode() == CApplication::MODE_TUTORIAL
			&& CTutorial::GetRobot() == nullptr)
		{
			CCharManager *pRobot;

			pRobot = *CTutorial::GetFile()->LoadCharFile(CFile::TXTNAME_ROBOT, m_Pos);

			CTutorial::SetRobot(pRobot);
		}

		// 操作を切り替える
		CCharManager::SetControl(type);

		// クールタイムのリセット
		SetChangeCoolTime(60);
	}

#ifdef _DEBUG
	if (CApplication::GetInputKeyboard()->GetKeyboardTrigger(DIK_O))
	{
		// オブジェクト操作に切り替える
		CCharManager::SetControl(CONTROL_CAMERA);
	}
#endif // _DEBUG
}

//==============================================================================================
// 操作変更のクールタイム
//==============================================================================================
bool CCharManager::ChangeCoolTime()
{
	m_nControlChange_CoolTime--;

	if (m_nControlChange_CoolTime <= 0)
	{
		return true;
	}

	return false;
}

//==============================================================================================
// ダメージ
//==============================================================================================
int CCharManager::Damage(int hp, int damage)
{
	int nHp = hp - damage;

	return nHp;
}

//==============================================================================================
// 角度の正規化
//==============================================================================================
float CCharManager::NormalizeRot(float rotspeed)
{
	// 目的の角度の正規化
	if (m_RotDest.y - m_Rot.y > D3DX_PI)
	{
		m_RotDest.y -= D3DX_PI * 2;
	}
	else if (m_RotDest.y - m_Rot.y < -D3DX_PI)
	{
		m_RotDest.y += D3DX_PI * 2;
	}

	// 目的の角度にする
	m_Rot.y += (m_RotDest.y - m_Rot.y) * rotspeed;

	// 角度の正規化
	if (m_Rot.y > D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2;
	}
	else if (m_Rot.y < -D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2;
	}

	return m_Rot.y;
}