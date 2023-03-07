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
	"Data/Texture/bg.jpg",				// �w�i
	"Data/Texture/title001.jpg",		// �^�C�g�����
	"Data/Texture/result.jpg",			// ���U���g���
	"Data/Texture/PressEnter.png",		// PressEnter
	"Data/Texture/gameUI.png",			// �Q�[����ʂ�UI
	"Data/Texture/RankingUI.png",		// �����L���O��UI
	"Data/Texture/bullet000.png",		// �e
	"Data/Texture/enemybullet000.png",	// �e
	"Data/Texture/tank000.png",			// �v���C���[�O
	"Data/Texture/tank001.png",			// �v���C���[�P
	"Data/Texture/tank002.png",			// �v���C���[�Q
	"Data/Texture/tank003.png",			// �v���C���[�R
	"Data/Texture/tank004.png",			// �v���C���[�S
	"Data/Texture/tank005.png",			// �v���C���[�T
	"Data/Texture/tank006.png",			// �v���C���[�U
	"Data/Texture/tank007.png",			// �v���C���[�V
	"Data/Texture/tank008.png",			// �v���C���[�W
	"Data/Texture/tank009.png",			// �v���C���[�X
	"Data/Texture/evolution000.png",	// �i���P
	"Data/Texture/evolution001.png",	// �i���Q
	"Data/Texture/evolution002.png",	// �i���R
	"Data/Texture/evolution003.png",	// �i���S
	"Data/Texture/evolution004.png",	// �i���T
	"Data/Texture/evolution005.png",	// �i���U
	"Data/Texture/evolution006.png",	// �i���V
	"Data/Texture/evolution007.png",	// �i���W
	"Data/Texture/evolution008.png",	// �i���X
	"Data/Texture/enemy000.png",		// �G
	"Data/Texture/enemy001.png",		// �G�P
	"Data/Texture/enemy002.png",		// �G�Q
	"Data/Texture/enemy003.png",		// �G�R
	"Data/Texture/enemy004.png",		// �G�S
	"Data/Texture/enemy005.png",		// �G�T
	"Data/Texture/enemy006.png",		// �G�U
	"Data/Texture/enemy007.png",		// �G�V
	"Data/Texture/enemy008.png",		// �G�W
	"Data/Texture/enemy009.png",		// �G�X
	"Data/Texture/explosion000.png",	// ����
	"Data/Texture/Extriangle0.png",		// �a1
	"Data/Texture/Exbox0.png",			// �a2
	"Data/Texture/Expentagon0.png",		// �a3
	"Data/Texture/number000.png",		// ����
	"Data/Texture/status.png",			// �X�e�[�^�X
	"Data/Texture/max.png",				// ���x��MAX
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//----------------------------------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//----------------------------------------------------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
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
