//==============================================================================================
//
// 弾　　　bullet.h
// 土田凌聖
//
//==============================================================================================
#ifndef _BULLET_H_
#define	_BULLET_H_

#include"object2D.h"

#define	BULLET_SIZE			(15.0f)		// 弾のサイズ
#define BULLET_WIDTH		(20.0f)		// 弾の幅
#define BULLET_HEIGHT		(20.0f)		// 弾の高さ
#define	BULLET_SPEED		(8.0f)		// 弾のスピード
#define BULLET_DEFAULTDECREASE		(0.01f)	// 減衰係数
#define BULLET_RADIUS		(10.0f)		// 弾の半径

class CPlayer;
class CEnemy;

class CBullet : public CObject2D
{
public:
	CBullet();
	~CBullet();

	enum BULLET_TYPE
	{
		BULLET_NONE = 0,
		BULLET_PLAYER,		// プレイヤーの弾
		BULLET_ENEMY,		// 敵の弾
		BULLET_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CBullet *Create(D3DXVECTOR3 pos, float rot, int type, float life, float speed, float size, float attack, float decrease);
	bool BulletMove(float *BulletLife, D3DXVECTOR3 *ShotMove, float Decrease);

	void SetRad(float Rad) { m_fRadius = Rad; }							// 半径の設定
	void SetRot(float rot) { m_fRot = rot; }							// 角度の設定
	void SetType(int type) { m_nType = type; }							// 種類の設定
	void SetPBulletLife(float life) { m_fPBulletLife = life; }			// プレイヤーの弾の体力の設定
	void SetPBulletSpeed(float speed) { m_PlayerShotMove.y = speed; }	// プレイヤーの弾の速度の設定
	void SetPBulletAttack(float attack) { m_fPAttack = attack; }		// プレイヤーの弾の攻撃力の設定
	void SetPDecrease(float decrease) { m_fPDecrease = decrease; }		// プレイヤーの弾の減衰係数の設定
	void SetEBulletLife(float life) { m_fEBulletLife = life; }			// 敵の弾の体力の設定
	void SetEBulletSpeed(float speed) { m_EnemyShotMove.y = speed; }	// 敵の弾の速度の設定
	void SetEBulletAttack(float attack) { m_fEAttack = attack; }		// 敵の弾の攻撃力の設定
	void SetEDecrease(float decrease) { m_fEDecrease = decrease; }		// 敵の弾の減衰係数の設定

	EObjType GetObjType() override { return OBJTYPE_BULLET; }	// オブジェクトのタイプの取得
	D3DXVECTOR3 GetPos() override { return m_Pos; }				// 位置の取得

private:
	D3DXVECTOR3 m_Pos;				// 位置
	D3DXVECTOR3 m_InitPos;			// 初期位置
	D3DXVECTOR3 m_Move;				// 移動量
	D3DXVECTOR3 m_PlayerShotMove;	// プレイヤーの弾の移動量
	D3DXVECTOR3 m_EnemyShotMove;	// 敵の弾の移動量
	int m_nType;					// 弾の種類
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	float m_fRot;					// 角度
	float m_fPBulletLife;			// プレイヤーの弾の寿命
	float m_fEBulletLife;			// 敵の弾の寿命
	float m_fPAttack;				// プレイヤーの攻撃力
	float m_fEAttack;				// 敵の攻撃力
	float m_fPDecrease;				// プレイヤーの減衰係数
	float m_fEDecrease;				// 敵の減衰係数
	float m_fDistance;				// オブジェクトと弾の距離
	float m_fRadius;				// 弾の半径
	bool m_bHit;					// 当たったか
};

#endif // !_BULLET_H_
