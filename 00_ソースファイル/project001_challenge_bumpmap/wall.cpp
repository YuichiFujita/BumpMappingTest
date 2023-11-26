//============================================================
//
//	壁処理 [wall.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "shaderBumpmap.h"

//************************************************************
//	マクロ定義
//************************************************************
#define WALL_PRIO	(0)	// 壁の優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CWall::mc_apTextureFile[] =	// テクスチャ定数
{
	"data\\TEXTURE\\wall000.png",	// 通常テクスチャ
};

//************************************************************
//	グローバル変数宣言
//************************************************************
namespace
{
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl = NULL;	// 頂点シェーダー
	
	LPDIRECT3DTEXTURE9	m_pNormalMap	= nullptr;	// 法線マップ
	CBumpMap*			m_pBumpMap		= nullptr;	// バンプマップ
	
	D3DXVECTOR4 LightPos	= D3DXVECTOR4(72.0f, 100.0f, 620.0f, 0.0f);	// 太陽の位置ベクトル
	D3DXVECTOR4 LightDir	= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);		// 平行光源の光の方向ベクトル
	D3DXVECTOR4 EyePos		= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);		// 視点の位置ベクトル
}

//************************************************************
//	子クラス [CWall] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, WALL_PRIO)
{
	// メンバ変数をクリア
	m_pEffect = nullptr;	// エフェクトポインタ
}

//============================================================
//	デストラクタ
//============================================================
CWall::~CWall()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CWall::Init(void)
{
	// メンバ変数を初期化
	m_pEffect = nullptr;	// エフェクトポインタ

	// オブジェクトメッシュウォールの初期化
	if (FAILED(CObjectMeshWall::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// バンプマップの生成
	m_pBumpMap = CBumpMap::Create();
	if (m_pBumpMap == nullptr)
	{
		return E_FAIL;
	}

	// 頂点データを定義する(詳細はオンラインマニュアルを参照)
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

#if 0
	//LPD3DXMESH *ppMesh = NULL;	// メッシュ (頂点情報) へのポインタ
	//CModel::SModel model = GetModelData();

	// 新しい頂点情報に基づき頂点データを再生成する
	//GetModelData().pMesh->SetVertexDecl(m_pd3dDevice, decl);
	if (FAILED(SetVertexDecl(CManager::GetInstance()->GetRenderer()->GetDevice(), decl)))
	//if (FAILED(model.pMesh->CloneMesh(model.pMesh->GetOptions(), &decl[0], CManager::GetInstance()->GetRenderer()->GetDevice(), &model.pMesh)))
	{
		return E_FAIL;
	}

	//model.pMesh = *ppMesh;
	//SetModelData(model);
#else
	// バンプマッピング用に頂点シェーダ宣言を作成する
	if (FAILED(CManager::GetInstance()->GetRenderer()->GetDevice()->CreateVertexDeclaration(decl, &m_pDecl)))
	{
		return E_FAIL;
	}
#endif

	// 法線マップをロード
	HRESULT hr = D3DXCreateTextureFromFileEx
	(
		CManager::GetInstance()->GetRenderer()->GetDevice(),
		_T("data\\TEXTURE\\normal2.png"),
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0,
		NULL,
		NULL,
		&m_pNormalMap
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// 平行光源の位置ベクトルから方向ベクトルを計算する
	LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CWall::Uninit(void)
{
	// バンプマップの破棄
	CBumpMap::Release(m_pBumpMap);

	// オブジェクトメッシュウォールの終了
	CObjectMeshWall::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CWall::Update(void)
{
	// オブジェクトメッシュウォールの更新
	CObjectMeshWall::Update();
}

//============================================================
//	描画処理
//============================================================
void CWall::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//LightPos = D3DXVECTOR4(cameraInfo.posV.x, cameraInfo.posV.y, cameraInfo.posV.z, 0.0f);	// 太陽の位置ベクトル
	//EyePos = D3DXVECTOR4(cameraInfo.posV.x, cameraInfo.posV.y, cameraInfo.posV.z, 1.0f);	// 視点の位置ベクトル

	// 平行光源の位置ベクトルから方向ベクトルを計算する
	//LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	// ワールドマトリックス
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// 法線マップをステージ１にセットする
	pDevice->SetTexture(1, m_pNormalMap);

	// 頂点シェーダ宣言をバンプマッピング用に設定する
	pDevice->SetVertexDeclaration(m_pDecl);

	// 描画開始
	m_pBumpMap->Begin();

	// マテリアルを設定
	m_pBumpMap->SetAmbient(1.5f);
	m_pBumpMap->SetSpecular(0.5f);
	m_pBumpMap->SetSpecularPower(1.0f);
	m_pBumpMap->SetMatrix(&mtxWorld, &EyePos, &LightDir);

	// パスを設定
	m_pBumpMap->BeginPass(0);

	// オブジェクトメッシュウォールの描画
	CObjectMeshWall::Draw();

	// ステージ１を初期化する
	pDevice->SetTexture(1, NULL);

	// 描画終了
	m_pBumpMap->EndPass();
	m_pBumpMap->End();
}

//============================================================
//	生成処理
//============================================================
CWall *CWall::Create
(
	const ETexture texture,		// 種類
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot,	// 向き
	const D3DXVECTOR2& rSize,	// 大きさ
	const D3DXCOLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const D3DCULL cull,			// カリング状況
	const bool bLight			// ライティング状況
)
{
	// 変数を宣言
	int nTextureID;	// テクスチャインデックス

	// ポインタを宣言
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CWall *pWall = nullptr;	// 壁生成用

	if (pWall == nullptr)
	{ // 使用されていない場合

		// メモリ確保
		pWall = new CWall;	// 壁
	}
	else { assert(false); return nullptr; }	// 使用中

	if (pWall != nullptr)
	{ // 確保に成功している場合

		// 壁の初期化
		if (FAILED(pWall->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pWall;
			pWall = nullptr;

			// 失敗を返す
			return nullptr;
		}

		// テクスチャを登録
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// テクスチャを割当
		pWall->BindTexture(nTextureID);

		// 位置を設定
		pWall->SetVec3Position(rPos);

		// 向きを設定
		pWall->SetVec3Rotation(rRot);

		// 大きさを設定
		pWall->SetVec2Sizing(rSize);

		// 色を設定
		pWall->SetColor(rCol);

		// カリングを設定
		pWall->SetCulling(cull);

		// ライティングを設定
		pWall->SetLighting(bLight);

		// 分割数を設定
		if (FAILED(pWall->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// メモリ開放
			delete pWall;
			pWall = nullptr;

			// 失敗を返す
			return nullptr;
		}

		// 確保したアドレスを返す
		return pWall;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}
