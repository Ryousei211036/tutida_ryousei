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
		TEXTURE_TILE = 0,
		TEXTURE_NUMBER,
		TEXTURE_TITLE,
		TEXTURE_RESULT,
		TEXTURE_SKIP,
		TEXTURE_EXCLAMATION,
		TEXTURE_TITLELOGO,
		TEXTURE_START,
		TEXTURE_TUTORIAL,
		TEXTURE_TUTORIAL_DOOR,
		TEXTURE_TUTORIAL_HACKING,
		TEXTURE_TUTORIAL_ENEMY,
		TEXTURE_TUTORIAL_LOCKED_DOOR,
		TEXTURE_TUTORIAL_STEALTH,
		TEXTURE_TUTORIAL_GUN,
		TEXTURE_TUTORIAL_ROBOT,
		TEXTURE_TUTORIAL_BUTTON,
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
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];			// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
