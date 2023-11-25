//============================================================
//
//	�Ǐ��� [wall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"

//************************************************************
//	�}�N����`
//************************************************************
#define WALL_PRIO	(0)	// �ǂ̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CWall::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\wall000.png",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CWall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, WALL_PRIO)
{
	// �����o�ϐ����N���A
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^
}

//============================================================
//	�f�X�g���N�^
//============================================================
CWall::~CWall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWall::Init(void)
{
	// �����o�ϐ���������
	m_pEffect = nullptr;	// �G�t�F�N�g�|�C���^

	// �I�u�W�F�N�g���b�V���E�H�[���̏�����
	if (FAILED(CObjectMeshWall::Init()))
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
void CWall::Uninit(void)
{
	// �G�t�F�N�g�̔j��
	SAFE_RELEASE(m_pEffect);

	// �I�u�W�F�N�g���b�V���E�H�[���̏I��
	CObjectMeshWall::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWall::Update(void)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̍X�V
	CObjectMeshWall::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CWall::Draw(void)
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

	// �I�u�W�F�N�g���b�V���E�H�[���̕`��
	CObjectMeshWall::Draw();

	// �`��I��
	m_pEffect->EndPass();
	m_pEffect->End();
}

//============================================================
//	��������
//============================================================
CWall *CWall::Create
(
	const ETexture texture,		// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �ϐ���錾
	int nTextureID;	// �e�N�X�`���C���f�b�N�X

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CWall *pWall = nullptr;	// �ǐ����p

	if (pWall == nullptr)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pWall = new CWall;	// ��
	}
	else { assert(false); return nullptr; }	// �g�p��

	if (pWall != nullptr)
	{ // �m�ۂɐ������Ă���ꍇ

		// �ǂ̏�����
		if (FAILED(pWall->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pWall;
			pWall = nullptr;

			// ���s��Ԃ�
			return nullptr;
		}

		// �e�N�X�`����o�^
		nTextureID = pTexture->Regist(mc_apTextureFile[texture]);

		// �e�N�X�`��������
		pWall->BindTexture(nTextureID);

		// �ʒu��ݒ�
		pWall->SetVec3Position(rPos);

		// ������ݒ�
		pWall->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pWall->SetVec2Sizing(rSize);

		// �F��ݒ�
		pWall->SetColor(rCol);

		// �J�����O��ݒ�
		pWall->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pWall->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pWall->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pWall;
			pWall = nullptr;

			// ���s��Ԃ�
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pWall;
	}
	else { assert(false); return nullptr; }	// �m�ێ��s
}
