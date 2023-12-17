//============================================================
//
//	バンプマッピングヘッダー [shaderBumpmap.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BUMPMAP_H_
#define _BUMPMAP_H_

//************************************************************
//	クラス定義
//************************************************************
// バンプマッピングクラス
class CBumpMap
{
public:
	// コンストラクタ
	CBumpMap();

	// デストラクタ
	~CBumpMap();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了

	void Begin(void);					// 開始
	void BeginPass(const BYTE pass);	// パス開始
	void EndPass(void);					// パス終了
	void End(void);						// 終了

	void SetAmbient(const float fAmbient);		// 環境光設定
	void SetSpecular(const float fSpecular);	// 反射光設定
	void SetSpecularPower(const float fPower);	// 反射ハイライト設定

	void SetMatrix	// マトリックス設定
	( // 引数
		D3DXMATRIX* pMtxWorld,		// ワールドマトリックス
		D3DXVECTOR4* pPosCamera,	// カメラ位置
		D3DXVECTOR4* pDirLight		// ライト方向
	);

	void CommitChanges(void);			// 状態変更の伝達
	bool IsEffectOK(void) const;		// エフェクト使用可能状況の取得
	LPD3DXEFFECT GetEffect(void) const;	// エフェクトポインタ取得

	// 静的メンバ関数
	static CBumpMap *Create(void);		// 生成
	static CBumpMap *GetInstance(void);	// 取得
	static HRESULT Release(void);		// 破棄

private:
	// 静的メンバ変数
	static CBumpMap *m_pBumpMap;	// バンプマッピング

	// メンバ変数
	LPD3DXEFFECT m_pEffect;			// エフェクトポインタ
	D3DXHANDLE m_pTechnique;		// テクニック関数
	D3DXHANDLE m_pMtxWorldViewProj;	// ワールドビュー射影変換マトリックス
	D3DXHANDLE m_pLightDir;			// 平行光源の方向ベクトル
	D3DXHANDLE m_pEyePos;			// 視点位置ベクトル
	D3DXHANDLE m_pAmbient;			// 環境光
	D3DXHANDLE m_pSpecular;			// ハイライトの範囲
	D3DXHANDLE m_pSpecularPower;	// ハイライトの強度
};

#endif	// _BUMPMAP_H_
