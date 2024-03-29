﻿//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"
#include "manager.h"
#include "sceneGame.h"
#include "renderer.h"
#include "input.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// カメラ基本情報
	namespace basic
	{
		const D3DXVECTOR3 INIT_VECU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトルの初期値
		const D3DXVECTOR3 INIT_POSV = D3DXVECTOR3(0.0f, 0.0f, -600.0f);	// 視点の初期値
		const float VIEW_NEAR	= 10.0f;	// モデルが見えるZ軸の最小値
		const float VIEW_FAR	= 50000.0f;	// モデルが見えるZ軸の最大値

		const float VIEW_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
		const float VIEW_ANGLE = D3DXToRadian(45.0f);	// 視野角
	}

	// 回転カメラ情報
	namespace rotate
	{
		const D3DXVECTOR3 INIT_POSR	= D3DXVECTOR3(0.0f, 550.0f, 0.0f);	// 回転カメラの注視点の初期値
		const D3DXVECTOR3 INIT_ROT	= D3DXVECTOR2(1.2f, 0.0f);			// 回転カメラの向き初期値

		const float INIT_DIS	= -6350.0f;	// 回転カメラの距離初期値
		const float ADD_ROTY	= 0.005f;	// 回転カメラの向き加算量Y
	}

	// 追従カメラ情報
	namespace follow
	{
		const float REV_POS		= 0.25f;	// カメラ位置の補正係数
		const float REV_ROT		= 0.045f;	// カメラ向きの補正係数
		const float INIT_DIS	= 920.0f;	// 追従カメラの距離
		const float INIT_ROTX	= 1.7f;		// 追従カメラの向きX初期値
		const float ADD_ROTY	= 0.45f;	// 追従カメラの向き加算量Y

		const float LOOK_ADD_FORWARD	= 450.0f;	// プレイヤー位置からの前への加算量
		const float LOOK_ADD_POSY		= 50.0f;	// プレイヤー位置からの上への加算量
	}

	// ズームカメラ情報
	namespace zoom
	{
		const float DEST_DIS = 640.0f;	// ズームカメラの目標距離
		const float REV_DIS = 0.006f;	// カメラ距離の補正係数
	}

	// 操作カメラ情報
	namespace control
	{
		const float REV_MOVE_MOUSE	= 1.0f;		// マウス操作でのカメラの移動の補正係数
		const float REV_DIS_MOUSE	= -0.2f;	// マウス操作でのカメラの距離の補正係数
		const float REV_ROT_MOUSE	= 0.0045f;	// マウス操作でのカメラの回転量の補正係数

		const float MIN_DIS	= -10000.0f;	// カメラの視点から注視点への距離の最小
		const float MAX_DIS	= -1.0f;		// カメラの視点から注視点への距離の最大

		const float LIMIT_ROT_HIGH	= D3DX_PI - 0.1f;	// X上回転の制限値
		const float LIMIT_ROT_LOW	= 0.1f;				// X下回転の制限値
	}

#if _DEBUG

	// エディットカメラ情報
	namespace edit
	{
		const float INIT_DIS = -1000.0f;	// エディットカメラの距離
	}

#endif	// _DEBUG
}

//************************************************************
//	親クラス [CCamera] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCamera::CCamera()
{
	// メンバ変数をクリア
	memset(&m_aCamera[0], 0, sizeof(m_aCamera));	// カメラの情報
	m_state = STATE_NONE;	// 状態
	m_bUpdate = false;		// 更新状況
}

