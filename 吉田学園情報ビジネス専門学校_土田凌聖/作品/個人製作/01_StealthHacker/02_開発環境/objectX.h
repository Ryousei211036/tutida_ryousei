//==============================================================================================
//
//モデルの処理[model.h]
//Author：土田凌聖
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include"Object.h"
#include"renderer.h"
#include"model.h"
#include"texture.h"

class CGage;

class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 3);
	~CObjectX();

	// 処理のタイプ
	enum ObjectX_Type
	{
		OBJECTX_WALL = 0,	// 壁
		OBJECTX_CHASE_WALL,	// 敵が追いかけてくる壁
		OBJECTX_ROBOT_WALL,	// ロボットが通れる壁
		OBJECTX_KEY_RED,	// 鍵(赤)
		OBJECTX_KEY_BLUE,	// 鍵(青)
		OBJECTX_KEY_GREEN,	// 鍵(緑)
		OBJECTX_DOOR_RED,	// ドア(赤)
		OBJECTX_DOOR_BLUE,	// ドア(青)
		OBJECTX_DOOR_GREEN,	// ドア(緑)
		OBJECTX_DOOR,		// ドア(鍵なし)
		OBJECTX_DOORWALL,	// ドアの上の壁
		OBJECTX_PC,			// パソコン
		OBJECTX_BUTTON,		// ボタン
		OBJECTX_GOAL,		// ゴール
		OBJECTX_CEILING,	// 天井
		OBJECTX_BULLET,		// 銃弾
		OBJECT_MAX,
		OBJECTX_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void DrawShadow();		// 影
	void Stencil();			// ステンシル
	bool CharCollision(CModel::EModel_Type type);	// キャラの当たり判定
	bool Collision(D3DXVECTOR3 pos);			// オブジェクトの当たり判定
	void CollisionType();	// タイプごとの判定
	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* name, int modeltype, ObjectX_Type type);
	void ObjectXCol();		// 色の設定
	void DoorMove();		// ドアの動き
	bool OpenDoor();		// ドアが空いているか

	void SetXFileName(const char *FileName) { m_FileName = FileName; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetVecAxis(D3DXVECTOR3 vecAxis) { m_vecAxis = vecAxis; }
	void SetValue(float Value) { m_fValue = Value; }
	void SetType(ObjectX_Type type) { m_nObjectType = type; }
	void SetModelType(int modeltype) { m_nModelType = modeltype; }
	static void SetNumObject(int num) { m_nNumObject = num; }
	void SetAlpha(float alpha) { m_Alpha = alpha; }

	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetMax() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMin() { return m_vtxMinModel; }
	ObjectX_Type GetObjectXType() { return m_nObjectType; }
	int GetModelType() { return m_nModelType; }
	static int GetNumObject() { return m_nNumObject; }
	static bool GetGoalOpen() { return m_bGoalOpen; }

	static const int m_nMax_Model = 128;	// オブジェクトXの最大数
private:
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_posOld;			// 前回の位置
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_Initrot;			// 最初の向き
	D3DXVECTOR3 m_Size;				// サイズ
	D3DXVECTOR3 m_vtxMinModel;		// 頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;		// 頂点の最大値
	D3DXVECTOR3 m_vecDir;			// プレイヤーの方向ベクトル
	LPD3DXBUFFER m_pBuffMat;		// マテリアル情報へのポインタ
	LPD3DXMESH m_pMesh;				// メッシュ情報へのポインタ
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	DWORD m_nNumMat;				// マテリアル情報の数
	D3DXMATRIX m_mtxRot;			// 回転マトリックス
	D3DXVECTOR3 m_vecAxis;			// 回転軸
	ObjectX_Type m_nObjectType;		// タイプ
	int m_nModelType;				// モデルのタイプ
	int m_nGoalColCount;			// ゴールの色が変わるまでの時間
	static int m_nNumObject;		// オブジェクトの数
	static int m_nNumHack;			// ハッキング回数
	float m_fRadius;				// オブジェクトの半径
	float m_fAngle;					// プレイヤーと仲間の角度
	float m_fDoorSpeed;				// ドアが開くスピード
	float m_fValue;					// 回転角
	float m_Alpha;					// アルファ値
	bool m_bAction;					// アクションできるか
	bool m_bGageFull_Up;			// ゲージが満タンになったか
	bool m_bZeroAlpha;				// ゲージのアルファ値が0になったか
	bool m_bHack;					// ハッキングしたか
	bool m_bButton_Player;			// プレイヤーがボタンを押した
	bool m_bButton_Robot;			// ロボットがボタンを押した
	static bool m_bGoalOpen;		// ゴールが開いたか
	const char *m_FileName;			// ファイルの名前
	CGage *m_pGage;					// ゲージの情報
	CTexture::TEXTURE m_texture;	// テクスチャ
	D3DXCOLOR m_Col;				// 色

	static const int m_nNumDoor = 4;	// ドアの種類の数
	
	bool m_bOpenDoor[m_nNumDoor];					// ドアが開くか
	static bool m_bGetKey[m_nNumDoor - 1];			// 鍵を取ったか
	static bool m_bGetKey_Robot[m_nNumDoor - 1];	// ロボットが鍵を取ったか
};

#endif // !_OBJECTX_H_