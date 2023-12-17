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
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���
	LPD3DXBUFFER pError = nullptr;	// �R���p�C���G���[���

	// �����o�ϐ���������
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^

	// �n�[�h�E�F�A�@�\���擾
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
	&&  caps.PixelShaderVersion  >= D3DPS_VERSION(2, 0))
	{ // ���_�E�s�N�Z���V�F�[�_�̃o�[�W�������g�p�\�ȏꍇ

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
			&pError		// �R���p�C���G���[���
		);
		if (SUCCEEDED(hr))
		{ // �Ǎ��ɐ��������ꍇ

			// �e�N�j�b�N�֐����擾
			m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
			m_pEffect->SetTechnique(m_pTechnique);	// �G�t�F�N�g�Ƀe�N�j�b�N�֐���ݒ�

			// �O���[�o���ϐ����擾
			m_pMtxWorldViewProj	= m_pEffect->GetParameterByName(nullptr, "m_mtxWorldViewProj");	// ���[���h�r���[�ˉe�ϊ��}�g���b�N�X
			m_pLightDir			= m_pEffect->GetParameterByName(nullptr, "m_LightDir");			// ���s�����̕����x�N�g��
			m_pEyePos			= m_pEffect->GetParameterByName(nullptr, "m_EyePos");			// ���_�ʒu�x�N�g��
			m_pAmbient			= m_pEffect->GetParameterByName(nullptr, "m_Ambient");			// ����
			m_pSpecular			= m_pEffect->GetParameterByName(nullptr, "m_Specular");			// �n�C���C�g�͈̔�
			m_pSpecularPower	= m_pEffect->GetParameterByName(nullptr, "m_SpecularPower");	// �n�C���C�g�̋��x
		}
		else
		{ // �Ǎ��Ɏ��s�����ꍇ

			// �ǂݍ��ݎ��s��Ԃ�
			return E_FAIL;
		}
	}
	else
	{ // �o�[�W�������g�p�s�ȏꍇ

		// �Â��o�[�W�����ɂ�鎸�s��Ԃ�
		return E_FAIL;
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
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

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
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �ϐ���錾
		D3DXVECTOR4 ambient = D3DXVECTOR4(Ambient, Ambient, Ambient, 1.0f);	// ����

		// �G�t�F�N�g�Ɋ�����ݒ�
		m_pEffect->SetVector(m_pAmbient, &ambient);
	}
}

//============================================================
//	���ˌ��̐ݒ菈��
//============================================================
void CBumpMap::SetSpecular(const float fSpecular)
{
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �G�t�F�N�g�ɔ��ˌ���ݒ�
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

		// �G�t�F�N�g�ɔ��˃n�C���C�g��ݒ�
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
	if (m_pEffect != nullptr)
	{ // �G�t�F�N�g���g�p����Ă���ꍇ

		// �ϐ���錾
		D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
		D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
		D3DXMATRIX mtxWorldView;	// ���[���h�r���[�}�g���b�N�X
		D3DXMATRIX mtxWVP;			// ���[���h�r���[�ˉe�ϊ��}�g���b�N�X
		D3DXVECTOR4 dirLight;		// ���s�����̕����x�N�g��
		D3DXVECTOR4 posEye;			// ���_�ʒu�x�N�g��

		// �|�C���^��錾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���

		// �r���[�}�g���b�N�X�E�v���W�F�N�V�����}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

		// ���[���h�r���[�ˉe�ϊ��}�g���b�N�X���v�Z
		mtxWVP = (*pMtxWorld) * mtxView * mtxProjection;

		// �G�t�F�N�g�Ƀ��[���h�r���[�ˉe�ϊ��}�g���b�N�X��ݒ�
		m_pEffect->SetMatrix(m_pMtxWorldViewProj, &mtxWVP);

		// �J�����̎��_�ʒu�x�N�g�����v�Z
		mtxWorldView = (*pMtxWorld) * mtxView;						// ���[���h�r���[�}�g���b�N�X���v�Z
		D3DXMatrixInverse(&mtxWorldView, nullptr, &mtxWorldView);	// ���[���h�r���[�}�g���b�N�X�̋t�s����v�Z
		D3DXVec4Transform(&posEye, pPosCamera, &mtxWorldView);		// �}�g���b�N�X���x�N�g���ɕϊ�

		// �G�t�F�N�g�Ɏ��_�ʒu�x�N�g����ݒ�
		m_pEffect->SetVector(m_pEyePos, &posEye);

		// ���s�����̕����x�N�g�����v�Z
		dirLight = *pDirLight;
		D3DXMatrixInverse(&mtxWorld, nullptr, pMtxWorld);						// ���[���h�}�g���b�N�X�̋t�s����v�Z
		D3DXVec4Transform(&dirLight, &dirLight, &mtxWorld);						// �}�g���b�N�X���x�N�g���ɕϊ�
		D3DXVec3Normalize((D3DXVECTOR3*)&dirLight, (D3DXVECTOR3*)&dirLight);	// �x�N�g�����O�����ϊ������K��

		// �G�t�F�N�g�ɕ��s�����̕����x�N�g����ݒ�
		m_pEffect->SetVector(m_pLightDir, &dirLight);
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
	// �G�t�F�N�g�|�C���^��Ԃ�
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