//============================================================
//	デストラクタ
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCamera::Init(void)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_state = STATE_NONE;	// 状態
	m_bUpdate = true;	 	// 更新状況

	//--------------------------------------------------------
	//	メインカメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MAIN].posV		= VEC3_ZERO;		// 現在の視点
	m_aCamera[TYPE_MAIN].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MAIN].destPosV	= VEC3_ZERO;		// 目標の視点
	m_aCamera[TYPE_MAIN].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MAIN].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MAIN].rot		= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MAIN].destRot	= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MAIN].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MAIN].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_MAIN].viewport.X			= 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_MAIN].viewport.Y			= 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_MAIN].viewport.Width		= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MAIN].viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MAIN].viewport.MinZ		= 0.0f;
	m_aCamera[TYPE_MAIN].viewport.MaxZ		= 1.0f;

	//--------------------------------------------------------
	//	モデルUI表示カメラの初期化
	//--------------------------------------------------------
	// カメラ情報を初期化
	m_aCamera[TYPE_MODELUI].posV		= basic::INIT_POSV;	// 現在の視点
	m_aCamera[TYPE_MODELUI].posR		= VEC3_ZERO;		// 現在の注視点
	m_aCamera[TYPE_MODELUI].destPosV	= basic::INIT_POSV;	// 目標の視点
	m_aCamera[TYPE_MODELUI].destPosR	= VEC3_ZERO;		// 目標の注視点
	m_aCamera[TYPE_MODELUI].vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_aCamera[TYPE_MODELUI].rot			= VEC3_ZERO;		// 現在の向き
	m_aCamera[TYPE_MODELUI].destRot		= VEC3_ZERO;		// 目標の向き
	m_aCamera[TYPE_MODELUI].fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_aCamera[TYPE_MODELUI].fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// ビューポート情報を初期化
	m_aCamera[TYPE_MODELUI].viewport.X		= 0;				// 左上隅のピクセル座標 (x)
	m_aCamera[TYPE_MODELUI].viewport.Y		= 0;				// 左上隅のピクセル座標 (y)
	m_aCamera[TYPE_MODELUI].viewport.Width	= SCREEN_WIDTH;		// 描画する画面の横幅
	m_aCamera[TYPE_MODELUI].viewport.Height	= SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_aCamera[TYPE_MODELUI].viewport.MinZ	= 0.0f;
	m_aCamera[TYPE_MODELUI].viewport.MaxZ	= 0.5f;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CCamera::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CCamera::Update(void)
{
	if (m_bUpdate)
	{ // 更新する状況の場合

		switch (m_state)
		{ // 状態ごとの処理
		case STATE_NONE:	// なにもしない状態

			// 無し

			break;

		case STATE_CONTROL:	// 操作状態

			// カメラの更新 (操作)
			Control();

			break;

		case STATE_ROTATE:	// 回転状態

			// カメラの更新 (回転)
			Rotate();

			break;

		case STATE_FOLLOW:	// 追従状態

			// カメラの更新 (追従)
			Follow();

			break;

		case STATE_ZOOM:	// ズーム状態

			// カメラの更新 (ズーム)
			Zoom();

			break;

		default:	// 例外処理
			assert(false);
			break;
		}
	}

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 視点 ]：%f %f %f\n", m_aCamera[TYPE_MAIN].posV.x, m_aCamera[TYPE_MAIN].posV.y, m_aCamera[TYPE_MAIN].posV.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[注視点]：%f %f %f\n", m_aCamera[TYPE_MAIN].posR.x, m_aCamera[TYPE_MAIN].posR.y, m_aCamera[TYPE_MAIN].posR.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 向き ]：%f %f %f\n", m_aCamera[TYPE_MAIN].rot.x, m_aCamera[TYPE_MAIN].rot.y, m_aCamera[TYPE_MAIN].rot.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[ 距離 ]：%f\n", m_aCamera[TYPE_MAIN].fDis);
}

//============================================================
//	カメラ設定処理
//============================================================
void CCamera::SetCamera(const EType type)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&m_aCamera[type].viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	( // 引数
		&m_aCamera[type].mtxProjection,	// プロジェクションマトリックス
		basic::VIEW_ANGLE,	// 視野角
		basic::VIEW_ASPECT,	// 画面のアスペクト比
		basic::VIEW_NEAR,	// Z軸の最小値
		basic::VIEW_FAR		// Z軸の最大値
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_aCamera[type].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_aCamera[type].mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&m_aCamera[type].mtxView,	// ビューマトリックス
		&m_aCamera[type].posV,		// 視点
		&m_aCamera[type].posR,		// 注視点
		&m_aCamera[type].vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera[type].mtxView);
}

