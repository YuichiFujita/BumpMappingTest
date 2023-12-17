//============================================================
//
//	�V�F�[�_�[�}�l�[�W���[���� [shaderManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "shaderManager.h"
#include "shaderBumpmap.h"

//************************************************************
//	�e�N���X [CShaderManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CShaderManager::CShaderManager()
{
	// �����o�ϐ����N���A
	m_pBumpMap = nullptr;	// �o���v�}�b�s���O
}

//============================================================
//	�f�X�g���N�^
//============================================================
CShaderManager::~CShaderManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CShaderManager::Init(void)
{
	// �����o�ϐ���������
	m_pBumpMap = nullptr;	// �o���v�}�b�s���O

	// �o���v�}�b�s���O�̐���
	m_pBumpMap = CBumpMap::Create();
	if (m_pBumpMap == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
HRESULT CShaderManager::Uninit(void)
{
	// �o���v�}�b�s���O�̔j��
	if (FAILED(CBumpMap::Release()))
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	��������
//============================================================
CShaderManager *CShaderManager::Create(void)
{
	// �V�F�[�_�[�}�l�[�W���[�̐���
	CShaderManager *pShaderManager = new CShaderManager;	// �V�F�[�_�[�}�l�[�W���[�̏��
	if (pShaderManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �V�F�[�_�[�}�l�[�W���[�̏�����
		if (FAILED(pShaderManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �V�F�[�_�[�}�l�[�W���[�̔j��
			delete pShaderManager;
			pShaderManager = nullptr;

			assert(false);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pShaderManager;
	}
}

//============================================================
//	�j������
//============================================================
HRESULT CShaderManager::Release(CShaderManager *&prShaderManager)
{
	if (prShaderManager != nullptr)
	{ // �g�p���̏ꍇ

		// �V�F�[�_�[�}�l�[�W���[�̏I��
		prShaderManager->Uninit();

		// �������J��
		delete prShaderManager;
		prShaderManager = nullptr;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
