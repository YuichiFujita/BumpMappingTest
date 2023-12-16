//============================================================
//
//	�o���v�}�b�s���O���� [shaderBumpmap.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderBumpmap.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* EFFECT_FX = "shaderBumpmap.fx";	// �o���v�}�b�v�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CBumpMap *CBumpMap::m_pBumpMap = nullptr;	// �o���v�}�b�s���O

//************************************************************
//	�e�N���X [CBumpMap] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBumpMap::CBumpMap()
{
	// �����o�ϐ����N���A
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBumpMap::~CBumpMap()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBumpMap::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;
	HRESULT hr;

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	LPD3DXBUFFER pErr = nullptr;	// �R���p�C���G���[���

	// �����o�ϐ���������
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^

	// 
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{ // ���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�̃o�[�W����������g�p����o�[�W�����ȏ�̏ꍇ

		// �G�t�F�N�g�t�@�C���̓Ǎ�
		hr = D3DXCreateEffectFromFile
		( // ����
			pDevice,	// �f�o�C�X�ւ̃|�C���^
			EFFECT_FX,	// �G�t�F�N�g�t�@�C��
			nullptr,	// �v���v���Z�b�T��`
			nullptr,	// �C���N���[�h����
			0,			// �Ǎ��I�v�V�����t���O
			nullptr,	// �O���[�o���ϐ��C���^�[�t�F�[�X
			&m_pEffect,	// �G�t�F�N�g�C���^�[�t�F�[�X
			&pErr		// �R���p�C���G���[���
		);
		if (SUCCEEDED(hr))
		{ // �Ǎ��ɐ��������ꍇ

			// ����������
			m_pTechnique		= m_pEffect->GetTechniqueByName("TShader");
			m_mtxWorldViewProj	= m_pEffect->GetParameterByName(nullptr, "m_mtxWorldViewProj");
			m_pLightDir			= m_pEffect->GetParameterByName(nullptr, "m_LightDir");
			m_pEyePos			= m_pEffect->GetParameterByName(nullptr, "m_EyePos");
			m_pAmbient			= m_pEffect->GetParameterByName(nullptr, "m_Ambient");
			m_pSpecularPower	= m_pEffect->GetParameterByName(nullptr, "m_SpecularPower");
			m_pSpecular			= m_pEffect->GetParameterByName(nullptr, "m_Specular");

			m_pEffect->SetTechnique(m_pTechnique);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -2;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBumpMap::Uninit(void)
{
	// �G�t�F�N�g�̔j��
	SAFE_RELEASE(m_pEffect);
}

//============================================================
//	�J�n����
//============================================================
void CBumpMap::Begin(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProjection);

		// �J�n
		m_pEffect->Begin(nullptr, 0);
	}
}

//============================================================
//	�p�X�J�n����
//============================================================
void CBumpMap::BeginPass(const BYTE pass)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �p�X�̊J�n
		m_pEffect->BeginPass(pass);
	}
}

//============================================================
//	�p�X�I������
//============================================================
void CBumpMap::EndPass(void)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �p�X�̏I��
		m_pEffect->EndPass();
	}
}

//============================================================
//	�I������
//============================================================
void CBumpMap::End(void)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �I��
		m_pEffect->End();
	}
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CBumpMap::SetAmbient(float Ambient)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		D3DXVECTOR4 A;
		A = D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f);
		m_pEffect->SetVector(m_pAmbient, &A);
	}
	else
	{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

		D3DMATERIAL9 tempMat;
		pDevice->GetMaterial(&tempMat);
		tempMat.Ambient.r = Ambient;
		tempMat.Ambient.g = Ambient;
		tempMat.Ambient.b = Ambient;
		tempMat.Ambient.a = 1.0f;
		pDevice->SetMaterial(&tempMat);
	}
}

//============================================================
//	���ˌ��̐ݒ菈��
//============================================================
void CBumpMap::SetSpecular(const float fSpecular)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// ���ˌ��̐ݒ�
		m_pEffect->SetFloat(m_pSpecular, fSpecular);
	}
}

