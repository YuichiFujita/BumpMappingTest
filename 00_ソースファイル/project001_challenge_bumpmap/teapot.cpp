//============================================================
//
//	�e�B�[�|�b�g���� [teapot.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "teapot.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "shaderBumpmap.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TEAPOT_PRIO	(0)	// �e�B�[�|�b�g�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTeapot::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\TEAPOT\\teapot001.x",	// �e�B�[�|�b�g
};

//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
LPDIRECT3DVERTEXDECLARATION9 m_pDecl = NULL;	// ���_�V�F�[�_�[

LPDIRECT3DTEXTURE9	m_pNormalMap	= nullptr;	// �@���}�b�v
CBumpMap*			m_pBumpMap		= nullptr;	// �o���v�}�b�v

D3DXVECTOR4 LightPos	= D3DXVECTOR4(72.0f, 100.0f, 620.0f, 0.0f);	// ���z�̈ʒu�x�N�g��
D3DXVECTOR4 LightDir	= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);		// ���s�����̌��̕����x�N�g��
D3DXVECTOR4 EyePos		= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);		// ���_�̈ʒu�x�N�g��

//************************************************************
//	�q�N���X [CTeapot] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, TEAPOT_PRIO)
{
	// �����o�ϐ����N���A
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTeapot::~CTeapot()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTeapot::Init(void)
{
	// �����o�ϐ���������
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(mc_apModelFile[MODEL_TEAPOT]);

	// �o���v�}�b�v�̐���
	m_pBumpMap = CBumpMap::Create();
	if (m_pBumpMap == nullptr)
	{
		return E_FAIL;
	}

	// ���_�f�[�^���`����(�ڍׂ̓I�����C���}�j���A�����Q��)
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	//LPD3DXMESH *ppMesh = NULL;	// ���b�V�� (���_���) �ւ̃|�C���^
	//CModel::SModel model = GetModelData();

	// �V�������_���Ɋ�Â����_�f�[�^���Đ�������
	//GetModelData().pMesh->SetVertexDecl(m_pd3dDevice, decl);
	if (FAILED(SetVertexDecl(CManager::GetInstance()->GetRenderer()->GetDevice(), decl)))
	//if (FAILED(model.pMesh->CloneMesh(model.pMesh->GetOptions(), &decl[0], CManager::GetInstance()->GetRenderer()->GetDevice(), &model.pMesh)))
	{
		return E_FAIL;
	}

	//model.pMesh = *ppMesh;
	//SetModelData(model);

	//// �o���v�}�b�s���O�p�ɒ��_�V�F�[�_�錾���쐬����
	//if (FAILED(CManager::GetInstance()->GetRenderer()->GetDevice()->CreateVertexDeclaration(decl, &m_pDecl)))
	//{
	//	return E_FAIL;
	//}

	// �@���}�b�v�����[�h
	HRESULT hr = D3DXCreateTextureFromFileEx
	(
		CManager::GetInstance()->GetRenderer()->GetDevice(),
		_T("data\\TEXTURE\\normal3.png"),
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
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ���s�����̈ʒu�x�N�g����������x�N�g�����v�Z����
	LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTeapot::Uninit(void)
{
	// �o���v�}�b�v�̔j��
	CBumpMap::Release(m_pBumpMap);

	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTeapot::Update(void)
{
	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CTeapot::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	// �J�������
	CCamera::SCamera cameraInfo = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN);	// �J�����擾

	// �T���v���[�X�e�[�g��ݒ�
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//LightPos = D3DXVECTOR4(cameraInfo.posV.x, cameraInfo.posV.y, cameraInfo.posV.z, 0.0f);	// ���z�̈ʒu�x�N�g��
	//EyePos = D3DXVECTOR4(cameraInfo.posV.x, cameraInfo.posV.y, cameraInfo.posV.z, 1.0f);	// ���_�̈ʒu�x�N�g��

	// ���s�����̈ʒu�x�N�g����������x�N�g�����v�Z����
	//LightDir = D3DXVECTOR4(-LightPos.x, -LightPos.y, -LightPos.z, 0.0f);
	//D3DXVec3Normalize((D3DXVECTOR3*)&LightDir, (D3DXVECTOR3*)&LightDir);

	// ���[���h�}�g���b�N�X
	D3DXMATRIX mtxWorld = GetMtxWorld();

	// �@���}�b�v���X�e�[�W�P�ɃZ�b�g����
	pDevice->SetTexture(1, m_pNormalMap);

	// ���_�V�F�[�_�錾���o���v�}�b�s���O�p�ɐݒ肷��
	//pDevice->SetVertexDeclaration(m_pDecl);

	// �`��J�n
	m_pBumpMap->Begin();

	// �}�e���A����ݒ�
	m_pBumpMap->SetAmbient(1.5f);
	m_pBumpMap->SetSpecular(0.5f);
	m_pBumpMap->SetSpecularPower(1.0f);
	m_pBumpMap->SetMatrix(&mtxWorld, &EyePos, &LightDir);

	// �p�X��ݒ�
	m_pBumpMap->BeginPass(0);

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();

	// �X�e�[�W�P������������
	pDevice->SetTexture(1, NULL);

	// �`��I��
	m_pBumpMap->EndPass();
	m_pBumpMap->End();
}

//============================================================
//	��������
//============================================================
CTeapot *CTeapot::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CTeapot *pTeapot = nullptr;	// �e�B�[�|�b�g�����p

	if (pTeapot == nullptr)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pTeapot = new CTeapot;	// �e�B�[�|�b�g
	}
	else { assert(false); return nullptr; }	// �g�p��

	if (pTeapot != nullptr)
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�B�[�|�b�g�̏�����
		if (FAILED(pTeapot->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pTeapot;
			pTeapot = nullptr;

			// ���s��Ԃ�
			return nullptr;
		}

		// �ʒu��ݒ�
		pTeapot->SetVec3Position(rPos);

		// ������ݒ�
		pTeapot->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// �m�ێ��s
}
