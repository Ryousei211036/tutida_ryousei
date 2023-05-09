//==============================================
//
//�J�����̏���[camera.h]
//Author�F�y�c����
//
//==============================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"renderer.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void Matrix();
	void Move();
	void Collision();
	void GoalCamera();

	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }

	D3DXVECTOR3 GetPosV() { return m_posV; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	CCamera *GetCamera() { return m_pCamera; }
	bool GoalOpen() { return m_bGoalOpen; }

private:
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_InitposV;			// ���_�̏����l
	D3DXVECTOR3 m_posVDest;			// �ړI�̎��_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_InitposR;			// �����_�̏����l
	D3DXVECTOR3 m_posRDest;			// �ړI�̒����_
	D3DXVECTOR3 m_CPos;				// �L�����N�^�[�̈ʒu
	D3DXVECTOR3 m_PPos;				// �v���C���[�̈ʒu
	D3DXVECTOR3 m_RPos;				// ���{�b�g�̈ʒu
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotDest;			// �ړI�̌���
	D3DXVECTOR3 m_Saverot;			// �p�x�̕ۑ�
	int m_nShiftGoalCount;			// �S�[�����f������
	int m_nGoalCount;				// �S�[�����f���܂ł̎���
	float m_fDistance;				// ���_���璍���_�܂ł̋���
	float m_fPlayerDistance;		// ���_����v���C���[�܂ł̋���
	float m_fRotSpeed;				// ��]���x
	bool m_bCollision;				// �I�u�W�F�N�g�ɓ������Ă��邩
	bool m_bGoalOpen;				// �S�[�����J������

	D3DXMATRIX m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_WorldCameraPosV;	// ���_�̃��[���h���W
	D3DXVECTOR3 m_WorldCameraPosR;	// �����_�̃��[���h���W

	CCamera *m_pCamera;		//�J�����̏��
};

#endif // !_CAMERA_H_