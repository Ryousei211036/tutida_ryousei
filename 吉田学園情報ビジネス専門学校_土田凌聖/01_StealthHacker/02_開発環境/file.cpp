//==============================================================================================
//
// �t�@�C���@�@�@file.cpp
// �y�c����
//
//==============================================================================================
#include"file.h"
#include"renderer.h"
#include"game.h"
#include"player.h"
#include"objectX.h"
#include"enemy.h"
#include"robot.h"
#include"application.h"

const char *CFile::m_FileName[] =
{
	"Data/TXT/Character/playerset.txt",
	"Data/TXT/Character/robotset.txt",
	"Data/TXT/Character/enemyset.txt",
	"Data/TXT/Character/enemyset tutorial.txt",
};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CFile::CFile()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CFile::~CFile()
{
}

//==============================================================================================
// �L�����N�^�[�̏��̓ǂݍ���
//==============================================================================================
CCharManager **CFile::LoadCharFile(CFile::TextName index, D3DXVECTOR3 pos)
{
	// �t�@�C���|�C���^
	FILE *pFile;

	// �t�@�C����
	pFile = fopen(m_FileName[index], "r");

	// �L�����N�^�[�Z�b�g�p
	D3DXVECTOR3 Pos;		// �ʒu
	D3DXVECTOR3 Rot;		// �p�x
	int nNumEnemy;			// �G�̐�
	int nIndex = 0;			// �ԍ�
	int nParentIndex;		// �e�̔ԍ�
	int nPartsIndex = 0;	// �p�[�c�̔ԍ�
	char XFileName[CObjectX::m_nMax_Model][0xff] = {};	// X�t�@�C����

	// ���[�V�����p
	D3DXVECTOR3 PartsPos;	// �ʒu
	D3DXVECTOR3 PartsRot;	// ��]
	int nFrame;				// �L�[�̃t���[����
	int nMaxKey;			// �L�[�̐�
	int nMaxParts;			// �p�[�c�̐�
	bool bLoop = false;			// ���[�v���邩
	int nKey = 0;			// ���݂̃L�[�̐�
	int nParts = 0;			// ���݂̃p�[�c
	int nNumMotion = 0;		// ���݂̃��[�V����

	memset(m_aString, 0, sizeof(m_aString));

	// �t�@�C�����J�����ꍇ
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPT����v
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// �t�@�C����
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ����ǂݔ�΂�
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// �L�����N�^�[�̐ݒ�
					if (strcmp(&m_aString[0], "CHARACTERSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_CHARACTERSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// �G�̐�
							if (strcmp(&m_aString[0], "NUM_ENEMY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nNumEnemy);
								CEnemy::SetNumEnemy(nNumEnemy);
							}

							// �v���C���[�̐���
							if (index == CFile::TXTNAME_PLAYER && m_pPlayer == nullptr)
							{
								if (strcmp(&m_aString[0], "POS") == 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);
									fscanf(pFile, "%f", &Pos.x);
									fscanf(pFile, "%f", &Pos.y);
									fscanf(pFile, "%f", &Pos.z);
								}

								// �N���G�C�g
								if (CApplication::GetMode() == CApplication::MODE_GAME
									|| CApplication::GetMode() == CApplication::MODE_TUTORIAL)
								{
									m_pPlayer = CPlayer::Create(Pos);
								}
								if (CApplication::GetMode() == CApplication::MODE_TITLE)
								{
									m_pPlayer = CPlayer::Create({ -30.0f, 0.0f, -50.0f });
								}
							}

							// ���{�b�g�̐���
							if (index == CFile::TXTNAME_ROBOT && m_pRobot == nullptr)
							{
								if (strcmp(&m_aString[0], "POS") == 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);
									fscanf(pFile, "%f", &Pos.x);
									fscanf(pFile, "%f", &Pos.y);
									fscanf(pFile, "%f", &Pos.z);
								}

								// �N���G�C�g
								m_pRobot = CRobot::Create(pos);
							}

							// �G�̐���
							if (strcmp(&m_aString[0], "POS") == 0)
							{
								for (int nCnt = 0; nCnt < nNumEnemy; nCnt++)
								{
									if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL)
										&& m_pEnemy[nCnt] == nullptr)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &Pos.x);
										fscanf(pFile, "%f", &Pos.y);
										fscanf(pFile, "%f", &Pos.z);


										while (strcmp(&m_aString[0], "ROT") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &Rot.x);
												fscanf(pFile, "%f", &Rot.y);
												fscanf(pFile, "%f", &Rot.z);

												break;
											}
										}
									}

									// �N���G�C�g
									m_pEnemy[nCnt] = CEnemy::Create(Pos, Rot);

									// ����POS��ǂݍ��ނ܂�
									while (strcmp(&m_aString[0], "POS") != 0)
									{
										if (nCnt == nNumEnemy - 1)
										{
											break;
										}

										// POS��ǂݍ��ނ܂�
										fscanf(pFile, "%s", &m_aString[0]);
									}
								}
							}

							if (strcmp(&m_aString[0], "PARTSSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_PARTSSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// �ԍ�
									if (strcmp(&m_aString[0], "INDEX") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nPartsIndex);
									}

									// �e�̔ԍ�
									if (strcmp(&m_aString[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nParentIndex);
									}

									// �p�[�c�̈ʒu
									if (strcmp(&m_aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &Pos.x);
										fscanf(pFile, "%f", &Pos.y);
										fscanf(pFile, "%f", &Pos.z);
									}

									// �p�[�c�̌���
									if (strcmp(&m_aString[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &Rot.x);
										fscanf(pFile, "%f", &Rot.y);
										fscanf(pFile, "%f", &Rot.z);
									}
								}

								if (index == CFile::TXTNAME_PLAYER && m_pPlayer != nullptr)
								{
									// ���f���p�[�c�̐ݒ�
									m_pPlayer->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_PLAYER);
									m_pPlayer->Init();
								}

								if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
								{
									// ���f���p�[�c�̐ݒ�
									m_pRobot->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_ROBOT);
									m_pRobot->Init();
								}

								for (int nCnt = 0; nCnt < nNumEnemy; nCnt++)
								{
									if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL) 
										&& m_pEnemy[nCnt] != nullptr)
									{
										// ���f���p�[�c�̐ݒ�
										m_pEnemy[nCnt]->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_ENEMY);
										m_pEnemy[nCnt]->Init();
									}
								}
							}
						}
					}

					// ���[�V�����̐ݒ�
					// �p�[�c�̐�
					if (strcmp(&m_aString[0], "NUMPARTS") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nMaxParts);
					}

					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ���[�v���邩
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								if (nLoop == 1)
								{
									// ���[�v����
									bLoop = true;
								}
								else
								{
									// ���[�v���Ȃ�
									bLoop = false;
								}
							}

							// �L�[�̐�
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_KEYSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// �L�[�t���[��
									if (strcmp(&m_aString[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nFrame);
									}

									if (strcmp(&m_aString[0], "KEY") == 0)
									{
										while (strcmp(&m_aString[0], "END_KEY") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											// �ʒu
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsPos.x);
												fscanf(pFile, "%f", &PartsPos.y);
												fscanf(pFile, "%f", &PartsPos.z);
											}

											// �p�x
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsRot.x);
												fscanf(pFile, "%f", &PartsRot.y);
												fscanf(pFile, "%f", &PartsRot.z);

											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												if (index == CFile::TXTNAME_PLAYER && m_pPlayer != nullptr)
												{
													// ���[�V�����l�̐ݒ�
													m_pPlayer->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);
												}

												if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
												{
													// ���[�V�����l�̐ݒ�
													m_pRobot->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);
												}

												for (int nCnt = 0; nCnt < nNumEnemy; nCnt++)
												{
													if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL)
														&& m_pEnemy[nCnt] != nullptr)
													{
														// ���[�V�����l�̐ݒ�
														m_pEnemy[nCnt]->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);
													}
												}

												nParts++;

												if (nParts == nMaxParts)
												{
													nParts = 0;
												}
											}
										}
									}
								}

								nKey++;

								if (nKey == nMaxKey)
								{
									nKey = 0;
								}
							}
						}

						nNumMotion++;
					}
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	if (index == CFile::TXTNAME_PLAYER && m_pPlayer != nullptr)
	{// �v���C���[�̏���Ԃ�
		return &m_pPlayer;
	}
	else if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
	{// ���{�b�g�̏���Ԃ�
		return &m_pRobot;
	}
	else if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL)
		&& m_pEnemy[0] != nullptr)
	{// �G�̏���Ԃ�
		return &m_pEnemy[0];
	}

	return nullptr;
}

