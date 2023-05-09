//==============================================================================================
// 
// キャラクターマネージャー　　　charmanager.h
// 土田凌聖
// 
//==============================================================================================
#ifndef _CHARMANAGER_H_
#define	_CHARMANAGER_H_

#include"object.h"
#include"model.h"

class CCharManager : public CObject
{
public:
	CCharManager();
	~CCharManager();
	
	// モーションのタイプ
	enum Motion_Type
	{
		MOTION_NEUTRAL = 0,		// ニュートラル
		MOTION_WALK,			// 歩き状態
		MOTION_GUN,				// 銃を構える
		MOTION_SHOT,			// 撃つ
		MOTION_KILL,			// ステルスキル
		MOTION_STEALTHDEATH,	// ステルスデス
		MOTION_DEATH,			// 死亡
		MOTION_BUTTON,			// ボタンを押す
		MOTION_FIND,			// 索敵
		MOTION_MAX,
		MOTION_NONE
	};

	// 操作のタイプ
	enum Control_Type
	{
		CONTROL_PLAYER = 0,	// プレイヤー
		CONTROL_ROBOT,		// ロボット
		CONTROL_CAMERA,		// カメラ
		CONTROL_MAX,
		CONTROL_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void Move() = 0;			// 移動処理
	virtual void Action() = 0;			// アクション
	virtual void SetMotionData(int maxkey, int key, int parts, int motion,
				int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// モーション値の読み込み
	virtual void Motion();							// モーションの設定
			bool PlayMotion(Motion_Type motion);	// モーションが終わったか
	virtual void ChangeMotion(int nummotion);		// モーションの切り替え
	virtual CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type);	// モデルパーツの設定
	virtual void Process_Motion() = 0;				// モーション再生中の処理
	void ChangeControl(Control_Type type);			// 操作の変更
	bool ChangeCoolTime();							// 操作変更のクールタイム
	int Damage(int hp, int damage);					// ダメージ
	float NormalizeRot(float rotspeed);				// 角度の正規化
	virtual void Death() = 0;		// 死亡

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }					// 位置
	void SetPosOld(D3DXVECTOR3 posold) { m_PosOld = posold; }		// 前回の位置
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }					// 角度
	void SetRotDest(D3DXVECTOR3 rotdest) { m_RotDest = rotdest; }	// 目的の角度
	void SetSizeMin(D3DXVECTOR3 min) { m_SizeMin = min; }			// サイズの最小値
	void SetSizeMax(D3DXVECTOR3 max) { m_SizeMax = max; }			// サイズの最大値
	void SetGunPos(D3DXVECTOR3 gunpos) { m_GunPos = gunpos; }		// 銃の位置
	void SetMotion(Motion_Type motion) { m_Motion_Type = motion; }	// モーション
	void SetRadius(float radius) { m_fRadius = radius; }			// 半径
	static void SetControl(Control_Type type) { m_Control_Type = type; }	// 操作のタイプ
	void SetChangeCoolTime(int cooltime) { m_nControlChange_CoolTime = cooltime; }
	void SetLife(int life) { m_nLife = life; }						// 体力
	///*static*/ void SetStealthKill(bool kill) { m_bStealthKill = kill; }
	void SetPlayStealthKill(bool kill) { m_bPlayStealthKill = kill; }
	void SetHit(bool hit) { m_bHit = hit; }
	void SetPushButton(bool button) { m_bPushButton = button; }		// ボタンを押したか

	// ゲッター
	D3DXVECTOR3 GetPos() { return m_Pos; }						// 位置
	D3DXVECTOR3 GetPosOld() { return m_PosOld; }				// 前回の位置
	D3DXVECTOR3 GetGunPos() { return m_GunPos; }				// 銃の位置
	D3DXVECTOR3 GetRot() { return m_Rot; }						// 角度
	D3DXVECTOR3 GetRotDest() { return m_RotDest; }				// 目的の角度
	D3DXVECTOR3 GetSizeMin() { return m_SizeMin; }				// サイズの最小値
	D3DXVECTOR3 GetSizeMax() { return m_SizeMax; }				// サイズの最大値
	Motion_Type GetMotion_Type() { return m_Motion_Type; }		// モーション
	static Control_Type GetControlType() { return m_Control_Type; }	// 操作のタイプ
	float GetRadius() { return m_fRadius; }						// 半径
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }				// ワールドマトリックス
	int GetLife() { return m_nLife; }							// 体力
	///*static*/ bool GetStealthKill() { return m_bStealthKill; }		// ステルスキルできるか
	bool GetPlayStealthKill() { return m_bPlayStealthKill; }		// ステルスキルできるか
	bool GetHit() { return m_bHit; }							// 弾に当たったか
	bool GetPushButton() { return m_bPushButton; }				// ボタンを押したか

private:
	static const int m_nNumParts = 11;		// パーツ数
	static const int m_nNumMotion = 11;		// モーション数

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_Pos;						// 位置
	D3DXVECTOR3 m_PosOld;					// 過去の位置
	D3DXVECTOR3 m_GunPos;					// 銃の位置
	D3DXVECTOR3 m_Rot;						// 現在の角度
	D3DXVECTOR3 m_RotDest;					// 目的の角度
	D3DXVECTOR3 InitPos[m_nNumParts];		// パーツの初期位置
	D3DXVECTOR3 InitRot[m_nNumParts];		// パーツの初期角度
	D3DXVECTOR3 m_SizeMin;					// 頂点の最小値
	D3DXVECTOR3 m_SizeMax;					// 頂点の最大値
	CModel *m_pModel[m_nNumParts];			// モデルの情報
	int m_nLife;							// 体力
	int m_nCurrentKey;						// 現在のキー番号
	int m_nCurrentMotion;					// 現在のモーション番号
	int m_nCountMotion;						// モーションカウンター
	static int m_nControlChange_CoolTime;	// 操作変更のクールタイム
	float m_fRadius;						// 半径
	float m_fSpeed;							// 移動速度
	bool m_bFootstep;						// 足音がなってるか
	bool m_bHit;							// 攻撃を受けたか
	bool m_bPushButton;						// ボタンを押したか
	bool m_bMove;							// 動いたか
	///*static*/ 	bool m_bStealthKill;			// ステルスキルができるか
	bool m_bPlayStealthKill;				// ステルスキル中か
	char *m_XFileName[m_nNumParts];			// Xファイルネーム
	Motion_Type m_Motion_Type;				// モーションのタイプ
	static Control_Type m_Control_Type;		// 操作のタイプ
	D3DXVECTOR3 POS[m_nNumParts];
	D3DXVECTOR3 ROT[m_nNumParts];

	// キー
	struct KEY
	{
		D3DXVECTOR3 KeyPos;
		D3DXVECTOR3 KeyRot;
	};

	// キーセット
	struct KEY_SET
	{
		int nFrame;
		KEY aKey[m_nNumParts];
	};

	// モーションセット
	struct MOTION_SET
	{
		int nMaxKey;			// キーの総数
		bool bLoop;				// ループするか
		bool bPlay;				// モーション再生中か
		bool bStop;				// モーションが止まったか
		KEY_SET aKeySet[15];
	};
	MOTION_SET m_MotionSet[m_nNumMotion];
};

#endif // !_CHARMANAGER_H_
