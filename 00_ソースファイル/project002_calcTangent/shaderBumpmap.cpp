//============================================================
//
//	バンプマッピング処理 [shaderBumpmap.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shaderBumpmap.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderBumpmap.fx";	// バンプマップのエフェクトファイル
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CBumpMap *CBumpMap::m_pBumpMap = nullptr;	// バンプマッピング

//************************************************************
//	親クラス [CBumpMap] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBumpMap::CBumpMap()
{
	// メンバ変数をクリア
	m_pEffect = nullptr;	// エフェクトポインタ
}

//============================================================
//	デストラクタ
//============================================================
CBumpMap::~CBumpMap()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBumpMap::Init(void)
{
	// 変数を宣言
	D3DCAPS9 caps;	// ハードウェア機能
	HRESULT hr;		// 異常終了の確認用

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	LPD3DXBUFFER pError = nullptr;	// コンパイルエラー情報

	// メンバ変数を初期化
	m_pEffect = nullptr;	// エフェクトポインタ

	// ハードウェア機能を取得
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // 頂点・ピクセルシェーダのバージョンが使用可能な場合

		// エフェクトファイルの読込
		hr = D3DXCreateEffectFromFile
		( // 引数
			pDevice,	// デバイスへのポインタ
			EFFECT_FX,	// エフェクトファイル
			nullptr,	// プリプロセッサ定義
			nullptr,	// インクルード操作
			0,			// 読込オプションフラグ
			nullptr,	// グローバル変数インターフェース
			&m_pEffect,	// エフェクトインターフェース
			&pError		// コンパイルエラー情報
		);
		if (SUCCEEDED(hr))
		{ // 読込に成功した場合

			// テクニック関数を取得
			m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
			m_pEffect->SetTechnique(m_pTechnique);	// エフェクトにテクニック関数を設定

			// グローバル変数を取得
			m_pMtxWorldViewProj	= m_pEffect->GetParameterByName(nullptr, "m_mtxWorldViewProj");	// ワールドビュー射影変換マトリックス
			m_pLightDir			= m_pEffect->GetParameterByName(nullptr, "m_LightDir");			// 平行光源の方向ベクトル
			m_pEyePos			= m_pEffect->GetParameterByName(nullptr, "m_EyePos");			// 視点位置ベクトル
			m_pAmbient			= m_pEffect->GetParameterByName(nullptr, "m_Ambient");			// 環境光
			m_pSpecular			= m_pEffect->GetParameterByName(nullptr, "m_Specular");			// ハイライトの範囲
			m_pSpecularPower	= m_pEffect->GetParameterByName(nullptr, "m_SpecularPower");	// ハイライトの強度
		}
		else
		{ // 読込に失敗した場合

			// 読み込み失敗を返す
			return E_FAIL;
		}
	}
	else
	{ // バージョンが使用不可な場合

		// 古いバージョンによる失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBumpMap::Uninit(void)
{
	// エフェクトの破棄
	SAFE_RELEASE(m_pEffect);
}

//============================================================
//	開始処理
//============================================================
void CBumpMap::Begin(void)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 開始
		m_pEffect->Begin(nullptr, 0);
	}
}

//============================================================
//	パス開始処理
//============================================================
void CBumpMap::BeginPass(const BYTE pass)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// パスの開始
		m_pEffect->BeginPass(pass);
	}
}

//============================================================
//	パス終了処理
//============================================================
void CBumpMap::EndPass(void)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// パスの終了
		m_pEffect->EndPass();
	}
}

//============================================================
//	終了処理
//============================================================
void CBumpMap::End(void)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 終了
		m_pEffect->End();
	}
}

//============================================================
//	環境光の設定処理
//============================================================
void CBumpMap::SetAmbient(float Ambient)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 変数を宣言
		D3DXVECTOR4 ambient = D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f);	// 環境光

		// エフェクトに環境光を設定
		m_pEffect->SetVector(m_pAmbient, &ambient);
	}
}

//============================================================
//	反射光の設定処理
//============================================================
void CBumpMap::SetSpecular(const float fSpecular)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// エフェクトに反射光を設定
		m_pEffect->SetFloat(m_pSpecular, fSpecular);
	}
}

//============================================================
//	反射ハイライトの設定処理
//============================================================
void CBumpMap::SetSpecularPower(const float fPower)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// エフェクトに反射ハイライトを設定
		m_pEffect->SetFloat(m_pSpecularPower, fPower);
	}
}

