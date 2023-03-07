//==============================================================================================
//
// ランキング　　　ranking.cpp
// 土田凌聖
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include<stdio.h>
#include"ranking.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		m_pScore[nCnt] = nullptr;
		m_aRankingData[nCnt] = 0;
	}
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//==============================================================================================
// デスラクタ
//==============================================================================================
CRanking::~CRanking()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CRanking::Init()
{
	CScore::Init();

	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		m_pScore[nCnt] = CScore::Create({ m_Pos.x + nCnt * 80, m_Pos.y + nCnt * 80, m_Pos.z }, m_fWidth, m_fHeight, m_Col);
	}
	
	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CRanking::Uninit()
{
	CScore::Uninit();

	for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{
			m_pScore[nCnt]->Uninit();
			m_pScore[nCnt] = nullptr;
		}
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CRanking::Update()
{
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CRanking::Draw()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, float width, float height, D3DXCOLOR col)
{
	CRanking *pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->SetSize(width, height);
		pRanking->SetCol(col);
		pRanking->SetPos(pos);
		pRanking->Init();
	}

	return pRanking;
}

//==============================================================================================
// スコアの設定
//==============================================================================================
void CRanking::SetScore(int score, int cnt)
{
	m_pScore[cnt]->SetScore(score);
}

//==============================================================================================
// スコアを並び変える
//==============================================================================================
void CRanking::CompareScore(int score)
{
	LoadScore();

	int nSave = 0;
	bool bSave = false;

	if (score >= m_aRankingData[NUM_RANKING - 1]
		&& score != 0)
	{
		m_aRankingData[NUM_RANKING - 1] = score;
		bSave = true;
	}

	for (int nCnt = 0; nCnt < (NUM_RANKING - 1); nCnt++)
	{
		for (int nCnt2 = (nCnt + 1); nCnt2 < NUM_RANKING; nCnt2++)
		{
			if (m_aRankingData[nCnt] < m_aRankingData[nCnt2])
			{
				nSave = m_aRankingData[nCnt];
				m_aRankingData[nCnt] = m_aRankingData[nCnt2];
				m_aRankingData[nCnt2] = nSave;
			}
		}
	}

	// 更新したスコアの色を変える
	if (bSave == true)
	{
		for (int nCnt = 0; nCnt < (NUM_RANKING); nCnt++)
		{
			if (score == m_aRankingData[nCnt])
			{
				m_pScore[nCnt]->SetCol({ 1.0f, 1.0f, 0.0f, 1.0f });

				break;
			}
		}
	}

	CRanking::SaveScore();
}

//==============================================================================================
// ロード
//==============================================================================================
void CRanking::LoadScore()
{
	FILE *pFile;

	pFile = fopen("Data/TXT/Ranking/ranking.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
		{
			fscanf(pFile, "%d", &m_aRankingData[nCnt]);
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}

//==============================================================================================
// スコアのセーブ
//==============================================================================================
void CRanking::SaveScore()
{
	FILE *pFile;

	pFile = fopen("Data/TXT/Ranking/ranking.txt", "w");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < NUM_RANKING; nCnt++)
		{
			fprintf(pFile, "%d\n", m_aRankingData[nCnt]);
			SetScore(m_aRankingData[nCnt], nCnt);
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}
