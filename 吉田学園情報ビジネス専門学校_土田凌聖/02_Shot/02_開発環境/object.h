//==============================================================================================
//
// オブジェクト　　　object.h
// Author : 土田凌聖
//
//==============================================================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_

#include <d3dx9.h>

#define	MAX_OBJECT		(300)		// オブジェクトの最大数
#define	MAX_PRI			(6)			// 優先順位の最大数

class CBullet;
class CExp;

class CObject
{
public:
	//CObject();
	explicit CObject(int nPriority = 3);
	virtual ~CObject();

	// オブジェクトの種類
	enum EObjType
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,		// プレイヤー
		OBJTYPE_ENEMY,		// 敵
		OBJTYPE_BULLET,		// 弾
		OBJTYPE_EXP,		// 餌
		OBJTYPE_HP,			// 体力ゲージ
		OBJTYPE_FADE,		// フェード
		OBJTYPE_MAX
	};

	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();
	CObject *Getobject(int nPriority, int nNum);				// オブジェクト情報の取得

	virtual void SetLife(int type, float life) { type, life;}	// 体力の設定の設定
	virtual void SetExp(int exp) { exp; }						// 経験値の設定の設定
	virtual void SetHpGage(float hpgage) { hpgage; }			// 体力ゲージの長さの設定
	virtual void SetHit(bool hit) { hit; }						// 攻撃が当たったか

	virtual float GetLife() { return 0.0f; }								// 体力の取得
	virtual float GetAttack() { return 0.0f; }								// 攻撃力の取得
	virtual float GetBodyDamage() { return 0.0f; }						// 本体ダメージの取得
	virtual int GetBulletLife() { return 0; }							// 弾の体力の取得
	virtual float GetBulletSpeed() { return 0.0f; }						// 弾の速度の取得
	virtual D3DXVECTOR3 GetPos() { return{}; }							// 位置の取得
	virtual D3DXVECTOR3 GetMove() { return{}; }							// 移動量の取得
	virtual float GetSize() { return 0.0f; }								// サイズの取得
	virtual float GetRadius() { return 0.0f; }								// 半径の取得
	virtual int GetExp() { return 0; }									// 経験値の取得
	virtual EObjType GetObjType() { return OBJTYPE_NONE; }							// オブジェクトタイプの取得
	virtual int GetNumTriangle() { return 0; }							// 三角の餌の数
	virtual int GetNumBox() { return 0; }								// 四角の餌の数
	virtual int GetNumPentagon() { return 0; }							// 五角形の餌の数
	virtual float GetHpGage() { return 0.0f; }								// 体力ゲージの長さの取得

private:
	static CObject *m_apObject[MAX_PRI][MAX_OBJECT];
	static int m_nNumaAll;
	int m_nID;
	int m_nPriority;
};

#endif // !_OBJECT_H_
