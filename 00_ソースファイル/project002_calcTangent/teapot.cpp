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
#include "texture.h"
#include "shaderBumpmap.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[] =	// �e�N�X�`���萔
	{
		"data\\TEXTURE\\normal5.png",		// �@���e�N�X�`��
	};
	const char *MODEL_FILE[] =		// ���f���萔
	{
		"data\\MODEL\\TEAPOT\\teapot005.x",	// �e�B�[�|�b�g
	};

	const int PRIORITY = 0;	// �e�B�[�|�b�g�̗D�揇��
}

//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
namespace
{
	D3DXVECTOR4 DIR_LIGHT	= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);	// ���s�����̌��̕����x�N�g��
	D3DXVECTOR4 POS_EYE		= D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);	// ���_�̈ʒu�x�N�g��
}

//************************************************************
//	�q�N���X [CTeapot] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTeapot::CTeapot() : CObjectModel(CObject::LABEL_TEAPOT, PRIORITY)
{

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
	// �ϐ��z���錾
	D3DVERTEXELEMENT9 decl[] =	// ���_�f�[�^��`
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���f����o�^�E����
	BindModel(MODEL_FILE[MODEL_TEAPOT]);

	// ���f���̒��_�f�[�^��ϊ�
	if (FAILED(SetVertexDecl(CManager::GetInstance()->GetRenderer()->GetDevice(), decl)))
	{ // �ϊ��Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTeapot::Uninit(void)
{
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
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`�����
	CBumpMap *pBumpMap = CBumpMap::GetInstance();				// �o���v�}�b�s���O���

	if (pDevice == nullptr || pTexture == nullptr || pBumpMap == nullptr)
	{ // ��񂪖������̂��������ꍇ

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw();

		// �����𔲂���
		assert(false);
		return;
	}

	if (pBumpMap->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �X�e�[�W1�̃e�N�X�`����ݒ�
		pDevice->SetTexture(1, pTexture->GetTexture(pTexture->Regist(TEXTURE_FILE[TEXTURE_NORMAL])));	// �@���e�N�X�`��

		// �`��J�n
		pBumpMap->Begin();

		// �}�e���A������ݒ�
		pBumpMap->SetAmbient(0.75f);
		pBumpMap->SetSpecular(5.5f);
		pBumpMap->SetSpecularPower(0.25f);
		pBumpMap->SetMatrix(GetPtrMtxWorld(), &POS_EYE, &DIR_LIGHT);

		// ��ԕύX�̓`�B
		pBumpMap->CommitChanges();

		// �p�X��ݒ�
		pBumpMap->BeginPass(0);

		// �I�u�W�F�N�g���f���̕`��
		CObjectModel::Draw();

		// �`��I��
		pBumpMap->EndPass();
		pBumpMap->End();

		// �X�e�[�W1�̃e�N�X�`����������
		pDevice->SetTexture(1, nullptr);
	}
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