//==============================================================================================
// ���f���̓ǂݍ���
//==============================================================================================
CObjectX **CFile::LoadModel()
{
	FILE *pFile = nullptr;		// �t�@�C���|�C���^��錾
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;
	int nModelType;
	int nObjectType;
	int Index = 0;
	char XFileName[CObjectX::m_nMax_Model][0xff] = {};	// X�t�@�C����
	int nCount = 0;
	memset(m_aString, 0, sizeof(m_aString));

	// �t�@�C�����J��
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		pFile = fopen("Data/TXT/Map/setmap.txt", "r");
	}
	if (CApplication::GetMode() == CApplication::MODE_TITLE)
	{
		pFile = fopen("Data/TXT/Map/settitle.txt", "r");
	}
	if (CApplication::GetMode() == CApplication::MODE_TUTORIAL)
	{
		pFile = fopen("Data/TXT/Map/settutorial.txt", "r");
	}

	//�t�@�C�����J�����ꍇ
	if (pFile != nullptr)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			//SCRIPT����v
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// �t�@�C����
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[Index][0]);
						Index++;
					}

					if (strcmp(&m_aString[0], "MODELSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_MODELSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// �ʒu
							if (strcmp(&m_aString[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Pos.x);
								fscanf(pFile, "%f", &Pos.y);
								fscanf(pFile, "%f", &Pos.z);
							}

							// �p�x
							if (strcmp(&m_aString[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Rot.x);
								fscanf(pFile, "%f", &Rot.y);
								fscanf(pFile, "%f", &Rot.z);
							}

							// �T�C�Y
							if (strcmp(&m_aString[0], "SCALE") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Scale.x);
								fscanf(pFile, "%f", &Scale.y);
								fscanf(pFile, "%f", &Scale.z);
							}

							// ���f���̃^�C�v
							if (strcmp(&m_aString[0], "MODELTYPE") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nModelType);
							}

							// �����̃^�C�v
							if (strcmp(&m_aString[0], "OBJTYPE") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nObjectType);
							}
						}

						m_pObjectX[nCount] = CObjectX::Create(Pos, Rot, Scale, &XFileName[nModelType][0], nModelType, (CObjectX::ObjectX_Type)nObjectType);
						nCount++;
					}
				}

				CObjectX::SetNumObject(nCount);
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	if (m_pObjectX[0] != nullptr)
	{
		return &m_pObjectX[0];
	}

	return nullptr;
}