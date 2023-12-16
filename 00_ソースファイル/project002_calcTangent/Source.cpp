//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "shaderBumpmap.h"

#if 0
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
//D3DPRESENT_PARAMETERS m_d3dParameters;
//D3DCAPS9 Caps;

LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;

CObjectModel*		m_pMeshTeapot = NULL;	// �e�B�[�|�b�g�I�u�W�F�N�g
LPDIRECT3DTEXTURE9	m_pNormalMap = NULL;	// �@���}�b�v
CBumpMap*			m_pBumpMap = NULL;		// �o���v�}�b�v

D3DXVECTOR4 LightPos = D3DXVECTOR4(72.0f, 100.0f, 620.0f, 0.0f);	// ���z�̈ʒu�x�N�g��
D3DXVECTOR4 LightDir;												// ���s�����̌��̕����x�N�g��
D3DXVECTOR4 EyePos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);			// ���_�̈ʒu�x�N�g��

//ID3DXMesh�C���^�[�t�F�[�X���g�p����ꍇ�K�v�Ȃ�
LPDIRECT3DVERTEXDECLARATION9 m_pDecl = NULL;	//���_�V�F�[�_�[
bool RenderOK = false;

//============================================================
//	���C���֐�
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
	//	�����ŃE�B���h�E�̍쐬����
	//	������Direct3D�̏��������s���B
	//****************************************************************
	//m_pd3dDevice->GetDeviceCaps(&Caps);

#if 0
	//�o���v�}�b�v�N���X�̏�����
	m_pBumpMap = new BUMPMAP(m_pd3dDevice);
	m_pBumpMap->Load();
#else
	// �o���v�}�b�v�̐���
	m_pBumpMap = CBumpMap::Create();
#endif

#if 0
	//���b�V���̃��[�h
	m_pMeshTeapot = new CDXUTMesh();
	m_pMeshTeapot->Create(m_pd3dDevice, _T("t-pot.x"));
#else
	// �o���v�}�b�v�̐���
	m_pMeshTeapot = CObjectModel::Create(VEC3_ZERO, VEC3_ZERO);
	m_pMeshTeapot->BindModel(_T("t-pot.x"));
#endif

	//���_�f�[�^���`����(�ڍׂ̓I�����C���}�j���A�����Q��)
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
	//�V�������_���Ɋ�Â����_�f�[�^���Đ�������
	m_pMeshTeapot->SetVertexDecl(m_pd3dDevice, decl);
#else
	//�o���v�}�b�s���O�p�ɒ��_�V�F�[�_�錾���쐬����
	m_pd3dDevice->CreateVertexDeclaration(decl, &m_pDecl);
#endif