//============================================================
//	カメラ目標位置の設定処理 (回転)
//============================================================
void CCamera::SetDestRotate(void)
{
	if (m_state != STATE_ROTATE)
	{ // カメラの回転状態ではない場合

		// 処理を抜ける
		return;
	}

	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y = rotate::INIT_ROT.y;
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラの目標位置の設定処理 (追従)
//============================================================
void CCamera::SetDestFollow(void)
{
	if (m_state != STATE_FOLLOW)
	{ // カメラの追従状態ではない場合

		// 処理を抜ける
		return;
	}

	if (CScene::GetPlayer() != nullptr)
	{ // プレイヤーが使用されている場合

		// 変数を宣言
		D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();	// プレイヤー位置
		D3DXVECTOR3 rotPlayer = CScene::GetPlayer()->GetVec3Rotation();	// プレイヤー向き
		D3DXVECTOR3 posLook  = VEC3_ZERO;	// 位置
		D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
		D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
		D3DXVECTOR3 diffRot  = VEC3_ZERO;	// 差分向き

		// カメラの見る位置を調整
		posLook.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;
		posLook.y = posPlayer.y + follow::LOOK_ADD_POSY;
		posLook.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;

		//----------------------------------------------------
		//	向きの更新
		//----------------------------------------------------
		// 向きを設定
		m_aCamera[TYPE_MAIN].rot.x = m_aCamera[TYPE_MAIN].destRot.x = follow::INIT_ROTX;
		m_aCamera[TYPE_MAIN].rot.y = m_aCamera[TYPE_MAIN].destRot.y = rotPlayer.y + HALF_PI + follow::ADD_ROTY;

		// 向きを正規化
		useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);
		useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

		//----------------------------------------------------
		//	距離の更新
		//----------------------------------------------------
		// 目標距離を設定
		m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = follow::INIT_DIS;

		//----------------------------------------------------
		//	位置の更新
		//----------------------------------------------------
		// 注視点の更新
		m_aCamera[TYPE_MAIN].posR = m_aCamera[TYPE_MAIN].destPosR = posLook;

		// 視点の更新
		m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
		m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
		m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CCamera::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの向きを設定
	m_aCamera[TYPE_MAIN].rot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.x);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.z);
}

//============================================================
//	目標向きの設定処理
//============================================================
void CCamera::SetVec3DestRotation(const D3DXVECTOR3& rRot)
{
	// 引数のカメラの目標向きを設定
	m_aCamera[TYPE_MAIN].destRot = rRot;

	// 向きを正規化
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.x);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.y);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].destRot.z);
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera::SCamera CCamera::GetCamera(const EType type)
{
	// カメラの情報を返す
	return m_aCamera[type];
}

//============================================================
//	カメラ状態の設定処理
//============================================================
void CCamera::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
}

//============================================================
//	カメラ状態取得処理
//============================================================
CCamera::EState CCamera::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3Rotation(void) const
{
	// カメラの向きを返す
	return m_aCamera[TYPE_MAIN].rot;
}

//============================================================
//	目標向き取得処理
//============================================================
D3DXVECTOR3 CCamera::GetVec3DestRotation(void) const
{
	// カメラの目標向きを返す
	return m_aCamera[TYPE_MAIN].destRot;
}

