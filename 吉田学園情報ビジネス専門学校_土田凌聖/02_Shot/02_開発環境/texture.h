//**********************************************************************************************
// 
// texture.h
// Author  : katsuki mizuki
// 
//**********************************************************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==============================================================================================
// �C���N���[�h
//==============================================================================================
#include <d3dx9.h>

//==============================================================================================
// ��`
//==============================================================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{	
		TEXTURE_BG = 0,				// �w�i
		TEXTURE_TITLE,				// �^�C�g�����
		TEXTURE_RESULT,				// ���U���g���
		TEXTURE_PRESSENTER,			// PressEnter
		TEXTURE_GAMEUI,				// �Q�[����ʂ�UI
		TEXTURE_RANKING,			// �����L���O��UI
		TEXTURE_PLAYERBULLET,		// �v���C���[�̒e
		TEXTURE_ENEMYBULLET,		// �G�̒e
		TEXTURE_PLAYER0,			// �v���C���[
		TEXTURE_PLAYER1,			// �v���C���[�P
		TEXTURE_PLAYER2,			// �v���C���[�Q
		TEXTURE_PLAYER3,			// �v���C���[�R
		TEXTURE_PLAYER4,			// �v���C���[�S
		TEXTURE_PLAYER5,			// �v���C���[�T
		TEXTURE_PLAYER6,			// �v���C���[�U
		TEXTURE_PLAYER7,			// �v���C���[�V
		TEXTURE_PLAYER8,			// �v���C���[�W
		TEXTURE_PLAYER9,			// �v���C���[�X
		TEXTURE_EVOLUTION1,			// �i���P
		TEXTURE_EVOLUTION2,			// �i���Q
		TEXTURE_EVOLUTION3,			// �i���R
		TEXTURE_EVOLUTION4,			// �i���S
		TEXTURE_EVOLUTION5,			// �i���T
		TEXTURE_EVOLUTION6,			// �i���U
		TEXTURE_EVOLUTION7,			// �i���V
		TEXTURE_EVOLUTION8,			// �i���W
		TEXTURE_EVOLUTION9,			// �i���X
		TEXTURE_ENEMY0,				// �G
		TEXTURE_ENEMY1,				// �G�P
		TEXTURE_ENEMY2,				// �G�Q
		TEXTURE_ENEMY3,				// �G�R
		TEXTURE_ENEMY4,				// �G�S
		TEXTURE_ENEMY5,				// �G�T
		TEXTURE_ENEMY6,				// �G�U
		TEXTURE_ENEMY7,				// �G�V
		TEXTURE_ENEMY8,				// �G�W
		TEXTURE_ENEMY9,				// �G�X
		TEXTURE_EXPLOSION,			// ����
		TEXTURE_EXTRIANGLE,			// �a�P
		TEXTURE_EXBOX,				// �a�Q
		TEXTURE_EXPENTAGON,			// �a�R
		TEXTURE_NUMBER,				// ����
		TEXTURE_STATUS,				// �X�e�[�^�X
		TEXTURE_LEVELMAX,			// ���x��Max
		TEXTURE_MAX,
		TEXTURE_NONE,
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];	// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
