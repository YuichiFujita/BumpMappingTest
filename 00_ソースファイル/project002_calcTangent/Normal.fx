float4x4 g_worldMat;
float4x4 g_viewMat;
float4x4 g_worldView;
float4x4 g_projMat;
float3 g_LightLocalDirect  = float3( 1, 0, 0 );

float3 cameraVec = float3( 0, 0, 1 );

texture g_diffuseMap;
sampler diffuseSampler = sampler_state {
	texture = <g_diffuseMap>;
	MipFilter = LINEAR; 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
};

texture g_normalMap;
sampler normalSampler = sampler_state {
	texture = <g_normalMap>;
	MipFilter = LINEAR; 
	MinFilter = LINEAR; 
	MagFilter = LINEAR; 
};

struct VS_OUT {
	float4 pos    : POSITION;
	float4 uv     : TEXCOORD0;
	float4 lightTangentDirect : TEXCOORD3;
};


// 頂点を射影空間へ移動
float4 TransVertex(
	float4 vertex,
	float4x4 worldMat,
	float4x4 viewMat,
	float4x4 projMat )
{
	vertex = mul( vertex, worldMat );
	vertex = mul( vertex, viewMat );
	vertex = mul( vertex, projMat );
	return vertex;
}

// 接空間行列の逆行列を算出
float4x4 InvTangentMatrix(
	float3 tangent,
	float3 binormal,
	float3 normal )
{
	float4x4 mat = { float4(tangent, 0.0f ), float4(binormal, 0.0f), float4(normal, 0.0f), {0.0f, 0.0f, 0.0f, 1.0f} };
	return transpose( mat );  // 転置
}


// 頂点シェーダ
VS_OUT NormalMap_VS(
	float4 inLocalPos : POSITION,
	float3 inTangent : TANGENT,
	float3 inBinormal : BINORMAL,
	float3 inLocalNormal : NORMAL, 
	float4 inUV : TEXCOORD0 )
{
	VS_OUT Out = (VS_OUT)0;

	// 頂点変換
	Out.pos = TransVertex( inLocalPos, g_worldMat, g_viewMat, g_projMat );

	// 接空間行列の逆行列を算出
	float4x4 invTangentMat = InvTangentMatrix( normalize(inTangent), normalize(inBinormal), normalize(inLocalNormal) );

	// ライトベクトルを接空間に移動
	Out.lightTangentDirect = mul( float4(-g_LightLocalDirect, 1.0f), invTangentMat );

	Out.uv     = inUV;
	return Out;
}


float4 NormalMap_PS( float4 inUV : TEXCOORD0, float3 inLightTangentDirect : TEXCOORD3 ) : COLOR0
{
	float3 normalColor = tex2D( normalSampler, inUV );
	float3 normalVec   = 2.0f * normalColor - 1.0f;
	normalVec = normalize( normalVec );
	
	float3 bright = dot( normalize(inLightTangentDirect), normalVec );
	bright = max( 0.0f, bright );
	float4 diffuseColor = tex2D( diffuseSampler, inUV );
	
	return float4( bright * diffuseColor.xyz, 1.0f );
}

technique NormalMap
{
	pass P0
	{
		VertexShader = compile vs_2_0 NormalMap_VS();
		PixelShader = compile ps_2_0 NormalMap_PS();
	}	
}