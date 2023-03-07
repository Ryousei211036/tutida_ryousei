//==============================================================================================
//
// 敵　　　enemy.h
// 土田凌聖
//
//==============================================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include"player.h"
#include"charmanager.h"

class CModel;
class CObject3D;

class CEnemy : public CCharManager

{
public:
	CEnemy();
	~CEnemy();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	void Move() override;		// 移動処理
	void Action() override;		// 敵のアクション
	void Death() override;		// 死亡
	void Search(CObject::EObjType type);		// 索敵
	void SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop) override;	// モーション値の読み込み
	void Motion() override;						// モーションの設定
	void ChangeMotion(int nummotion) override;	// モーションの切り替え
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type) override;		// モデルパーツの設定
	void Process_Motion() override;		// モーション中の処理

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	static void SetNumEnemy(int num) { m_nNumEnemy = num; }

	EObjType GetObjType() override { return OBJTYPE_ENEMY; }
	static int GetNumEnemy() { return m_nNumEnemy; }

private:
	D3DXVECTOR3 m_Pos;				// 位置
	D3DXVECTOR3 m_PosOld;			// 前回の位置
	D3DXVECTOR3 m_Rot;				// 角度
	D3DXVECTOR3 m_RotDest;			// 角度
	D3DXVECTOR3 m_EyeRot;			// 視線の角度
	D3DXVECTOR3 m_SizeMin;			// 頂点の最小値
	D3DXVECTOR3 m_SizeMax;			// 頂点の最大値
	D3DXVECTOR3 m_TargetPos;		// ターゲットの位置
	Motion_Type m_Motion;			// 敵の状態
	static int m_nNumEnemy;			// 敵の数
	int m_nLife;					// 体力
	int m_nBulletCoolTime;			// 弾を撃つ間隔
	int m_nAlpha_Exclamation;		// ビックリマークが消えるまでの時間
	int m_nChaseTime;				// 追いかける時間
	float m_fSpeed;					// 移動速度
	float m_fRotSpeed;				// 回転のスピード
	float m_fRadius;				// 半径
	float m_fKillArea;				// ステルスキルできる範囲
	float m_fKillDistance;			// ステルスキルできる距離
	float m_fView_Distance;			// 敵に見つかる距離
	bool m_bStealthKill;
	bool m_bPlay_Shot;				// 射撃モーション中か
	bool m_bStealthDeath;
	bool m_bFind_Player;			// プレイヤーを見つけたか
	bool m_bFind_Robot;				// ロボットを見つけたか
	bool m_bLostTarget;				// ターゲットを見失ったか
	bool m_bDeath;					// 死亡したか
	bool m_bHit;					// 攻撃を受けたか
	bool m_bEyeType;
	bool HitObject;
	bool FindSound;
	CObject3D *pObject3D;

	// 計算用
	D3DXVECTOR3 PPos;		// 位置(プレイヤー)
	D3DXVECTOR3 RPos;		// 位置(ロボット)
	D3DXVECTOR3 OPos;		// 位置(オブジェクトX)
};

#endif // !_ENEMY_H_
