//==============================================================================================
//
// ステータスのレベルの表示　　　statuslevel.h
// 土田凌聖
//
//==============================================================================================
#ifndef _STATUSLEVEL_H_
#define	_STATUSLEVEL_H_

#include"score.h"
#include"texture.h"

#define	MAX_STATUSLEVLE		(6)	// ステータスの最大レベル

class CStatuslevel : public CScore
{
public:
	CStatuslevel();
	~CStatuslevel();

	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CStatuslevel *Create(D3DXVECTOR3 pos, float width, float height);
	void AddStatuslevel(int statuslevel);

	void SetSize(float width, float height);
	void SetStatuslevel(int statuslevel);
	void SetCol(D3DXCOLOR col);
	
private:
	CNumber *m_pNumber;		// 数字の情報
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	int m_nStatuslevel;		// ステータスレベル
	CTexture::TEXTURE m_pTexture;	// テクスチャ変更用
};

#endif // !_STATUSLEVEL_H_
