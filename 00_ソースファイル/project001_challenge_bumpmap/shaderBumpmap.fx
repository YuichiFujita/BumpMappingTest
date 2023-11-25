//============================================================
//
//	バンプマッピングのエフェクトファイル [shaderBumpmap.fx]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	メンバ変数宣言
//************************************************************
float4x4 m_mtxWorldViewProj : WorldViewProjection;	// ワールドビュー射影変換マトリックス

float4	m_LightDir;				// 平行光源の方向ベクトル
float4	m_EyePos;				// 視点位置ベクトル
float4	m_Ambient = 1.0f;		// 環境光
float	m_Specular = 0.0f;		// ハイライトの範囲
float	m_SpecularPower = 0.0f;	// ハイライトの強度

//************************************************************
//	これは何？？？宣言
//************************************************************
sampler s0 : register(s0);	// オブジェクトのテクスチャー
sampler s1 : register(s1);	// 法線マップ

//************************************************************
//	構造体定義
//************************************************************
// 頂点シェーダー出力情報
struct VS_OUTPUT
{
   float4 Pos      : POSITION;  //頂点座標
   float2 Tex      : TEXCOORD0; //テクセル座標
   float3 Eye      : TEXCOORD1; //頂点座標系での視線方向ベクトル
   float3 Light    : TEXCOORD2; //頂点座標系での頂点 → ライト位置ベクトル
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点シェーダー
//============================================================
VS_OUTPUT VS
(
	float3 Pos		: POSITION,		// 頂点座標
	float3 Tangent	: TANGENT0,		// 接線ベクトル
	float3 Binormal	: BINORMAL0,	// 従法線ベクトル
	float3 Normal	: NORMAL,		// 法線ベクトル
	float2 Tex		: TEXCOORD0		// テクセル
)
{
	// 変数を宣言
	VS_OUTPUT Out;

	Out.Pos = mul( float4( Pos, 1.0f ), m_WVP );
	Out.Tex = Tex;

	// 視線ベクトルを計算
	float3 Eye = normalize( m_EyePos.xyz - Pos.xyz );

	// 視線ベクトルを頂点座標系に変換する
	Out.Eye.x = dot( Eye, Tangent );
	Out.Eye.y = dot( Eye, Binormal );
	Out.Eye.z = dot( Eye, Normal );
	Out.Eye   = normalize( Out.Eye );

	// 頂点座標 -> ライトの位置ベクトル
	float3 Light = -m_LightDir.xyz;

	// ライトベクトルを頂点座標系に変換する
	Out.Light.x = dot( Light, Tangent );
	Out.Light.y = dot( Light, Binormal );
	Out.Light.z = dot( Light, Normal );
	Out.Light   = normalize( Out.Light );

	// 頂点情報を返す
	return Out;
}

//============================================================
//	ピクセルシェーダー
//============================================================
float4 PS( VS_OUTPUT In ) : COLOR
{
	// 法線マップを参照し、法線を取得する
	// 法線マップは 0.0f ～ 1.0f の範囲に保存してあるので -1.0f ～ 1.0f の範囲に変換する
	float3 Normal = 2.0f * tex2D( s1, In.Tex ).xyz - 1.0f;

	// フォンシェーディングによるスペキュラーの色を計算する
	// ハーフベクトルの計算
	float3 H = normalize( In.Light + In.Eye );

	// スペキュラーカラーを計算する
	float S = pow( max( 0.0f, dot( Normal, H ) ), m_Specular ) * m_SpecularPower;

	// 合成する
	return tex2D( s0, In.Tex ) * max( m_Ambient, dot( Normal, In.Light ) ) + S;
}

//============================================================
//	テクニック関数
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
