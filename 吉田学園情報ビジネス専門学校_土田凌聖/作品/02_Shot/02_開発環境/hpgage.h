//==============================================================================================
//
// 体力ゲージ　　　hpgage.h
// 土田凌聖
//
//==============================================================================================
#ifndef _HPGAGE_H_
#define	_HPGAGE_H_
#include"object2D.h"

#define	HP_WIDTH		(25.0f)		// 体力ゲージの幅
#define	HP_HEIGHT		(5.0f)		// 体力ゲージの高さ
#define	HP				(50.0f)		// 体力
#define HP_COOLTIME		(300)		// 体力ゲージが消えるまでの時間

class CHpGage : public CObject2D
{
public:
	CHpGage();
	~CHpGage();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	
	static CHpGage *Create(D3DXVECTOR3 pos, float width, float width2, float height, D3DXCOLOR col);
	EObjType GetObjType() override { return OBJTYPE_HP; }		// オブジェクトのタイプの取得
	void SubHpgage(float Damage);	// 体力ゲージを減らす
	void AddHpgage(float Heal);		// 体力ゲージを増やす
	void SetPos(D3DXVECTOR3 pos);	// 体力ゲージの位置
	void SetCol(D3DXCOLOR col);		// 体力ゲージの色

private:
	D3DXVECTOR3 m_Pos;
	float m_fTotal;				// 体力
	float m_fHpWidth;			// 体力ゲージの長さ
	float m_fSubHp;				// 体力の減少量
	bool m_bZeroHp;				// 体力ゲージが０になったか
};

#endif // !_HP_H_
