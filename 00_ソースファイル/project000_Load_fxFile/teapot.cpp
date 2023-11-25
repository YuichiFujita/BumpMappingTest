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

//************************************************************
//	マクロ定義
//************************************************************
#define TEAPOT_PRIO	(0)	// ティーポットの優先順位

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const char *CTeapot::mc_apModelFile[] =	// モデル定数
{
	"data\\MODEL\\TEAPOT\\teapot000.x",	// ティーポット
};

//************************************************************
//	子クラス [CTeapot] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, TEAPOT_PRIO)
{
	// メンバ変数をクリア
	m_pEffect = nullptr;	// エフェクトポインタ
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
	// メンバ変数を初期化
	m_pEffect = nullptr;	// エフェクトポインタ

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// エフェクトファイルの読み込み
	D3DXCreateEffectFromFile
	( // 引数
		CManager::GetInstance()->GetRenderer()->GetDevice(),	// デバイスへのポインタ
		_T("first_create_mat.fx"),	// エフェクトファイル
		nullptr,	// プリプロセッサ定義
		nullptr,	// インクルード操作
		0,			// 読込オプションフラグ
		nullptr,	// グローバル変数インターフェース
		&m_pEffect,	// エフェクトインターフェース
		nullptr		// コンパイルエラー情報
	);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTeapot::Uninit(void)
{
	// エフェクトの破棄
	SAFE_RELEASE(m_pEffect);

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
	// カメラ情報
	CCamera::SCamera cameraInfo = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN);	// カメラ取得

	// ビュー変換・射影変換
	D3DXMATRIX View = cameraInfo.mtxView;
	D3DXMATRIX Projection = cameraInfo.mtxProjection;

	// ワールドマトリックス・ワールドビュー射影変換行列
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXMATRIX mtxWorldViewProj;

	// エフェクト内のワールドビュー射影変換行列を設定
	mtxWorldViewProj = mtxWorld * View * Projection;
	m_pEffect->SetMatrix("mtxWorldViewProj", &mtxWorldViewProj);

	// 描画開始
	m_pEffect->SetTechnique("BasicTec");
	UINT numPass;
	m_pEffect->Begin(&numPass, 0);
	m_pEffect->BeginPass(0);

	// オブジェクトモデルの描画
	CObjectModel::Draw();

	// 描画終了
	m_pEffect->EndPass();
	m_pEffect->End();
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

		// モデルを登録・割当
		pTeapot->BindModel(mc_apModelFile[MODEL_TEAPOT]);

		// 位置を設定
		pTeapot->SetVec3Position(rPos);

		// 向きを設定
		pTeapot->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}
