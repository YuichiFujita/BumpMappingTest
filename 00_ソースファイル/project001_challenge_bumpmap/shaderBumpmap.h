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

	//void Invalidate();
	//void Restore();
	//HRESULT Load();

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
	bool IsOK(void) const;				// エフェクト使用可能状況の取得
	LPD3DXEFFECT GetEffect(void) const;	// エフェクトポインタ取得

	// 静的メンバ関数
	static CBumpMap *Create(void);	// 生成
	static HRESULT Release(CBumpMap *&prBumpMap);	// 破棄

private:
	// メンバ変数
	LPD3DXEFFECT m_pEffect;	// エフェクトポインタ

	D3DXHANDLE m_pTechnique, m_pWVP, m_pLightDir, m_pEyePos, m_pAmbient, m_pSpecular, m_pSpecularPower;
	D3DXMATRIX m_matView, m_matProj;
};

#endif	// _BUMPMAP_H_