//============================================================
//	マトリックスの設定処理
//============================================================
void CBumpMap::SetMatrix
(
	D3DXMATRIX* pMtxWorld,		// ワールドマトリックス
	D3DXVECTOR4* pPosCamera,	// カメラ位置
	D3DXVECTOR4* pDirLight		// ライト方向
)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 変数を宣言
		D3DXMATRIX mtxView;			// ビューマトリックス
		D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX mtxWorld;		// ワールドマトリックス
		D3DXMATRIX mtxWorldView;	// ワールドビューマトリックス
		D3DXMATRIX mtxWVP;			// ワールドビュー射影変換マトリックス
		D3DXVECTOR4 dirLight;		// 平行光源の方向ベクトル
		D3DXVECTOR4 posEye;			// 視点位置ベクトル

		// ポインタを宣言
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

		// ビューマトリックス・プロジェクションマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

		// ワールドビュー射影変換マトリックスを計算
		mtxWVP = (*pMtxWorld) * mtxView * mtxProjection;

		// エフェクトにワールドビュー射影変換マトリックスを設定
		m_pEffect->SetMatrix(m_pMtxWorldViewProj, &mtxWVP);

		// カメラの視点位置ベクトルを計算
		mtxWorldView = (*pMtxWorld) * mtxView;						// ワールドビューマトリックスを計算
		D3DXMatrixInverse(&mtxWorldView, nullptr, &mtxWorldView);	// ワールドビューマトリックスの逆行列を計算
		D3DXVec4Transform(&posEye, pPosCamera, &mtxWorldView);		// マトリックスをベクトルに変換

		// エフェクトに視点位置ベクトルを設定
		m_pEffect->SetVector(m_pEyePos, &posEye);

		// 平行光源の方向ベクトルを計算
		dirLight = *pDirLight;
		D3DXMatrixInverse(&mtxWorld, nullptr, pMtxWorld);						// ワールドマトリックスの逆行列を計算
		D3DXVec4Transform(&dirLight, &dirLight, &mtxWorld);						// マトリックスをベクトルに変換
		D3DXVec3Normalize((D3DXVECTOR3*)&dirLight, (D3DXVECTOR3*)&dirLight);	// ベクトルを三次元変換し正規化

		// エフェクトに平行光源の方向ベクトルを設定
		m_pEffect->SetVector(m_pLightDir, &dirLight);
	}
}

//============================================================
//	状態変更の伝達処理
//============================================================
void CBumpMap::CommitChanges(void)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 状態変更の伝達
		m_pEffect->CommitChanges();
	}
}

//============================================================
//	エフェクト使用可能状況の取得処理
//============================================================
bool CBumpMap::IsEffectOK(void) const
{
	// エフェクトポインタの使用状況を返す
	return (m_pEffect != nullptr) ? true : false;
}

//============================================================
//	エフェクトポインタ取得処理
//============================================================
LPD3DXEFFECT CBumpMap::GetEffect(void) const
{
	// エフェクトポインタを返す
	return m_pEffect;
}

//============================================================
//	生成処理
//============================================================
CBumpMap *CBumpMap::Create(void)
{
	// 例外処理
	assert(m_pBumpMap == nullptr);	// インスタンス使用中

	// バンプマッピングの生成
	m_pBumpMap = new CBumpMap;	// バンプマッピングの情報
	if (m_pBumpMap == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// バンプマッピングの初期化
		if (FAILED(m_pBumpMap->Init()))
		{ // 初期化に失敗した場合

			// バンプマッピングの破棄
			delete m_pBumpMap;
			m_pBumpMap = nullptr;

			assert(false);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pBumpMap;
	}
}

//============================================================
//	取得処理
//============================================================
CBumpMap *CBumpMap::GetInstance(void)
{
	// 例外処理
	assert(m_pBumpMap != nullptr);	// インスタンス未使用

	// バンプマッピングのポインタを返す
	return m_pBumpMap;
}

//============================================================
//	破棄処理
//============================================================
HRESULT CBumpMap::Release(void)
{
	if (m_pBumpMap != nullptr)
	{ // バンプマッピングが使用中の場合

		// バンプマッピングの終了
		m_pBumpMap->Uninit();

		// メモリ開放
		delete m_pBumpMap;
		m_pBumpMap = nullptr;

		// 成功を返す
		return S_OK;
	}

	// 失敗を返す
	assert(false);	// 破棄済み
	return E_FAIL;
}
