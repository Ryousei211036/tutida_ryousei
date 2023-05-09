//**********************************************************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**********************************************************************************************

//==============================================================================================
// �C���N���[�h
//==============================================================================================
#include "application.h"
#include "texture.h"
#include "renderer.h"

#include <assert.h>

//==============================================================================================
// ��`
//==============================================================================================
const char* CTexture::s_FileName[] =
{// �e�N�X�`���̃p�X
	"Data/TEXTURE/tile1.jpg",				// ��
	"Data/TEXTURE/number.png",				// ����
	"Data/TEXTURE/title.jpg",				// �^�C�g��
	"Data/TEXTURE/result.jpg",				// ���U���g
	"Data/TEXTURE/skip.png",				// �`���[�g���A���X�L�b�v
	"Data/TEXTURE/Exclamation.png",			// �r�b�N���}�[�N
	"Data/TEXTURE/titlelogo.png",			// �^�C�g�����S
	"Data/TEXTURE/start.jpg",				// �X�^�[�g
	"Data/TEXTURE/tutorial.jpg",			// �`���[�g���A��
	"Data/TEXTURE/tutorial_door.jpg",		// �`���[�g���A��(�h�A)
	"Data/TEXTURE/tutorial_hack.jpg",		// �`���[�g���A��(�n�b�L���O)
	"Data/TEXTURE/tutorial_enemy.jpg",		// �`���[�g���A��(�G)
	"Data/TEXTURE/tutorial_lock_door.jpg",	// �`���[�g���A��(���̂��������h�A)
	"Data/TEXTURE/tutorial_stealth.jpg",	// �`���[�g���A��(�X�e���X)
	"Data/TEXTURE/tutorial_gun.jpg",		// �`���[�g���A��(�e)
	"Data/TEXTURE/tutorial_robot.jpg",		// �`���[�g���A��(���{�b�g)
	"Data/TEXTURE/tutorial_button.jpg",		// �`���[�g���A��(�{�^��)
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//----------------------------------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//----------------------------------------------------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	//ZeroMemory(&s_pTexture[0], sizeof(s_pTexture));
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//----------------------------------------------------------------------------------------------
CTexture::~CTexture()
{
}

//----------------------------------------------------------------------------------------------
// �S�Ă̓ǂݍ���
//----------------------------------------------------------------------------------------------
void CTexture::LoadAll()
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̓ǂݍ��݂�����Ă���
			continue;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//----------------------------------------------------------------------------------------------
// �ǂݍ���
//----------------------------------------------------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// �e�N�X�`���̓ǂݍ��݂�����Ă���
		return;
	}

	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//----------------------------------------------------------------------------------------------
// �S�Ẳ��
//----------------------------------------------------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//----------------------------------------------------------------------------------------------
// ���
//----------------------------------------------------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// �e�N�X�`���̉��
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//----------------------------------------------------------------------------------------------
// �擾
//----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// �e�N�X�`�����g�p���Ȃ�
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// �ǂݍ���
	Load(inTexture);

	return s_pTexture[inTexture];
}
