//============================================================
//
//	ティーポット処理 [teapot.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "teapot.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "shaderBumpmap.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// テクスチャ定数
	{
		"data\\TEXTURE\\normal5.png",		// 法線テクスチャ
	};
	const char *MODEL_FILE[] =		// モデル定数
	{
		"data\\MODEL\\TEAPOT\\teapot005.x",	// ティーポット
	};

	const int PRIORITY = 0;	// ティーポットの優先順位
}

//************************************************************
//	グローバル変数宣言
//************************************************************
namespace
{
	D3DXVECTOR4 LightPos	= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);	// 太陽の位置ベクトル
	D3DXVECTOR4 LightDir	= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);	// 平行光源の光の方向ベクトル
	D3DXVECTOR4 EyePos		= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	// 視点の位置ベクトル
}

//************************************************************
//	子クラス [CTeapot] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, PRIORITY)
{

}

//============================================================
//	デストラクタ
//============================================================
CTeapot::~CTeapot()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTeapot::Init(void)
{
	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルを登録・割当
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

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

	// xファイルモデルの頂点データを変換
	if (FAILED(SetVertexDecl(CManager::GetInstance()->GetRenderer()->GetDevice(), decl)))
	{
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTeapot::Uninit(void)
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CTeapot::Update(void)
{
	// オブジェクトモデルの更新
	CObjectModel::Update();
}

//============================================================
//	描画処理
//============================================================
void CTeapot::Draw(void)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャ情報

	// カメラ情報
	CCamera::SCamera cameraInfo = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN);	// カメラ取得

	// サンプラーステートを設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// ワールドマトリックスを取得
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// バンプマップの取得
	CBumpMap *pBumpMap = CBumpMap::GetInstance();
	assert(pBumpMap != nullptr);	// 未使用

	if (pBumpMap->IsEffectOK())
	{ // エフェクトが使用可能な場合

		// 法線マップをステージ１にセットする
		pDevice->SetTexture(1, pTexture->GetTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_NORMAL])));

		// 描画開始
		pBumpMap->Begin();

		// マテリアルを設定
		pBumpMap->SetAmbient(0.75f);
		pBumpMap->SetSpecular(5.5f);
		pBumpMap->SetSpecularPower(0.25f);
		pBumpMap->SetMatrix(&mtxWorld, &EyePos, &LightDir);

		// 状態変更の伝達
		pBumpMap->CommitChanges();

		// パスを設定
		pBumpMap->BeginPass(0);

		// オブジェクトモデルの描画
		CObjectModel::Draw();

		// 描画終了
		pBumpMap->EndPass();
		pBumpMap->End();

		// ステージ１を初期化する
		pDevice->SetTexture(1, nullptr);
	}
}

//============================================================
//	生成処理
//============================================================
CTeapot *CTeapot::Create
(
	const D3DXVECTOR3& rPos,	// 位置
	const D3DXVECTOR3& rRot		// 向き
)
{
	// ポインタを宣言
	CTeapot *pTeapot = nullptr;	// ティーポット生成用

	if (pTeapot == nullptr)
	{ // 使用されていない場合

		// メモリ確保
		pTeapot = new CTeapot;	// ティーポット
	}
	else { assert(false); return nullptr; }	// 使用中

	if (pTeapot != nullptr)
	{ // 確保に成功している場合

		// ティーポットの初期化
		if (FAILED(pTeapot->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pTeapot;
			pTeapot = nullptr;

			// 失敗を返す
			return nullptr;
		}

		// 位置を設定
		pTeapot->SetVec3Position(rPos);

		// 向きを設定
		pTeapot->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}
