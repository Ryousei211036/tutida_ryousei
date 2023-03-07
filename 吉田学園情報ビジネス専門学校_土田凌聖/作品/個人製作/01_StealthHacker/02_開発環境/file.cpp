//==============================================================================================
//
// ファイル　　　file.cpp
// 土田凌聖
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
// コンストラクタ
//==============================================================================================
CFile::CFile()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CFile::~CFile()
{
}

//==============================================================================================
// キャラクターの情報の読み込み
//==============================================================================================
CCharManager **CFile::LoadCharFile(CFile::TextName index, D3DXVECTOR3 pos)
{
	// ファイルポインタ
	FILE *pFile;

	// ファイル名
	pFile = fopen(m_FileName[index], "r");

	// キャラクターセット用
	D3DXVECTOR3 Pos;		// 位置
	D3DXVECTOR3 Rot;		// 角度
	int nNumEnemy;			// 敵の数
	int nIndex = 0;			// 番号
	int nParentIndex;		// 親の番号
	int nPartsIndex = 0;	// パーツの番号
	char XFileName[CObjectX::m_nMax_Model][0xff] = {};	// Xファイル名

	// モーション用
	D3DXVECTOR3 PartsPos;	// 位置
	D3DXVECTOR3 PartsRot;	// 回転
	int nFrame;				// キーのフレーム数
	int nMaxKey;			// キーの数
	int nMaxParts;			// パーツの数
	bool bLoop = false;			// ループするか
	int nKey = 0;			// 現在のキーの数
	int nParts = 0;			// 現在のパーツ
	int nNumMotion = 0;		// 現在のモーション

	memset(m_aString, 0, sizeof(m_aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// ファイル名
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ＝を読み飛ばす
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// キャラクターの設定
					if (strcmp(&m_aString[0], "CHARACTERSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_CHARACTERSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// 敵の数
							if (strcmp(&m_aString[0], "NUM_ENEMY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nNumEnemy);
								CEnemy::SetNumEnemy(nNumEnemy);
							}

							// プレイヤーの生成
							if (index == CFile::TXTNAME_PLAYER && m_pPlayer == nullptr)
							{
								if (strcmp(&m_aString[0], "POS") == 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);
									fscanf(pFile, "%f", &Pos.x);
									fscanf(pFile, "%f", &Pos.y);
									fscanf(pFile, "%f", &Pos.z);
								}

								// クリエイト
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

							// ロボットの生成
							if (index == CFile::TXTNAME_ROBOT && m_pRobot == nullptr)
							{
								if (strcmp(&m_aString[0], "POS") == 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);
									fscanf(pFile, "%f", &Pos.x);
									fscanf(pFile, "%f", &Pos.y);
									fscanf(pFile, "%f", &Pos.z);
								}

								// クリエイト
								m_pRobot = CRobot::Create(pos);
							}

							// 敵の生成
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

									// クリエイト
									m_pEnemy[nCnt] = CEnemy::Create(Pos, Rot);

									// 次のPOSを読み込むまで
									while (strcmp(&m_aString[0], "POS") != 0)
									{
										if (nCnt == nNumEnemy - 1)
										{
											break;
										}

										// POSを読み込むまで
										fscanf(pFile, "%s", &m_aString[0]);
									}
								}
							}

							if (strcmp(&m_aString[0], "PARTSSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_PARTSSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// 番号
									if (strcmp(&m_aString[0], "INDEX") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nPartsIndex);
									}

									// 親の番号
									if (strcmp(&m_aString[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nParentIndex);
									}

									// パーツの位置
									if (strcmp(&m_aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &Pos.x);
										fscanf(pFile, "%f", &Pos.y);
										fscanf(pFile, "%f", &Pos.z);
									}

									// パーツの向き
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
									// モデルパーツの設定
									m_pPlayer->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_PLAYER);
									m_pPlayer->Init();
								}

								if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
								{
									// モデルパーツの設定
									m_pRobot->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_ROBOT);
									m_pRobot->Init();
								}

								for (int nCnt = 0; nCnt < nNumEnemy; nCnt++)
								{
									if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL) 
										&& m_pEnemy[nCnt] != nullptr)
									{
										// モデルパーツの設定
										m_pEnemy[nCnt]->SetModel(nPartsIndex, nParentIndex, Pos, Rot, &XFileName[nPartsIndex][0], CModel::MODEL_TYPE_ENEMY);
										m_pEnemy[nCnt]->Init();
									}
								}
							}
						}
					}

					// モーションの設定
					// パーツの数
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

							// ループするか
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								if (nLoop == 1)
								{
									// ループする
									bLoop = true;
								}
								else
								{
									// ループしない
									bLoop = false;
								}
							}

							// キーの数
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

									// キーフレーム
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

											// 位置
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsPos.x);
												fscanf(pFile, "%f", &PartsPos.y);
												fscanf(pFile, "%f", &PartsPos.z);
											}

											// 角度
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
													// モーション値の設定
													m_pPlayer->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);
												}

												if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
												{
													// モーション値の設定
													m_pRobot->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);
												}

												for (int nCnt = 0; nCnt < nNumEnemy; nCnt++)
												{
													if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL)
														&& m_pEnemy[nCnt] != nullptr)
													{
														// モーション値の設定
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

		//ファイルを閉じる
		fclose(pFile);
	}

	if (index == CFile::TXTNAME_PLAYER && m_pPlayer != nullptr)
	{// プレイヤーの情報を返す
		return &m_pPlayer;
	}
	else if (index == CFile::TXTNAME_ROBOT && m_pRobot != nullptr)
	{// ロボットの情報を返す
		return &m_pRobot;
	}
	else if ((index == CFile::TXTNAME_ENEMY || index == CFile::TXTNAME_ENEMY_TUTORIAL)
		&& m_pEnemy[0] != nullptr)
	{// 敵の情報を返す
		return &m_pEnemy[0];
	}

	return nullptr;
}

//==============================================================================================
// モデルの読み込み
//==============================================================================================
CObjectX **CFile::LoadModel()
{
	FILE *pFile = nullptr;		// ファイルポインタを宣言
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;
	int nModelType;
	int nObjectType;
	int Index = 0;
	char XFileName[CObjectX::m_nMax_Model][0xff] = {};	// Xファイル名
	int nCount = 0;
	memset(m_aString, 0, sizeof(m_aString));

	// ファイルを開く
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

	//ファイルが開いた場合
	if (pFile != nullptr)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			//SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// ファイル名
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

							// 位置
							if (strcmp(&m_aString[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Pos.x);
								fscanf(pFile, "%f", &Pos.y);
								fscanf(pFile, "%f", &Pos.z);
							}

							// 角度
							if (strcmp(&m_aString[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Rot.x);
								fscanf(pFile, "%f", &Rot.y);
								fscanf(pFile, "%f", &Rot.z);
							}

							// サイズ
							if (strcmp(&m_aString[0], "SCALE") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%f", &Scale.x);
								fscanf(pFile, "%f", &Scale.y);
								fscanf(pFile, "%f", &Scale.z);
							}

							// モデルのタイプ
							if (strcmp(&m_aString[0], "MODELTYPE") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nModelType);
							}

							// 処理のタイプ
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

		//ファイルを閉じる
		fclose(pFile);
	}

	if (m_pObjectX[0] != nullptr)
	{
		return &m_pObjectX[0];
	}

	return nullptr;
}