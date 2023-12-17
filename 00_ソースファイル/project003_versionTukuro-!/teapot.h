//============================================================
//
//	ティーポットヘッダー [teapot.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TEAPOT_H_
#define _TEAPOT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	クラス定義
//************************************************************
// ティーポットクラス
class CTeapot : public CObjectModel
{
public:
	// テクスチャ列挙
	enum ETeture
	{
		TEXTURE_NORMAL = 0,	// 法線テクスチャ
		TEXTURE_MAX			// この列挙型の総数
	};

	// モデル列挙
	enum EModel
	{
		MODEL_TEAPOT = 0,	// ティーポット
		MODEL_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CTeapot();

	// デストラクタ
	~CTeapot();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CTeapot *Create	// 生成
	( // 引数
		const D3DXVECTOR3& rPos,	// 位置
		const D3DXVECTOR3& rRot		// 向き
	);
};

#endif	// _TEAPOT_H_