//============================================================
//	���˃n�C���C�g�̐ݒ菈��
//============================================================
void CBumpMap::SetSpecularPower(const float fPower)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// ���˃n�C���C�g�̐ݒ�
		m_pEffect->SetFloat(m_pSpecularPower, fPower);
	}
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CBumpMap::SetMatrix
(
	D3DXMATRIX* pMtxWorld,		// ���[���h�}�g���b�N�X
	D3DXVECTOR4* pPosCamera,	// �J�����ʒu
	D3DXVECTOR4* pDirLight		// ���C�g����
)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		D3DXMATRIX m, m1;
		D3DXVECTOR4 LightDir;
		D3DXVECTOR4 v;

		m = (*pMtxWorld) * m_mtxView * m_mtxProjection;
		m_pEffect->SetMatrix(m_mtxWorldViewProj, &m);

		// �J�����ʒu
		m1 = (*pMtxWorld) * m_mtxView;
		D3DXMatrixInverse(&m1, nullptr, &m1);
		D3DXVec4Transform(&v, pPosCamera, &m1);
		m_pEffect->SetVector(m_pEyePos, &v);

		// Light
		LightDir = *pDirLight;
		D3DXMatrixInverse(&m1, nullptr, pMtxWorld);
		D3DXVec4Transform(&v, &LightDir, &m1);
		D3DXVec3Normalize((D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v);
		m_pEffect->SetVector(m_pLightDir, &v);
	}
	else
	{ // �G�t�F�N�g���g�p����Ă��Ȃ��ꍇ

		pDevice->SetTransform(D3DTS_WORLD, pMtxWorld);
	}
}

//============================================================
//	��ԕύX�̓`�B����
//============================================================
void CBumpMap::CommitChanges(void)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// ��ԕύX�̓`�B
		m_pEffect->CommitChanges();
	}
}

//============================================================
//	�G�t�F�N�g�g�p�\�󋵂̎擾����
//============================================================
bool CBumpMap::IsEffectOK(void) const
{
	// �G�t�F�N�g�|�C���^�̎g�p�󋵂�Ԃ�
	return (m_pEffect != nullptr) ? true : false;
}

//============================================================
//	�G�t�F�N�g�|�C���^�擾����
//============================================================
LPD3DXEFFECT CBumpMap::GetEffect(void) const
{
	// �G�t�F�N�g��Ԃ�
	return m_pEffect;
}

//============================================================
//	��������
//============================================================
CBumpMap *CBumpMap::Create(void)
{
	// ��O����
	assert(m_pBumpMap == nullptr);	// �C���X�^���X�g�p��

	// �o���v�}�b�s���O�̐���
	m_pBumpMap = new CBumpMap;	// �o���v�}�b�s���O�̏��
	if (m_pBumpMap == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �o���v�}�b�s���O�̏�����
		if (FAILED(m_pBumpMap->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �o���v�}�b�s���O�̔j��
			delete m_pBumpMap;
			m_pBumpMap = nullptr;

			assert(false);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return m_pBumpMap;
	}
}

//============================================================
//	�擾����
//============================================================
CBumpMap *CBumpMap::GetInstance(void)
{
	// ��O����
	assert(m_pBumpMap != nullptr);	// �C���X�^���X���g�p

	// �o���v�}�b�s���O�̃|�C���^��Ԃ�
	return m_pBumpMap;
}

//============================================================
//	�j������
//============================================================
HRESULT CBumpMap::Release(void)
{
	if (m_pBumpMap != nullptr)
	{ // �o���v�}�b�s���O���g�p���̏ꍇ

		// �o���v�}�b�s���O�̏I��
		m_pBumpMap->Uninit();

		// �������J��
		delete m_pBumpMap;
		m_pBumpMap = nullptr;

		// ������Ԃ�
		return S_OK;
	}

	// ���s��Ԃ�
	assert(false);	// �j���ς�
	return E_FAIL;
}
