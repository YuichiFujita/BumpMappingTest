//============================================================
//
//	マネージャー処理 [manager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "shaderManager.h"
#include "retentionManager.h"
#include "object.h"

#ifdef _DEBUG	// デバッグ処理

#include "debug.h"

#endif	// _DEBUG

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CManager *CManager::m_pManager = nullptr;	// マネージャーオブジェクト

//************************************************************
//	親クラス [CManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CManager::CManager()
{
	// メンバ変数をクリア
	m_pRenderer	= nullptr;	// レンダラーオブジェクト
	m_pKeyboard	= nullptr;	// キーボードオブジェクト
	m_pMouse	= nullptr;	// マウスオブジェクト
	m_pPad		= nullptr;	// パッドオブジェクト
	m_pSound	= nullptr;	// サウンドオブジェクト
	m_pCamera	= nullptr;	// カメラオブジェクト
	m_pLight	= nullptr;	// ライトオブジェクト
	m_pTexture	= nullptr;	// テクスチャオブジェクト
	m_pModel	= nullptr;	// モデルオブジェクト
	m_pFade		= nullptr;	// フェードオブジェクト
	m_pScene	= nullptr;	// シーンオブジェクト
	m_pShaderManager	= nullptr;	// シェーダーマネージャー
	m_pRetentionManager	= nullptr;	// データ保存マネージャー
	m_pDebugProc		= nullptr;	// デバッグ表示

#ifdef _DEBUG	// デバッグ処理

	// メンバ変数をクリア
	m_pDebug = nullptr;	// デバッグ

#endif	// _DEBUG
}

//============================================================
//	デストラクタ
//============================================================
CManager::~CManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_pRenderer	= nullptr;	// レンダラーオブジェクト
	m_pKeyboard	= nullptr;	// キーボードオブジェクト
	m_pMouse	= nullptr;	// マウスオブジェクト
	m_pPad		= nullptr;	// パッドオブジェクト
	m_pSound	= nullptr;	// サウンドオブジェクト
	m_pCamera	= nullptr;	// カメラオブジェクト
	m_pLight	= nullptr;	// ライトオブジェクト
	m_pTexture	= nullptr;	// テクスチャオブジェクト
	m_pModel	= nullptr;	// モデルオブジェクト
	m_pFade		= nullptr;	// フェードオブジェクト
	m_pScene	= nullptr;	// シーンオブジェクト
	m_pShaderManager	= nullptr;	// シェーダーマネージャー
	m_pRetentionManager	= nullptr;	// データ保存マネージャー
	m_pDebugProc		= nullptr;	// デバッグ表示

#ifdef _DEBUG	// デバッグ処理

	m_pDebug = nullptr;	// デバッグ

