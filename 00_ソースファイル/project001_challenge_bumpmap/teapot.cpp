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

//************************************************************
//	�}�N����`
//************************************************************
#define TEAPOT_PRIO	(0)	// �e�B�[�|�b�g�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CTeapot::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\TEAPOT\\teapot000.x",	// �e�B�[�|�b�g
};

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

	// �G�t�F�N�g�t�@�C���̓ǂݍ���
	D3DXCreateEffectFromFile
	( // ����
		CManager::GetInstance()->GetRenderer()->GetDevice(),	// �f�o�C�X�ւ̃|�C���^
		_T("first_create_mat.fx"),	// �G�t�F�N�g�t�@�C��
		nullptr,	// �v���v���Z�b�T��`
		nullptr,	// �C���N���[�h����
		0,			// �Ǎ��I�v�V�����t���O
		nullptr,	// �O���[�o���ϐ��C���^�[�t�F�[�X
		&m_pEffect,	// �G�t�F�N�g�C���^�[�t�F�[�X
		nullptr		// �R���p�C���G���[���
	);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTeapot::Uninit(void)
{
	// �G�t�F�N�g�̔j��
	SAFE_RELEASE(m_pEffect);

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
	// �J�������
	CCamera::SCamera cameraInfo = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN);	// �J�����擾

	// �r���[�ϊ��E�ˉe�ϊ�
	D3DXMATRIX View = cameraInfo.mtxView;
	D3DXMATRIX Projection = cameraInfo.mtxProjection;

	// ���[���h�}�g���b�N�X�E���[���h�r���[�ˉe�ϊ��s��
	D3DXMATRIX mtxWorld = GetMtxWorld();
	D3DXMATRIX mtxWorldViewProj;

	// �G�t�F�N�g���̃��[���h�r���[�ˉe�ϊ��s���ݒ�
	mtxWorldViewProj = mtxWorld * View * Projection;
	m_pEffect->SetMatrix("mtxWorldViewProj", &mtxWorldViewProj);

	// �`��J�n
	m_pEffect->SetTechnique("BasicTec");
	UINT numPass;
	m_pEffect->Begin(&numPass, 0);
	m_pEffect->BeginPass(0);

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();

	// �`��I��
	m_pEffect->EndPass();
	m_pEffect->End();
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

		// ���f����o�^�E����
		pTeapot->BindModel(mc_apModelFile[MODEL_TEAPOT]);

		// �ʒu��ݒ�
		pTeapot->SetVec3Position(rPos);

		// ������ݒ�
		pTeapot->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pTeapot;
	}
	else { assert(false); return nullptr; }	// �m�ێ��s
}
