//************************************************************
//	インクルードファイル
//************************************************************
#include "objectModel.h"
#include "shaderBumpmap.h"

#if 0
//************************************************************
//	グローバル変数宣言
//************************************************************
//D3DPRESENT_PARAMETERS m_d3dParameters;
//D3DCAPS9 Caps;

LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;

CObjectModel*		m_pMeshTeapot = NULL;	// ティーポットオブジェクト
LPDIRECT3DTEXTURE9	m_pNormalMap = NULL;	// 法線マップ
CBumpMap*			m_pBumpMap = NULL;		// バンプマップ

D3DXVECTOR4 LightPos = D3DXVECTOR4(72.0f, 100.0f, 620.0f, 0.0f);	// 太陽の位置ベクトル
D3DXVECTOR4 LightDir;												// 平行光源の光の方向ベクトル
D3DXVECTOR4 EyePos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);			// 視点の位置ベクトル

//ID3DXMeshインターフェースを使用する場合必要なし
LPDIRECT3DVERTEXDECLARATION9 m_pDecl = NULL;	//頂点シェーダー
bool RenderOK = false;

//============================================================
//	メイン関数
//============================================================
int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR     /*lpCmpLine*/,
	INT       /*nCmdShow*/
)
{
	char* AppName = "Tutrial";
	HWND hWnd = NULL;
	MSG msg; ZeroMemory(&msg, sizeof(MSG));

	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_VREDRAW | CS_HREDRAW;
		//wc.lpfnWndProc = (WNDPROC)WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(DWORD);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = AppName;
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.hInstance = hInstance;
		::RegisterClassEx(&wc);
	}

	//****************************************************************
	//	ここでウィンドウの作成処理
	//	ここでDirect3Dの初期化を行う。
	//****************************************************************
	//m_pd3dDevice->GetDeviceCaps(&Caps);

#if 0
	//バンプマップクラスの初期化
	m_pBumpMap = new BUMPMAP(m_pd3dDevice);
	m_pBumpMap->Load();
#else
	// バンプマップの生成
	m_pBumpMap = CBumpMap::Create();
#endif

#if 0
	//メッシュのロード
	m_pMeshTeapot = new CDXUTMesh();
	m_pMeshTeapot->Create(m_pd3dDevice, _T("t-pot.x"));
#else
	// バンプマップの生成
	m_pMeshTeapot = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
	m_pMeshTeapot->BindModel(_T("t-pot.x"));
#endif

	//頂点データを定義する(詳細はオンラインマニュアルを参照)
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

#if 0
	//新しい頂点情報に基づき頂点データを再生成する
	m_pMeshTeapot->SetVertexDecl(m_pd3dDevice, decl);
#else
	//バンプマッピング用に頂点シェーダ宣言を作成する
	m_pd3dDevice->CreateVertexDeclaration(decl, &m_pDecl);
#endif

#if 0
	//****************************************************************
	//高さマップから法線マップを作成する(注意１)
	//****************************************************************
	D3DSURFACE_DESC desc;
	LPDIRECT3DTEXTURE9 HeightMap = NULL;

	//高さマップをファイルからロード
	D3DXCreateTextureFromFileEx(m_pd3dDevice,
		_T("height.bmp"),  //高さマップテクスチャーのファイル名
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0,
		NULL,
		NULL,
		&HeightMap);

	//テクスチャ情報の入手
	HeightMap->GetLevelDesc(0, &desc);

	//法線マップの空テクスチャーを作成
	D3DXCreateTexture(m_pd3dDevice,
		desc.Width,
		desc.Height,
		1,
		0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pNormalMap);

	//法線マップを作成
	D3DXComputeNormalMap(m_pNormalMap,      //出力する法線マップテクスチャーのポインタ。
		HeightMap,         //入力する高さマップテクスチャーのポインタ。
		NULL,              //256色のカラーパレット情報。
		0,                 //法線マップ生成の定数。法線マップの使用方法により変更する。
		D3DX_CHANNEL_RED,  //高さマップで参照する色成分。
		1.0f               //高さ情報を乗算する定数値。値を大きくするほど高くなる。
	);
	SAFE_RELEASE(HeightMap);
#else

	//****************************************************************
	//	作成済みの法線マップをロードする
	//****************************************************************   
	D3DXCreateTextureFromFileEx
	(
		m_pd3dDevice,
		_T("normal.bmp"),   //法線マップテクスチャーのファイル名
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0,
		NULL,
		NULL,
		&m_pNormalMap
	);

