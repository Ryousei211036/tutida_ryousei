//==============================================================================================
//
// �̗̓Q�[�W�@�@�@hpgage.h
// �y�c����
//
//==============================================================================================
#ifndef _HPGAGE_H_
#define	_HPGAGE_H_
#include"object2D.h"

#define	HP_WIDTH		(25.0f)		// �̗̓Q�[�W�̕�
#define	HP_HEIGHT		(5.0f)		// �̗̓Q�[�W�̍���
#define	HP				(50.0f)		// �̗�
#define HP_COOLTIME		(300)		// �̗̓Q�[�W��������܂ł̎���

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
	EObjType GetObjType() override { return OBJTYPE_HP; }		// �I�u�W�F�N�g�̃^�C�v�̎擾
	void SubHpgage(float Damage);	// �̗̓Q�[�W�����炷
	void AddHpgage(float Heal);		// �̗̓Q�[�W�𑝂₷
	void SetPos(D3DXVECTOR3 pos);	// �̗̓Q�[�W�̈ʒu
	void SetCol(D3DXCOLOR col);		// �̗̓Q�[�W�̐F

private:
	D3DXVECTOR3 m_Pos;
	float m_fTotal;				// �̗�
	float m_fHpWidth;			// �̗̓Q�[�W�̒���
	float m_fSubHp;				// �̗͂̌�����
	bool m_bZeroHp;				// �̗̓Q�[�W���O�ɂȂ�����
};

#endif // !_HP_H_
