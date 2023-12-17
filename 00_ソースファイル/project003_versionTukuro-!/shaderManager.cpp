//============================================================
//
//	シェーダーマネージャー処理 [shaderManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shaderManager.h"
#include "shaderBumpmap.h"

//************************************************************
//	親クラス [CShaderManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShaderManager::CShaderManager()
{
	// メンバ変数をクリア
	m_pBumpMap = nullptr;	// バンプマッピング
}

//============================================================
//	デストラクタ
//============================================================
CShaderManager::~CShaderManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShaderManager::Init(void)
{
	// メンバ変数を初期化
	m_pBumpMap = nullptr;	// バンプマッピング

	// バンプマッピングの生成
	m_pBumpMap = CBumpMap::Create();
	if (m_pBumpMap == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CShaderManager::Uninit(void)
{
	// バンプマッピングの破棄
	if (FAILED(CBumpMap::Release()))
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	生成処理
//============================================================
CShaderManager *CShaderManager::Create(void)
{
	// シェーダーマネージャーの生成
	CShaderManager *pShaderManager = new CShaderManager;	// シェーダーマネージャーの情報
	if (pShaderManager == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// シェーダーマネージャーの初期化
		if (FAILED(pShaderManager->Init()))
		{ // 初期化に失敗した場合

			// シェーダーマネージャーの破棄
			delete pShaderManager;
			pShaderManager = nullptr;

			assert(false);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pShaderManager;
	}
}

//============================================================
//	破棄処理
//============================================================
HRESULT CShaderManager::Release(CShaderManager *&prShaderManager)
{
	if (prShaderManager != nullptr)
	{ // 使用中の場合

		// シェーダーマネージャーの終了
		prShaderManager->Uninit();

		// メモリ開放
		delete prShaderManager;
		prShaderManager = nullptr;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}
