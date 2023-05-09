//==============================================================================================
//
// ロボット　　　robot.h
// 土田凌聖
//
//==============================================================================================
#ifndef _ROBOT_H_
#define	_ROBOT_H_

#include"charmanager.h"

class CGage;

class CRobot : public CCharManager
{
public:
	CRobot();
	~CRobot();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRobot *Create(D3DXVECTOR3 pos);

	void Move() override;			// 移動処理
	void Action() override;			// アクション
	void Death() override;			// 死亡
	void SetMotionData(int maxkey, int key, int parts, int motion,
		int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop) override;				// モーション値の読み込み
	void Motion() override;							// モーションの設定
	void ChangeMotion(int nummotion) override;		// モーションの切り替え
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename, CModel::EModel_Type type) override;	// モデルパーツの設定
	void Process_Motion() override;		// モーション中の処理

private:
	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_PosOld;		// 過去の位置
	D3DXVECTOR3 m_Rot;			// 現在の角度
	D3DXVECTOR3 m_RotDest;		// 目的の角度
	Motion_Type m_Motion;		// ロボットの状態
	D3DXVECTOR3 m_SizeMin;		// 頂点の最小値
	D3DXVECTOR3 m_SizeMax;		// 頂点の最大値
	int m_nConChange_CoolTime;	// 操作変更のクールタイム
	int m_nLife;				// 体力
	float m_fRotSpeed;			// 回転のスピード
	float m_fSpeed;				// 移動速度
	float m_fRadius;			// 半径
	bool m_bDeath;				// 死亡したか
	bool m_bGageFull_Up;		// ゲージが満タンになったか
	bool m_bResetGage;			// ゲージをリセットしたか
	bool m_bPushButton;			// ボタンを押したか
	CGage *m_pGage;				// ゲージの情報
};

#endif // !_ROBOT_H_