//============================================================
//	生成処理
//============================================================
CCamera *CCamera::Create(void)
{
	// ポインタを宣言
	CCamera *pCamera = nullptr;	// カメラ生成用

	if (pCamera == nullptr)
	{ // 使用されていない場合

		// メモリを確保
		pCamera = new CCamera;	// カメラ
	}
	else { assert(false); return nullptr; }	// 使用中

	if (pCamera != nullptr)
	{ // 確保に成功している場合

		// カメラの初期化
		if (FAILED(pCamera->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pCamera;
			pCamera = nullptr;

			// 失敗を返す
			return nullptr;
		}

		// 確保したアドレスを返す
		return pCamera;
	}
	else { assert(false); return nullptr; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CCamera::Release(CCamera *&prCamera)
{
	if (prCamera != nullptr)
	{ // 使用中の場合

		// カメラの終了
		prCamera->Uninit();

		// メモリ開放
		delete prCamera;
		prCamera = nullptr;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

//============================================================
//	カメラの更新処理 (回転)
//============================================================
void CCamera::Rotate(void)
{
	//--------------------------------------------------------
	//	向きの更新
	//--------------------------------------------------------
	// 現在向きの更新
	m_aCamera[TYPE_MAIN].rot.x = rotate::INIT_ROT.x;
	m_aCamera[TYPE_MAIN].rot.y += rotate::ADD_ROTY;
	useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

	//--------------------------------------------------------
	//	距離の更新
	//--------------------------------------------------------
	m_aCamera[TYPE_MAIN].fDis = rotate::INIT_DIS;

	//--------------------------------------------------------
	//	位置の更新
	//--------------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR = rotate::INIT_POSR;

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}

//============================================================
//	カメラの更新処理 (追従)
//============================================================
void CCamera::Follow(void)
{
	if (CScene::GetPlayer() != nullptr)
	{ // プレイヤーが使用されている場合

		// 変数を宣言
		D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();	// プレイヤー位置
		D3DXVECTOR3 rotPlayer = CScene::GetPlayer()->GetVec3Rotation();	// プレイヤー向き
		D3DXVECTOR3 posLook  = VEC3_ZERO;	// 位置
		D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
		D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
		D3DXVECTOR3 diffRot  = VEC3_ZERO;	// 差分向き

		// カメラの見る位置を調整
		posLook.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;
		posLook.y = posPlayer.y + follow::LOOK_ADD_POSY;
		posLook.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;

		//----------------------------------------------------
		//	向きの更新
		//----------------------------------------------------
		// 目標向きを設定
		m_aCamera[TYPE_MAIN].destRot.x = follow::INIT_ROTX;
		m_aCamera[TYPE_MAIN].destRot.y = rotPlayer.y + HALF_PI + follow::ADD_ROTY;

		// 目標向きを正規化
		useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].destRot);

		// 差分向きを計算
		diffRot = m_aCamera[TYPE_MAIN].destRot - m_aCamera[TYPE_MAIN].rot;
		useful::Vec3NormalizeRot(diffRot);	// 差分向きを正規化

		// 現在向きの更新
		m_aCamera[TYPE_MAIN].rot += diffRot * follow::REV_ROT;
		useful::Vec3NormalizeRot(m_aCamera[TYPE_MAIN].rot);	// 現在向きを正規化

		//----------------------------------------------------
		//	距離の更新
		//----------------------------------------------------
		// 目標距離を設定
		m_aCamera[TYPE_MAIN].fDis = m_aCamera[TYPE_MAIN].fDestDis = follow::INIT_DIS;

		//----------------------------------------------------
		//	位置の更新
		//----------------------------------------------------
		// 注視点の更新
		m_aCamera[TYPE_MAIN].destPosR = posLook;

		// 視点の更新
		m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
		m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
		m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

		// 注視点の差分位置を計算
		diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

		// 視点の差分位置を計算
		diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

		// 注視点の現在位置を更新
		m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * follow::REV_POS;
		m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * follow::REV_POS;
		m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * follow::REV_POS;

		// 視点の現在位置を更新
		m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * follow::REV_POS;
		m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * follow::REV_POS;
		m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * follow::REV_POS;
	}
}

//============================================================
//	カメラの更新処理 (ズーム)
//============================================================
void CCamera::Zoom(void)
{
	// 変数を宣言
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();	// プレイヤー位置
	D3DXVECTOR3 rotPlayer = CScene::GetPlayer()->GetVec3Rotation();	// プレイヤー向き
	D3DXVECTOR3 posLook  = VEC3_ZERO;	// 位置
	D3DXVECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	D3DXVECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置
	float fDiffDis = 0.0f;	// 差分距離

	// カメラの見る位置を調整
	posLook.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;
	posLook.y = posPlayer.y + follow::LOOK_ADD_POSY;
	posLook.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * follow::LOOK_ADD_FORWARD;

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 目標距離を設定
	m_aCamera[TYPE_MAIN].fDestDis = zoom::DEST_DIS;

	// 差分距離を計算
	fDiffDis = m_aCamera[TYPE_MAIN].fDestDis - m_aCamera[TYPE_MAIN].fDis;

	// 距離を設定
	m_aCamera[TYPE_MAIN].fDis += fDiffDis * zoom::REV_DIS;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の更新
	m_aCamera[TYPE_MAIN].destPosR = posLook;

	// 視点の更新
	m_aCamera[TYPE_MAIN].destPosV.x = m_aCamera[TYPE_MAIN].destPosR.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].destPosV.y = m_aCamera[TYPE_MAIN].destPosR.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].destPosV.z = m_aCamera[TYPE_MAIN].destPosR.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// 注視点の差分位置を計算
	diffPosR = m_aCamera[TYPE_MAIN].destPosR - m_aCamera[TYPE_MAIN].posR;

	// 視点の差分位置を計算
	diffPosV = m_aCamera[TYPE_MAIN].destPosV - m_aCamera[TYPE_MAIN].posV;

	// 注視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posR.x += diffPosR.x * follow::REV_POS;
	m_aCamera[TYPE_MAIN].posR.y += diffPosR.y * follow::REV_POS;
	m_aCamera[TYPE_MAIN].posR.z += diffPosR.z * follow::REV_POS;

	// 視点の現在位置を更新
	m_aCamera[TYPE_MAIN].posV.x += diffPosV.x * follow::REV_POS;
	m_aCamera[TYPE_MAIN].posV.y += diffPosV.y * follow::REV_POS;
	m_aCamera[TYPE_MAIN].posV.z += diffPosV.z * follow::REV_POS;
}

//============================================================
//	カメラの更新処理 (操作)
//============================================================
void CCamera::Control(void)
{
	// 位置の更新
	Move();

	// 距離の更新
	Distance();

	// 向きの更新
	Rotation();
}

//============================================================
//	位置の更新処理 (操作)
//============================================================
void CCamera::Move(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		m_aCamera[TYPE_MAIN].posV.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posV.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;

		// 注視点を移動
		m_aCamera[TYPE_MAIN].posR.x -= sinf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z -= cosf(m_aCamera[TYPE_MAIN].rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.x += sinf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_aCamera[TYPE_MAIN].posR.z += cosf(m_aCamera[TYPE_MAIN].rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
	}
}

//============================================================
//	距離の更新処理 (操作)
//============================================================
void CCamera::Distance(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (mouseMove.z != 0.0f)
	{ // マウスホイールが操作された場合

		// 距離を加算
		m_aCamera[TYPE_MAIN].fDis += mouseMove.z * control::REV_DIS_MOUSE;
	}

	// 距離の補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].fDis, control::MIN_DIS, control::MAX_DIS);
}

//============================================================
//	向きの更新処理 (操作)
//============================================================
void CCamera::Rotation(void)
{
	// ポインタを宣言
	CInputMouse	*pMouse = CManager::GetInstance()->GetMouse();	// マウスの取得

	// 変数を宣言
	D3DXVECTOR3 mouseMove = pMouse->GetMove();	// マウスの移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && !pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 左クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, control::LIMIT_ROT_LOW, control::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 視点の更新
	m_aCamera[TYPE_MAIN].posV.x = m_aCamera[TYPE_MAIN].posR.x + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posV.y = m_aCamera[TYPE_MAIN].posR.y + ((m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posV.z = m_aCamera[TYPE_MAIN].posR.z + ((m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_RIGHT) && !pMouse->IsPress(CInputMouse::KEY_LEFT))
	{ // 右クリックだけが押されている場合

		// カメラのY軸を回転
		m_aCamera[TYPE_MAIN].rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_aCamera[TYPE_MAIN].rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_aCamera[TYPE_MAIN].rot.x, control::LIMIT_ROT_LOW, control::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_aCamera[TYPE_MAIN].rot.y);

	// 注視点の更新
	m_aCamera[TYPE_MAIN].posR.x = m_aCamera[TYPE_MAIN].posV.x + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * sinf(m_aCamera[TYPE_MAIN].rot.y));
	m_aCamera[TYPE_MAIN].posR.y = m_aCamera[TYPE_MAIN].posV.y + ((-m_aCamera[TYPE_MAIN].fDis * cosf(m_aCamera[TYPE_MAIN].rot.x)));
	m_aCamera[TYPE_MAIN].posR.z = m_aCamera[TYPE_MAIN].posV.z + ((-m_aCamera[TYPE_MAIN].fDis * sinf(m_aCamera[TYPE_MAIN].rot.x)) * cosf(m_aCamera[TYPE_MAIN].rot.y));
}