#endif	// _DEBUG

	//--------------------------------------------------------
	//	システムの生成
	//--------------------------------------------------------
	// レンダラーの生成
	m_pRenderer = CRenderer::Create(hWnd, bWindow);
	if (m_pRenderer == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードの生成
	m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (m_pKeyboard == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスの生成
	m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (m_pMouse == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パッドの生成
	m_pPad = CInputPad::Create();
	if (m_pPad == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = CSound::Create(hWnd);
	if (m_pSound == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = CCamera::Create();
	if (m_pCamera == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ライトの生成
	m_pLight = CLight::Create();
	if (m_pLight == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シェーダーマネージャーの生成
	m_pShaderManager = CShaderManager::Create();
	if (m_pShaderManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// データ保存マネージャーの生成
	m_pRetentionManager = CRetentionManager::Create();
	if (m_pRetentionManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	情報の読込・設定
	//--------------------------------------------------------
	// テクスチャの生成・読込
	m_pTexture = CTexture::Create();
	if (m_pTexture == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの生成・読込
	m_pModel = CModel::Create();
	if (m_pModel == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フェードの生成・シーンの設定
	m_pFade = CFade::Create();
	if (m_pFade == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の生成
	m_pDebugProc = CDebugProc::Create();
	if (m_pDebugProc == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// デバッグ処理

	// デバッグの生成
	m_pDebug = CDebug::Create();
	if (m_pDebug == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CManager::Uninit(void)
{
	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の破棄
	if (FAILED(CDebugProc::Release(m_pDebugProc)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#ifdef _DEBUG	// デバッグ処理

	// デバッグの破棄
	if (FAILED(CDebug::Release(m_pDebug)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	//--------------------------------------------------------
	//	情報の破棄
	//--------------------------------------------------------
	// テクスチャの破棄
	if (FAILED(CTexture::Release(m_pTexture)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// モデルの破棄
	if (FAILED(CModel::Release(m_pModel)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	システムの破棄
	//--------------------------------------------------------
	// データ保存マネージャーの破棄
	if (FAILED(CRetentionManager::Release(m_pRetentionManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シェーダーマネージャーの破棄
	if (FAILED(CShaderManager::Release(m_pShaderManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの破棄
	if (FAILED(CScene::Release(m_pScene)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// フェードの破棄
	if (FAILED(CFade::Release(m_pFade)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ライトの破棄
	if (FAILED(CLight::Release(m_pLight)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// カメラの破棄
	if (FAILED(CCamera::Release(m_pCamera)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// サウンドの破棄
	if (FAILED(CSound::Release(m_pSound)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// パッドの破棄
	if (FAILED(CInputPad::Release(m_pPad)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// マウスの破棄
	if (FAILED(CInputMouse::Release(m_pMouse)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キーボードの破棄
	if (FAILED(CInputKeyboard::Release(m_pKeyboard)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// 例外処理
	assert(CObject::GetNumAll() == 0);	// 破棄の失敗

	// レンダラーの破棄
	if (FAILED(CRenderer::Release(m_pRenderer)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CManager::Update(void)
{
#ifdef _DEBUG	// デバッグ処理

	// デバッグ表示
	m_pDebugProc->Print(CDebugProc::POINT_CENTER, "[FPS]：%d\n", m_pDebug->GetFps());

#endif	// _DEBUG

	if (m_pPad != nullptr)
	{ // 使用中の場合

		// パッドの更新
		m_pPad->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pMouse != nullptr)
	{ // 使用中の場合

		// マウスの更新
		m_pMouse->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pKeyboard != nullptr)
	{ // 使用中の場合

		// キーボードの更新
		m_pKeyboard->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pFade != nullptr)
	{ // 使用中の場合

		// フェードの更新
		m_pFade->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pScene != nullptr)
	{ // 使用中の場合

		// シーンの更新
		m_pScene->Update();
	}
	else { assert(false); }	// 非使用中

	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	if (m_pDebugProc != nullptr)
	{ // 使用中の場合

		// デバッグ表示の更新
		m_pDebugProc->Update();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	描画処理
//============================================================
void CManager::Draw(void)
{
	if (m_pRenderer != nullptr)
	{ // 使用中の場合

		// レンダラーの描画
		m_pRenderer->Draw();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	生成処理
//============================================================
CManager *CManager::Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pManager == nullptr)
	{ // 使用されていない場合

		// メモリを確保
		m_pManager = new CManager;	// マネージャー
	}
	else { assert(false); return nullptr; }	// 使用中

	if (m_pManager != nullptr)
	{ // 確保に成功している場合

		// マネージャーの初期化
		if (FAILED(m_pManager->Init(hInstance, hWnd, bWindow)))
		{ // 初期化に失敗した場合

			// マネージャーの終了
			if (FAILED(m_pManager->Uninit()))
			{ // マネージャーの終了に失敗した場合

				// メモリ開放
				delete m_pManager;
				m_pManager = nullptr;

				// 失敗を返す
				assert(false);
				return nullptr;
			}

			// メモリ開放
			delete m_pManager;
			m_pManager = nullptr;

			// 失敗を返す
			assert(false);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pManager;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}

//============================================================
//	取得処理
//============================================================
CManager *CManager::GetInstance(void)
{
	// 例外処理
	assert(m_pManager != nullptr);	// インスタンス未使用

	// マネージャーのポインタを返す
	return m_pManager;
}

//============================================================
//	破棄処理
//============================================================
HRESULT CManager::Release(CManager *&prManager)
{
	if (prManager != nullptr)
	{ // 使用中の場合

		// マネージャーの終了
		if (FAILED(prManager->Uninit()))
		{ // マネージャーの終了に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// メモリ開放
		delete prManager;
		prManager = nullptr;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	シーンの設定処理
//============================================================
void CManager::SetScene(const CScene::EMode mode, const int nWait)
{
	// 次のシーンを設定
	m_pFade->Set(mode, nWait);
}

//============================================================
//	モードの設定処理
//============================================================
HRESULT CManager::SetMode(const CScene::EMode mode)
{
	// サウンドを停止
	m_pSound->Stop();

	if (m_pScene != nullptr)
	{ // シーンが使用中の場合

		// シーンの破棄
		if (FAILED(CScene::Release(m_pScene)))
		{ // 破棄に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	if (m_pScene == nullptr)
	{ // シーンが非使用中の場合

		// シーンの生成
		m_pScene = CScene::Create(mode);
		if (m_pScene == nullptr)
		{ // 非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 成功を返す
	return S_OK;
}

//============================================================
//	モードの取得処理
//============================================================
CScene::EMode CManager::GetMode(void)
{
	// 現在のモードを返す
	return m_pScene->GetMode();
}

//============================================================
//	レンダラー取得処理
//============================================================
CRenderer *CManager::GetRenderer(void)
{
	// レンダラーのポインタを返す
	return m_pRenderer;
}

//============================================================
//	キーボード取得処理
//============================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	// キーボードのポインタを返す
	return m_pKeyboard;
}

//============================================================
//	マウス取得処理
//============================================================
CInputMouse *CManager::GetMouse(void)
{
	// マウスのポインタを返す
	return m_pMouse;
}

//============================================================
//	パッド取得処理
//============================================================
CInputPad *CManager::GetPad(void)
{
	// パッドのポインタを返す
	return m_pPad;
}

//============================================================
//	サウンド取得処理
//============================================================
CSound *CManager::GetSound(void)
{
	// サウンドのポインタを返す
	return m_pSound;
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera *CManager::GetCamera(void)
{
	// カメラのポインタを返す
	return m_pCamera;
}

//============================================================
//	ライト取得処理
//============================================================
CLight *CManager::GetLight(void)
{
	// ライトのポインタを返す
	return m_pLight;
}

//============================================================
//	テクスチャ取得処理
//============================================================
CTexture *CManager::GetTexture(void)
{
	// テクスチャのポインタを返す
	return m_pTexture;
}

//============================================================
//	モデル取得処理
//============================================================
CModel *CManager::GetModel(void)
{
	// モデルのポインタを返す
	return m_pModel;
}

//============================================================
//	フェード取得処理
//============================================================
CFade *CManager::GetFade(void)
{
	// フェードのポインタを返す
	return m_pFade;
}

//============================================================
//	シーン取得処理
//============================================================
CScene *CManager::GetScene(void)
{
	// シーンのポインタを返す
	return m_pScene;
}

//============================================================
//	シェーダーマネージャー取得処理
//============================================================
CShaderManager *CManager::GetShaderManager(void)
{
	return nullptr;
}

//============================================================
//	データ保存マネージャー取得処理
//============================================================
CRetentionManager *CManager::GetRetentionManager(void)
{
	// データ保存マネージャーを返す
	return m_pRetentionManager;
}

//============================================================
//	デバッグ表示取得処理
//============================================================
CDebugProc *CManager::GetDebugProc(void)
{
	// デバッグ表示のポインタを返す
	return m_pDebugProc;
}

//============================================================
//	デバッグ取得処理
//============================================================
#ifdef _DEBUG	// デバッグ処理

CDebug *CManager::GetDebug(void)
{
	// デバッグのポインタを返す
	return m_pDebug;
}

#endif	// _DEBUG
