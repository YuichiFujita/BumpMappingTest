//============================================================
//
//	�o���v�}�b�s���O�̃G�t�F�N�g�t�@�C�� [shaderBumpmap.fx]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�����o�ϐ��錾
//************************************************************
float4x4 m_mtxWorldViewProj : WorldViewProjection;	// ���[���h�r���[�ˉe�ϊ��}�g���b�N�X

float4	m_LightDir;				// ���s�����̕����x�N�g��
float4	m_EyePos;				// ���_�ʒu�x�N�g��
float4	m_Ambient = 1.0f;		// ����
float	m_Specular = 0.0f;		// �n�C���C�g�͈̔�
float	m_SpecularPower = 0.0f;	// �n�C���C�g�̋��x

//************************************************************
//	����͉��H�H�H�錾
//************************************************************
sampler s0 : register(s0);	// �I�u�W�F�N�g�̃e�N�X�`���[
sampler s1 : register(s1);	// �@���}�b�v

//************************************************************
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
   float4 Pos      : POSITION;  //���_���W
   float2 Tex      : TEXCOORD0; //�e�N�Z�����W
   float3 Eye      : TEXCOORD1; //���_���W�n�ł̎��������x�N�g��
   float3 Light    : TEXCOORD2; //���_���W�n�ł̒��_ �� ���C�g�ʒu�x�N�g��
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_�V�F�[�_�[
//============================================================
VS_OUTPUT VS
(
	float3 Pos		: POSITION,		// ���_���W
	float3 Tangent	: TANGENT0,		// �ڐ��x�N�g��
	float3 Binormal	: BINORMAL0,	// �]�@���x�N�g��
	float3 Normal	: NORMAL,		// �@���x�N�g��
	float2 Tex		: TEXCOORD0		// �e�N�Z��
)
{
	// �ϐ���錾
	VS_OUTPUT Out;

	Out.Pos = mul( float4( Pos, 1.0f ), m_WVP );
	Out.Tex = Tex;

	// �����x�N�g�����v�Z
	float3 Eye = normalize( m_EyePos.xyz - Pos.xyz );

	// �����x�N�g���𒸓_���W�n�ɕϊ�����
	Out.Eye.x = dot( Eye, Tangent );
	Out.Eye.y = dot( Eye, Binormal );
	Out.Eye.z = dot( Eye, Normal );
	Out.Eye   = normalize( Out.Eye );

	// ���_���W -> ���C�g�̈ʒu�x�N�g��
	float3 Light = -m_LightDir.xyz;

	// ���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	Out.Light.x = dot( Light, Tangent );
	Out.Light.y = dot( Light, Binormal );
	Out.Light.z = dot( Light, Normal );
	Out.Light   = normalize( Out.Light );

	// ���_����Ԃ�
	return Out;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
float4 PS( VS_OUTPUT In ) : COLOR
{
	// �@���}�b�v���Q�Ƃ��A�@�����擾����
	// �@���}�b�v�� 0.0f �` 1.0f �͈̔͂ɕۑ����Ă���̂� -1.0f �` 1.0f �͈̔͂ɕϊ�����
	float3 Normal = 2.0f * tex2D( s1, In.Tex ).xyz - 1.0f;

	// �t�H���V�F�[�f�B���O�ɂ��X�y�L�����[�̐F���v�Z����
	// �n�[�t�x�N�g���̌v�Z
	float3 H = normalize( In.Light + In.Eye );

	// �X�y�L�����[�J���[���v�Z����
	float S = pow( max( 0.0f, dot( Normal, H ) ), m_Specular ) * m_SpecularPower;

	// ��������
	return tex2D( s0, In.Tex ) * max( m_Ambient, dot( Normal, In.Light ) ) + S;
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
