//==============================================================================================
//
// プレイヤー　　　player.h
// 土田凌聖
//
//==============================================================================================
#ifndef _PLAYER_H_
#define	_PLAYER_H_

#include"object2D.h"

#define	PLAYER_SIZE					(25.0f)		// サイズ
#define PLAYER_WIDTH				(50.0f)		// 幅
#define PLAYER_HEIGHT				(50.0f)		// 高さ
#define PLAYER_BULLETPOS			(30.0f)		// 中心から砲口までの距離
#define PLAYER_DEFAULTLIFE			(200)		// 最初の体力
#define PLAYER_DEFAULTMAXLIFE		(200)		// 最初の体力の最大値
#define	PLAYER_DEFAULTATTACK		(5.0f)		// 最初の攻撃力
#define	PLAYER_DEFAULTSPEED			(3)			// 最初の移動スピード
#define	PLAYER_DEFAULTBODYDAMAGE	(10.0f)		// 最初の本体攻撃力
#define PLAYER_BULLETSPEED			(8)			// 弾の速度
#define PLAYER_BULLETLIFE			(100.0f)	// 弾の体力
#define	PLAYER_ROT					(0.05f)		// 回転速度
#define	PLAYER_RADIUS				(30.0f)		// 半径
#define	PLAYER_DEFAULTRELOATD		(40.0f)		// 最初のリロード時間
#define PLAYER_DECREASE				(0.02f)		// 減衰係数
#define PLAYER_STOPPOS				(1200)		// 移動できる範囲
#define PLAYER_MAXLEVEL				(30)		// 最大レベル
#define PLAYER_NUMSTATUS			(8)			// ステータスの数sdf

// 前方宣言
class CEnemy;
class CHpGage;
class CExpgage;
class CStatus;
class CStatuslevel;
class CEvodisplay;
class CLevelDisplay;

class CPlayer : public CObject2D
{
public:
	CPlayer();
	~CPlayer();

	enum PlayerType
	{
		PLAYER_NONE = 0,
		PLAYER_TYPE0,		// プレイヤータイプ０
		PLAYER_TYPE1,		// プレイヤータイプ１
		PLAYER_TYPE2,		// プレイヤータイプ２
		PLAYER_TYPE3,		// プレイヤータイプ３
		PLAYER_TYPE4,		// プレイヤータイプ４
		PLAYER_TYPE5,		// プレイヤータイプ５
		PLAYER_TYPE6,		// プレイヤータイプ６
		PLAYER_TYPE7,		// プレイヤータイプ７
		PLAYER_TYPE8,		// プレイヤータイプ８
		PLAYER_TYPE9,		// プレイヤータイプ９
		PLAYER_MAX
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CPlayer *Create();
	void LevelUp();
	static D3DXVECTOR3 Move();
	static D3DXVECTOR3 StopX(D3DXVECTOR3 pos);
	static D3DXVECTOR3 StopY(D3DXVECTOR3 pos);
	void Shot();
	void HitDamage(float damage);		// 体力ゲージが減る処理
	void HealHp(float heal);			// 体力ゲージが増える処理
	void Player(LPD3DXFONT pFont);

	void SetLife(int type, float life) override;
	void SetExp(int exp) override;

	EObjType GetObjType() override { return OBJTYPE_PLAYER; }		// オブジェクトのタイプの取得
	D3DXVECTOR3 GetPos() override { return m_Pos; }					// プレイヤーの位置の取得
	D3DXVECTOR3 GetMove() override { return m_Move; }				// プレイヤーの移動量の取得
	float GetSize() override { return m_fSize; }					// サイズの取得
	float GetRadius() override { return m_fRadius; }				// 半径の取得
	float GetLife() override { return m_fLife; }						// 体力の取得
	float GetAttack() override { return m_fAttack; }					// 攻撃力の取得
	float GetBodyDamage() override { return m_fBodyDamage; }			// 本体ダメージの取得
	float GetBulletSpeed() override { return m_fBulletSpeed; }		// 弾のスピードの取得

private:
	int m_nLevel;								// プレイヤーのレベル
	int m_nCntLevel;							// レベルが上がった回数
	int m_nLevelSave;							// レベルの保存
	int m_nGrawLevel[3];						// 進化するレベル
	int m_nGrawCount;							// 進化した回数
	int m_nExp;									// 経験値
	int m_nLevelUpExp;							// レベルアップに必要な経験値
	int m_nHpCoolTime;							// 体力ゲージが消えるまでの時間
	int m_nStatuslevel[8];						// ステータスレベル
	float m_fSize;								// サイズ
	float m_fWidth;								// 幅
	float m_fHeight;							// 高さ
	float m_fRadius;							// プレイヤーの半径
	float m_fDamage;							// 受けたダメージ
	float m_fTotalDamage;						// 受けたダメージの合計
	float m_fHeal;								// 体力回復量
	float m_fHpAlpha;							// 体力ゲージのアルファ値
	float m_fStatusAlpha;						// ステータス表示のアルファ値
	float m_fEvoAlpha;							// 進化先表示のアルファ値
	float m_fHealLife;							// 体力の回復量
	float m_fMaxLife;							// 体力の最大値
	float m_fBodyDamage;						// 本体ダメージ
	float m_fBulletSpeed;						// 弾の速度
	float m_fBulletLife;						// 弾の体力
	float m_fAttack;							// 攻撃力
	float m_fReload;							// リロードの時間
	float m_fBulletDecrease;					// 弾の減衰係数
	bool bRelease;								// プレイヤーリリースされたか
	bool m_bCreateHp;							// 体力ゲージが出たか
	bool m_bLevelUp;							// レベルが上がったか
	bool m_bDeathSound;							// 死亡音がなったか
	D3DXVECTOR3 m_Pos;							// 位置
	PlayerType m_PlayerType;					// プレイヤーのタイプ
	CHpGage *m_pHpgage;							// 体力ゲージのの情報
	CExpgage *m_pExpgage;						// 経験値ゲージの情報
	CStatus *m_pStatus;							// ステータスの情報
	CStatuslevel *m_pStatuslevel[PLAYER_NUMSTATUS];	// ステータスレベルの情報
	CEvodisplay *m_pEvodisplay[3];				// 進化先の情報
	CLevelDisplay *m_plevel;					// プレイヤーのレベル表示の情報
	static D3DXVECTOR3 m_ZeroPos;				// 0の位置
	static D3DXVECTOR3 m_Move;					// 移動量
	static D3DXVECTOR3 m_BulletPos;				// 弾を発射する位置
	static float m_fRot;						// 角度
	static float m_fLife;						// 体力
	static float m_fSpeed;						// スピード
	static float m_fCoolTime;					// 弾のクールタイム
	static bool m_bCollisionX;					// 壁に当たったか(X座標)
	static bool m_bCollisionY;					// 壁に当たったか(Y座標)

	LPD3DXFONT m_Font;		// フォント
};

#endif // !_PLAYER_H_
