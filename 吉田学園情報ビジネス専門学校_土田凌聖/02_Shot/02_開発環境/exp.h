//==============================================================================================
//
// 餌　　　exp.h
// 土田凌聖
//
//==============================================================================================
#ifndef _EX_H_
#define	_EX_H_

#include"object2D.h"
#include"object.h"
#include"bullet.h"

#define	MAX_EXP				(50)		// 餌の最大数
#define	EXP_RADIUS			(20.0f)		// 餌の半径
#define EXP_CREATETIME		(50)		// 生成までの時間
#define	EXP_WIDTH			(10.0f)		// 餌の幅
#define	EXP_HEIGHT			(10.0f)		// 餌の高さ
#define	EXP_TRIANGLESIZE	(20.0f)		// 三角餌のサイズ
#define	EXP_BOXSIZE			(15.0f)		// 四角餌のサイズ
#define	EXP_PENTAGONSIZE	(30.0f)		// 五角形餌のサイズ
#define	EXP_TRIANGLELIFE	(15)		// 三角餌の体力
#define	EXP_BOXLIFE			(10)		// 四角餌の体力
#define	EXP_PENTAGONLIFE	(20)		// 五角形餌の体力
#define EXP_BODYDAMAGE		(5)			// 本体攻撃力
#define	EXP_DAMAGECOOLTIME	(100.0f)	// 攻撃をくらった後のクールタイム
#define EXP_DECREASE		(0.1f)		// 減衰係数

class CHpGage;

class CExp : public CObject2D
{
public:
	CExp();
	~CExp();

	enum Exptype
	{
		EXPTYPE_TRIANGLE = 0,	// 三角
		EXPTYPE_BOX,			// 四角
		EXPTYPE_PENTAGON,		// 五角形
		EXPTYPE_MAX,
		EXPTYPE_NONE,
	};

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	static CExp *Create(D3DXVECTOR3 pos, int type, float rot);

	void SetLife(int type, float life) override;
	void SetType(int type) { m_nType = type; }
	float HitDamage(float damage); 								// 受けたダメージの取得
	static void SetExpBreakType(CBullet::BULLET_TYPE type) { m_BulletType = type; }
	void SetHpGage(float hpgage) override{ m_fHpGage = hpgage; }

	float GetSize() override { return m_fSize; }				// サイズの取得
	float GetLife() override { return m_fLife; }				// 体力の取得
	EObjType GetObjType() override { return OBJTYPE_EXP; }		// オブジェクトのタイプの取得
	CExp::Exptype GetExpType();									// 餌のタイプの取得
	D3DXVECTOR3 GetPos() override { return m_Pos; }				// 位置の取得
	float GetRadius() override { return m_fRadius; }			// 半径の取得
	int GetExp() override { return m_nExp; }					// 経験値の取得
	int GetNumTriangle() override { return m_nNumTriangle; }	// 三角の数
	int GetNumBox() override { return m_nNumBox; }				// 四角の数
	int GetNumPentagon() override { return m_nNumPentagon; }	// 五角形の数
	static int GetNumExp() { return m_nNumExp; }				// 餌の数
	float GetBodyDamage() override { return m_fBodyDamage; }	// 攻撃力の取得
	float GetHpGage() override { return m_fHpGage; }			// 体力ゲージの長さ

private:
	D3DXVECTOR3 m_Pos;				// 位置
	D3DXVECTOR3 m_InitPos;			// 初期位置
	D3DXVECTOR3 m_Move;				// 移動量
	D3DXVECTOR3 m_PushMove;			// 押された時の移動量
	D3DXVECTOR3 m_TotalMove;		// 移動量の合計
	int m_nExp;						// 経験値
	int m_nScore;					// スコア
	int m_nHpCoolTime;				// 体力ゲージが消えるまでの時間
	int m_nType;					// 種類
	int m_nBodyDamageType;			// プレイヤーと敵どっちに当たったか
	float m_fSize;					// サイズ
	float m_fWidth;					// 幅
	float m_fHeight;				// 高さ
	float m_fLife;					// 体力
	float m_fMaxLife;				// 最大体力
	float m_fRadius;				// 半径
	float m_fBodyDamage;			// 本体攻撃力
	float m_fDamageCoolTime;		// 攻撃をくらった後のクールタイム
	float m_fHpAlpha;				// 体力ゲージのアルファ値
	float m_fHpGage;				// 体力ゲージの長さ
	float m_fDamage;				// 受けたダメージ
	bool m_bDamage;					// 攻撃をくらったか
	bool m_bCreateHp;				// 体力ゲージが出たか
	CHpGage *m_pHpgage;			// 体力ゲージの情報
	static CBullet::BULLET_TYPE m_BulletType;		// 餌を壊した弾の種類
	static D3DXVECTOR3 m_CreatePos;	// 生成する位置
	static int m_nNumTriangle;		// 三角の数
	static int m_nNumBox;			// 四角の数
	static int m_nNumPentagon;		// 五角形の数
	static int m_nNumExp;			// 数
};

#endif // !_EX_H_