#endif

	// 平行光源の位置ベクトルから方向ベクトルを計算する
	LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	RenderOK = true;

	//デバイス消失後にリストアする必要があるオブジェクトの初期化
	//Restore();

	{
		::ShowWindow(hWnd, SW_SHOW);
		::UpdateWindow(hWnd);

		do
		{
			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				if (MainLoop(hWnd) == FALSE)
					::DestroyWindow(hWnd);
			}
		} while (msg.message != WM_QUIT);

		::UnregisterClass(AppName, hInstance);
	}

	return msg.wParam;
}

#if 0
//デバイスのリセット前に開放すべきオブジェクト
void Invalidate()
{
	m_pBumpMap->Invalidate();
}

//デバイスのリセット後に初期化すべきオブジェクト
void Restore()
{
	m_pBumpMap->Restore();

	//固定機能パイプラインライティングを設定する
	D3DLIGHT9 Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(LightDir.x, LightDir.y, LightDir.z);
	Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetLight(0, &Light);
	m_pd3dDevice->LightEnable(0, TRUE);

	D3DMATERIAL9 Material;
	ZeroMemory(&Material, sizeof(Material));
	Material.Diffuse.r = 1.0f;
	Material.Diffuse.g = 1.0f;
	Material.Diffuse.b = 1.0f;
	Material.Diffuse.a = 1.0f;
	m_pd3dDevice->SetMaterial(&Material);
}
#endif

// メッセージループからコールされる関数
BOOL MainLoop(HWND HWnd)
{
	HRESULT hr;

	//レンダリング不可能
	if (RenderOK == false)
	{
		hr = m_pd3dDevice->TestCooperativeLevel();
		switch (hr)
		{
			//デバイスは消失しているがReset可能
		case D3DERR_DEVICENOTRESET:

			//開放
			//Invalidate();

			//デバイスをリセットする
			//hr = m_pd3dDevice->Reset(&m_d3dParameters);

			switch (hr)
			{
				//デバイスロスト
			case D3DERR_DEVICELOST:
				break;

				//内部ドライバーエラー
			case D3DERR_DRIVERINTERNALERROR:
				return FALSE;
				break;

				//メソッドの呼び出しが無効です
			case D3DERR_INVALIDCALL:
				return FALSE;
				break;

			case S_OK:

				//初期化
				//Restore();

				RenderOK = true;
			}
			break;
		}
	}

	//レンダリング可能
	else
	{
		D3DXMATRIX matProj, matView, matWorld;

		m_pd3dDevice->Clear(0L,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x0,
			1.0f,
			0L
		);

		m_pd3dDevice->BeginScene();

		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		//射影座標変換
		D3DXMatrixPerspectiveFovLH(&matProj,
			D3DX_PI / 4.0f,
			4.0f / 3.0f,
			0.1f,
			1000.0f
		);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		//ビュー座標変換
		D3DXMatrixIdentity(&matView);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		//ワールド座標変換
		D3DXMatrixIdentity(&matWorld);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

#if 0
		//ティーポットのテクスチャーをステージ０にセットする
		m_pd3dDevice->SetTexture(0, m_pMeshTeapot->m_pTextures[0]);
#endif

		//法線マップをステージ１にセットする
		m_pd3dDevice->SetTexture(1, m_pNormalMap);

		//頂点シェーダ宣言をバンプマッピング用に設定する
		m_pd3dDevice->SetVertexDeclaration(m_pDecl);

		//ティーポットをレンダリング
		m_pBumpMap->Begin();

		//マテリアルを設定する
		m_pBumpMap->SetAmbient(0.0f);
		m_pBumpMap->SetSpecular(10.0f);
		m_pBumpMap->SetSpecularPower(0.75f);
		m_pBumpMap->SetMatrix(&matWorld, &EyePos, &LightDir);

		m_pBumpMap->BeginPass(0);

#if 0
		m_pMeshTeapot->m_pLocalMesh->DrawSubset(0);
#endif

		m_pBumpMap->EndPass();
		m_pBumpMap->End();

		m_pd3dDevice->SetTexture(1, NULL);

		m_pd3dDevice->EndScene();

		hr = m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		//デバイスロストのチェック
		switch (hr)
		{
			//デバイスロスト
		case D3DERR_DEVICELOST:
			RenderOK = false;
			break;

			//内部ドライバーエラー
		case D3DERR_DRIVERINTERNALERROR:
			return FALSE;
			break;

			//メソッドの呼び出しが無効です
		case D3DERR_INVALIDCALL:
			return FALSE;
			break;
		}
	}

	return TRUE;
}
#endif