#if 0
	//****************************************************************
	//�����}�b�v����@���}�b�v���쐬����(���ӂP)
	//****************************************************************
	D3DSURFACE_DESC desc;
	LPDIRECT3DTEXTURE9 HeightMap = NULL;

	//�����}�b�v���t�@�C�����烍�[�h
	D3DXCreateTextureFromFileEx(m_pd3dDevice,
		_T("height.bmp"),  //�����}�b�v�e�N�X�`���[�̃t�@�C����
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

	//�e�N�X�`�����̓���
	HeightMap->GetLevelDesc(0, &desc);

	//�@���}�b�v�̋�e�N�X�`���[���쐬
	D3DXCreateTexture(m_pd3dDevice,
		desc.Width,
		desc.Height,
		1,
		0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pNormalMap);

	//�@���}�b�v���쐬
	D3DXComputeNormalMap(m_pNormalMap,      //�o�͂���@���}�b�v�e�N�X�`���[�̃|�C���^�B
		HeightMap,         //���͂��鍂���}�b�v�e�N�X�`���[�̃|�C���^�B
		NULL,              //256�F�̃J���[�p���b�g���B
		0,                 //�@���}�b�v�����̒萔�B�@���}�b�v�̎g�p���@�ɂ��ύX����B
		D3DX_CHANNEL_RED,  //�����}�b�v�ŎQ�Ƃ���F�����B
		1.0f               //����������Z����萔�l�B�l��傫������قǍ����Ȃ�B
	);
	SAFE_RELEASE(HeightMap);
#else

	//****************************************************************
	//	�쐬�ς݂̖@���}�b�v�����[�h����
	//****************************************************************   
	D3DXCreateTextureFromFileEx
	(
		m_pd3dDevice,
		_T("normal.bmp"),   //�@���}�b�v�e�N�X�`���[�̃t�@�C����
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

	// ���s�����̈ʒu�x�N�g����������x�N�g�����v�Z����
	LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	RenderOK = true;

	//�f�o�C�X������Ƀ��X�g�A����K�v������I�u�W�F�N�g�̏�����
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
//�f�o�C�X�̃��Z�b�g�O�ɊJ�����ׂ��I�u�W�F�N�g
void Invalidate()
{
	m_pBumpMap->Invalidate();
}

//�f�o�C�X�̃��Z�b�g��ɏ��������ׂ��I�u�W�F�N�g
void Restore()
{
	m_pBumpMap->Restore();

	//�Œ�@�\�p�C�v���C�����C�e�B���O��ݒ肷��
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

// ���b�Z�[�W���[�v����R�[�������֐�
BOOL MainLoop(HWND HWnd)
{
	HRESULT hr;

	//�����_�����O�s�\
	if (RenderOK == false)
	{
		hr = m_pd3dDevice->TestCooperativeLevel();
		switch (hr)
		{
			//�f�o�C�X�͏������Ă��邪Reset�\
		case D3DERR_DEVICENOTRESET:

			//�J��
			//Invalidate();

			//�f�o�C�X�����Z�b�g����
			//hr = m_pd3dDevice->Reset(&m_d3dParameters);

			switch (hr)
			{
				//�f�o�C�X���X�g
			case D3DERR_DEVICELOST:
				break;

				//�����h���C�o�[�G���[
			case D3DERR_DRIVERINTERNALERROR:
				return FALSE;
				break;

				//���\�b�h�̌Ăяo���������ł�
			case D3DERR_INVALIDCALL:
				return FALSE;
				break;

			case S_OK:

				//������
				//Restore();

				RenderOK = true;
			}
			break;
		}
	}

	//�����_�����O�\
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

		//�ˉe���W�ϊ�
		D3DXMatrixPerspectiveFovLH(&matProj,
			D3DX_PI / 4.0f,
			4.0f / 3.0f,
			0.1f,
			1000.0f
		);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		//�r���[���W�ϊ�
		D3DXMatrixIdentity(&matView);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		//���[���h���W�ϊ�
		D3DXMatrixIdentity(&matWorld);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

#if 0
		//�e�B�[�|�b�g�̃e�N�X�`���[���X�e�[�W�O�ɃZ�b�g����
		m_pd3dDevice->SetTexture(0, m_pMeshTeapot->m_pTextures[0]);
#endif

		//�@���}�b�v���X�e�[�W�P�ɃZ�b�g����
		m_pd3dDevice->SetTexture(1, m_pNormalMap);

		//���_�V�F�[�_�錾���o���v�}�b�s���O�p�ɐݒ肷��
		m_pd3dDevice->SetVertexDeclaration(m_pDecl);

		//�e�B�[�|�b�g�������_�����O
		m_pBumpMap->Begin();

		//�}�e���A����ݒ肷��
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

		//�f�o�C�X���X�g�̃`�F�b�N
		switch (hr)
		{
			//�f�o�C�X���X�g
		case D3DERR_DEVICELOST:
			RenderOK = false;
			break;

			//�����h���C�o�[�G���[
		case D3DERR_DRIVERINTERNALERROR:
			return FALSE;
			break;

			//���\�b�h�̌Ăяo���������ł�
		case D3DERR_INVALIDCALL:
			return FALSE;
			break;
		}
	}

	return TRUE;
}
#endif
