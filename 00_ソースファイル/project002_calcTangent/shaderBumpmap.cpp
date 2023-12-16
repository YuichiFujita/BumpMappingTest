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
	D3DCAPS9 caps;
	HRESULT hr;

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報
	LPD3DXBUFFER pErr = nullptr;	// コンパイルエラー情報

	// メンバ変数を初期化
	m_pEffect = nullptr;	// エフェクトポインタ

	// 
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{ // 頂点シェーダーとピクセルシェーダのバージョンが今回使用するバージョン以上の場合

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
			&pErr		// コンパイルエラー情報
		);
		if (SUCCEEDED(hr))
		{ // 読込に成功した場合

			// 情報を初期化
			m_pTechnique		= m_pEffect->GetTechniqueByName("TShader");
			m_mtxWorldViewProj	= m_pEffect->GetParameterByName(nullptr, "m_mtxWorldViewProj");
			m_pLightDir			= m_pEffect->GetParameterByName(nullptr, "m_LightDir");
			m_pEyePos			= m_pEffect->GetParameterByName(nullptr, "m_EyePos");
			m_pAmbient			= m_pEffect->GetParameterByName(nullptr, "m_Ambient");
			m_pSpecularPower	= m_pEffect->GetParameterByName(nullptr, "m_SpecularPower");
			m_pSpecular			= m_pEffect->GetParameterByName(nullptr, "m_Specular");

			m_pEffect->SetTechnique(m_pTechnique);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -2;
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
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProjection);

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
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		D3DXVECTOR4 A;
		A = D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f);
		m_pEffect->SetVector(m_pAmbient, &A);
	}
	else
	{ // エフェクトが使用されていない場合

		D3DMATERIAL9 tempMat;
		pDevice->GetMaterial(&tempMat);
		tempMat.Ambient.r = Ambient;
		tempMat.Ambient.g = Ambient;
		tempMat.Ambient.b = Ambient;
		tempMat.Ambient.a = 1.0f;
		pDevice->SetMaterial(&tempMat);
	}
}

//============================================================
//	反射光の設定処理
//============================================================
void CBumpMap::SetSpecular(const float fSpecular)
{
	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		// 反射光の設定
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

		// 反射ハイライトの設定
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
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス情報

	if (m_pEffect != nullptr)
	{ // エフェクトが使用されている場合

		D3DXMATRIX m, m1;
		D3DXVECTOR4 LightDir;
		D3DXVECTOR4 v;

		m = (*pMtxWorld) * m_mtxView * m_mtxProjection;
		m_pEffect->SetMatrix(m_mtxWorldViewProj, &m);

		// カメラ位置
		m1 = (*pMtxWorld) * m_mtxView;
		D3DXMatrixInverse(&m1, nullptr, &m1);
		D3DXVec4Transform(&v, pPosCamera, &m1);
		m_pEffect->SetVector(m_pEyePos, &v);

		// Light
		LightDir = *pDirLight;
		D3DXMatrixInverse(&m1, nullptr, pMtxWorld);
		D3DXVec4Transform(&v, &LightDir, &m1);
		D3DXVec3Normalize((D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v);
		m_pEffect->SetVector(m_pLightDir, &v);
	}
	else
	{ // エフェクトが使用されていない場合

		pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
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
	// エフェクトを返す
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
