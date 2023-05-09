//==============================================================================================
//
// 敵　　　enemy.h
// 土田凌聖
//
//==============================================================================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include"object2D.h"

#define MAX_ENEMY				(5)			// 敵の最大数
#define	ENEMY_SIZE				(25.0f)		// 敵のサイズ
#define	ENEMY_WIDTH				(50.0f)		// 敵の幅
#define	ENEMY_HEIGHT			(50.0f)		// 敵の高さ
#define ENEMYCREATE_TIME		(600)		// 生成までの時間
#define	ENEMY_DAMAGECOOLTIME	(100.0f)	// 攻撃をくらった後のクールタイム
#define	ENEMY_RADIUS			(30.0f)		// 敵の半径
#define	ENEMY_ROT				(0.05f)		// 敵の回転速度
#define TARGET_COUNT			(600)		// プレイヤーを狙う時間
#define ENEMY_DEFAULTLIFE		(20)		// 最初の体力
#define ENEMY_DEFAULTMAXLIFE	(20)		// 最初の体力の最大値
#define	ENEMY_DEFAULTATTACK		(5.0f)		// 最初の攻撃力
#define	ENEMY_DEFAULTSPEED		(3)			// 最初の移動スピード
#define	ENEMY_DEFAULTBODYDAMAGE	(10.0f)		// 最初の本体攻撃力
#define ENEMY_BULLETSPEED		(8)			// 弾の速度
#define ENEMY_BULLETLIFE		(100.0f)	// 弾の体力
#define	ENEMY_DEFAULTRELOATD	(40.0f)		// 最初のリロード時間
#define ENEMY_MAXLEVEL			(30)		// 最大レベル
#define ENEMY_DECREASE			(0.05f)		// 減衰係数
#define ENEMY_NUMSTATUS			(8)			// ステータスの数

class CPlayer;
class CHpGage;
class CStatuslevel;

class CEnemy : public CObject2D
{
public:
	CEnemy();
	~CEnemy() override;

	enum EnemyType
	{
		ENEMY_NONE = 0,
		ENEMY_TYPE0,		// 敵タイプ０
		ENEMY_TYPE1,		// 敵タイプ１
		ENEMY_TYPE2,		// 敵タイプ２
		ENEMY_TYPE3,		// 敵タイプ３
		ENEMY_TYPE4,		// 敵タイプ４
		ENEMY_TYPE5,		// 敵タイプ５
		ENEMY_TYPE6,		// 敵タイプ６
		ENEMY_TYPE7,		// 敵タイプ７
		ENEMY_TYPE8,		// 敵タイプ８
		ENEMY_TYPE9,		// 敵タイプ９
		ENEMY_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CEnemy *Create(D3DXVECTOR3 pos, int Level);
	void Move();						// 移動処理
	void Shot();						// 弾を撃つ
	void LevelUp();						// レベルアップ処理
	void HitDamage(float damage);		// 体力ゲージが減る処理
	void HealHp(float heal);			// 体力ゲージが増える処理
	void Enemy(LPD3DXFONT pFont);

	void SetLevel(int Level);			// レベルの設定
	void SetLife(int type, float life) override;

	EObjType GetObjType() override { return OBJTYPE_ENEMY; }		// オブジェクトのタイプの取得
	D3DXVECTOR3 GetPos() override { return m_Pos; }					// 位置の取得
	float GetSize() override { return m_fSize; }					// サイズの取得
	float GetRadius() override { return m_fRadius; }				// 半径の取得
	float GetLife() { return m_fLife; }								// 体力の取得
	float GetAttack() override { return m_fAttack; }				// 攻撃力の取得
	float GetBodyDamage() override { return m_fBodyDamage; }		// 本体ダメージの取得
	float GetBulletSpeed() override { return m_fBulletSpeed; }		// 弾のスピードの取得
	static int GetNumEnemy() { return m_nNumEnemy; }				// 敵の数の取得
	static int GetNumMaxEnemy() { return m_nNumEnemy; }				// 敵の最大数の取得
	static int GetNumDeathEnemy() { return m_nDeathCount; }				// 敵の最大数の取得

	static void SetNumEnemy(int numenemy) { m_nNumEnemy = numenemy; }

private:
	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_InitPos;		// 初期位置
	D3DXVECTOR3 m_Move;			// 敵の移動量
	D3DXVECTOR3 m_PlayerMove;	// プレイヤーの移動量
	D3DXVECTOR3 m_TotalMove;	// 移動量の合計
	D3DXVECTOR3 m_CreatePos;	// 生成する位置
	D3DXVECTOR3 m_BulletPos;	// 弾を発射する位置
	int m_fTargetCount;			// プレイヤーを狙う時間
	int m_nExp;					// 経験値
	int m_nExpSave;				// 経験値の保存先
	int m_nGiveExp;				// 渡す経験値
	int m_nScore;				// スコア
	int m_nDamage;				// 受けたダメージ
	int m_nHpCoolTime;			// 体力ゲージが消えるまでの時間
	int m_nLevel;				// 敵のレベル
	int m_nLevelSave;			// レベルの保存
	int m_nGrawLevel[3];		// 進化するレベル
	int m_nStatuslevel[8];		// ステータスレベル
	int m_nLvUpCount;			// レベルが上がった回数
	float m_fRot;				// 角度
	float m_fSize;				// サイズ
	float m_fWidth;				// 幅
	float m_fHeight;			// 高さ
	float m_fCoolTime;			// 弾のクールタイム
	float m_fRadius;			// 敵の半径
	float m_fBulletDecrease;	// 弾の減衰係数
	float m_fDamageCoolTime;	// 攻撃を受けた後のクールタイム
	float m_fTotalDamage;		// 受けたダメージの合計
	float m_fHpAlpha;			// 体力ゲージのアルファ値
	float m_fAngle;				// 敵とプレイヤー、餌の角度
	float m_fAngleDest;			// 目的の角度
	float m_fTargetDistance;	// ターゲットとの距離
	float m_fLife;				// 体力
	float m_fHealLife;			// 体力の自動回復量
	float m_fMaxLife;			// 体力の最大値
	float m_fBodyDamage;		// 本体ダメージ
	float m_fBulletSpeed;		// 弾の速度
	float m_fBulletLife;		// 弾の体力
	float m_fAttack;			// 攻撃力
	float m_fReload;			// リロードの時間
	float m_fSpeed;				// スピード
	bool m_bGrawUp;				// 進化したか
	bool m_bHit;				// 攻撃が当たったか
	bool m_bNearPlayer;			// プレイヤーに近いか
	bool m_bCreateHp;			// 体力ゲージが出たか
	CHpGage *m_pHpgage;			// 体力ゲージの情報
	EnemyType m_EnemyType;		// 敵のタイプ
	D3DXVECTOR3 m_TargetPos;	// ターゲットの位置
	CStatuslevel *m_pStatuslevel[ENEMY_NUMSTATUS];	// ステータスレベルの情報
	static int m_nDeathCount;	// 倒された数
	static int m_nNumEnemy;		// 敵の数
	static int m_nCreateTime;	// 敵生成のクールタイム

	LPD3DXFONT m_Font;		// フォント
};

#endif // !_ENEMY_H_
