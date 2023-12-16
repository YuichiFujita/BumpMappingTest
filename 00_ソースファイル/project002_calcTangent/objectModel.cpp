//============================================================
//
//	�I�u�W�F�N�g���f������ [objectModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectModel::CObjectModel()
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pMat	= nullptr;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ZERO;	// �g�嗦
	m_nModelID = 0;			// ���f���C���f�b�N�X
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectModel::CObjectModel(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pMat	= nullptr;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ZERO;	// �g�嗦
	m_nModelID = 0;			// ���f���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectModel::~CObjectModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectModel::Init(void)
{
	// �����o�ϐ���������
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pMat	= nullptr;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ONE;		// �g�嗦
	m_nModelID = NONE_IDX;	// ���f���C���f�b�N�X

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectModel::Uninit(void)
{
	// �}�e���A���ւ̃|�C���^��j��
	if (m_pMat != nullptr)
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = nullptr;
	}

	// �I�u�W�F�N�g���f����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectModel::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetTexture(m_modelData.pTextureID[nCntMat]));

		if (m_scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}

		// ���f���̕`��
		m_modelData.pMesh->DrawSubset(nCntMat);

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//============================================================
//	���f���������� (�C���f�b�N�X)
//============================================================
void CObjectModel::BindModel(const int nModelID)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetInstance()->GetModel();	// ���f���ւ̃|�C���^
	if (pModel == nullptr)
	{ // ���f���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (nModelID > NONE_IDX)
	{ // ���f���C���f�b�N�X���g�p�\�ȏꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = nModelID;

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(nModelID);

		// ���}�e���A���̐ݒ�
		SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat);
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���f���������� (�p�X)
//============================================================
void CObjectModel::BindModel(const char *pModelPass)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetInstance()->GetModel();	// ���f���ւ̃|�C���^
	if (pModel == nullptr)
	{ // ���f���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (pModelPass != nullptr)
	{ // ���蓖�Ă郂�f���p�X�����݂���ꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = pModel->Regist(pModelPass);

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(m_nModelID);

		// ���}�e���A���̐ݒ�
		SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat);
	}
	else { assert(false); }	// ���f���p�X����
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CObjectModel::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// �����̊g�嗦����
	m_scale = rScale;
}

//============================================================
//	�g�嗦�擾����
//============================================================
D3DXVECTOR3 CObjectModel::GetVec3Scaling(void) const
{
	// �g�嗦��Ԃ�
	return m_scale;
}

//============================================================
//	�}�e���A���̑S�ݒ菈��
//============================================================
void CObjectModel::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	// �����̃}�e���A����S�}�e���A���ɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = rMat;
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CObjectModel::ResetMaterial(void)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	// �S�}�e���A���ɏ����}�e���A�����Đݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectModel::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObjectModel::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	��������
//============================================================
CObjectModel *CObjectModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �|�C���^��錾
	CObjectModel *pObjectModel = nullptr;		// �I�u�W�F�N�g���f�������p

	if (pObjectModel == nullptr)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectModel = new CObjectModel;	// �I�u�W�F�N�g���f��
	}
	else { assert(false); return nullptr; }	// �g�p��

	if (pObjectModel != nullptr)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���f���̏�����
		if (FAILED(pObjectModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectModel;
			pObjectModel = nullptr;

			// ���s��Ԃ�
			return nullptr;
		}

		// �ʒu��ݒ�
		pObjectModel->SetVec3Position(rPos);

		// ������ݒ�
		pObjectModel->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pObjectModel->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectModel;
	}
	else { assert(false); return nullptr; }	// �m�ێ��s
}

//============================================================
//	�}�e���A���ݒ菈��
//============================================================
void CObjectModel::SetMaterial(const D3DXMATERIAL& rMat, const int nID)
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����ݒ�
		m_pMat[nID] = rMat;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�}�e���A���擾����
//============================================================
D3DXMATERIAL CObjectModel::GetMaterial(const int nID) const
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����Ԃ�
		return m_pMat[nID];
	}
	else
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă���ꍇ

		// �ϐ���錾
		D3DXMATERIAL mat;	// ��O���̃}�e���A��

		// �}�e���A���̃��������N���A
		ZeroMemory(&mat, sizeof(mat));

		// ��O����
		assert(false);

		// ��O���̃}�e���A����Ԃ�
		return mat;
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CObjectModel::SetAlpha(const float fAlpha)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �ϐ���錾
		float fSetAlpha = fAlpha;	// �ݒ肷�铧���x

		// �����x�̕␳
		useful::LimitNum(fSetAlpha, 0.0f, pOriginMat[nCntMat].MatD3D.Diffuse.a);

		// �����x��ݒ�
		m_pMat[nCntMat].MatD3D.Diffuse.a = fSetAlpha;
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CObjectModel::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (m_pMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_pMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CObjectModel::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pOriginMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = pOriginMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectModel::SetModelData(const CModel::SModel& rModel)
{
	// �����̃��f��������
	m_modelData = rModel;
}

//============================================================
//	���f�����擾����
//============================================================
CModel::SModel CObjectModel::GetModelData(void) const
{
	// ���f������Ԃ�
	return m_modelData;
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectModel::SetMtxWorld(const D3DXMATRIX& rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

HRESULT CObjectModel::SetVertexDecl
(
	const LPDIRECT3DDEVICE9 pDevice,
	const D3DVERTEXELEMENT9 *pDecl,
	bool bAutoComputeNormals,
	bool bAutoComputeTangents, 
	bool bSplitVertexForOptimalTangents
)
#if 0
{
	// �ϐ���錾
	HRESULT hr;		// 

	// �|�C���^��錾
	LPD3DXMESH pTempMesh = nullptr;	// ���b�V��
	
	if (m_modelData.pMesh)
	{
		// ���b�V���̕���
		hr = m_modelData.pMesh->CloneMesh(m_modelData.pMesh->GetOptions(), pDecl, pDevice, &pTempMesh);
		if (FAILED(hr))
		{
			SAFE_RELEASE(pTempMesh);
			return E_FAIL;
		}
	}
	
	
	// Check if the old declaration contains a normal.
	bool bHadNormal = false;
	bool bHadTangent = false;
	D3DVERTEXELEMENT9 aOldDecl[MAX_FVF_DECL_SIZE];
	if (m_modelData.pMesh && SUCCEEDED(m_modelData.pMesh->GetDeclaration(aOldDecl)))
	{
		for (UINT index = 0; index < D3DXGetDeclLength(aOldDecl); index++)
		{
			if (aOldDecl[index].Usage == D3DDECLUSAGE_NORMAL)
			{
				bHadNormal = true;
			}
			if (aOldDecl[index].Usage == D3DDECLUSAGE_TANGENT)
			{
				bHadTangent = true;
			}
		}
	}
	
	// Check if the new declaration contains a normal.
	bool bHaveNormalNow = false;
	bool bHaveTangentNow = false;
	D3DVERTEXELEMENT9 aNewDecl[MAX_FVF_DECL_SIZE];
	if (pTempMesh && SUCCEEDED(pTempMesh->GetDeclaration(aNewDecl)))
	{
		for (UINT index = 0; index < D3DXGetDeclLength(aNewDecl); index++)
		{
			if (aNewDecl[index].Usage == D3DDECLUSAGE_NORMAL)
			{
				bHaveNormalNow = true;
			}
			if (aNewDecl[index].Usage == D3DDECLUSAGE_TANGENT)
			{
				bHaveTangentNow = true;
			}
		}
	}
	
	SAFE_RELEASE(m_modelData.pMesh);
	
	if (pTempMesh)
	{
		m_modelData.pMesh = pTempMesh;

		if (!bHadNormal && bHaveNormalNow && bAutoComputeNormals)
		{
			// Compute normals in case the meshes have them
			D3DXComputeNormals(m_modelData.pMesh, nullptr);
		}

		if (bHaveNormalNow && !bHadTangent && bHaveTangentNow && bAutoComputeTangents)
		{
			ID3DXMesh* pNewMesh;

			DWORD *rgdwAdjacency = nullptr;
			rgdwAdjacency = new DWORD[m_modelData.pMesh->GetNumFaces() * 3];
			if (rgdwAdjacency == nullptr)
			{
				return E_OUTOFMEMORY;
			}

			//V(m_modelData.pMesh->GenerateAdjacency(1e-6f,rgdwAdjacency));
			if (FAILED(m_modelData.pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency)))
			{
				return E_FAIL;
			}

			float fPartialEdgeThreshold;
			float fSingularPointThreshold;
			float fNormalEdgeThreshold;
			if (bSplitVertexForOptimalTangents)
			{
				fPartialEdgeThreshold = 0.01f;
				fSingularPointThreshold = 0.25f;
				fNormalEdgeThreshold = 0.01f;
			}
			else
			{
				fPartialEdgeThreshold = -1.01f;
				fSingularPointThreshold = 0.01f;
				fNormalEdgeThreshold = -1.01f;
			}

			// Compute tangents, which are required for normal mapping
			hr = D3DXComputeTangentFrameEx
			( // ����
				m_modelData.pMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				rgdwAdjacency,
				fPartialEdgeThreshold,
				fSingularPointThreshold,
				fNormalEdgeThreshold,
				&pNewMesh,
				nullptr
			);

			SAFE_DEL_ARRAY(rgdwAdjacency);
			if (FAILED(hr))
			{
				return hr;
			}

			SAFE_RELEASE(m_modelData.pMesh);
			m_modelData.pMesh = pNewMesh;
		}
	}
	
	return S_OK;
}
#else
{
#if 0
	// �ϐ���錾
	HRESULT hr;		// 

	// �|�C���^��錾
	LPD3DXMESH pTempMesh = nullptr;	// ���b�V��
	
	if (m_modelData.pMesh)
	{
		// ���b�V���̕���
		hr = m_modelData.pMesh->CloneMesh(m_modelData.pMesh->GetOptions(), pDecl, pDevice, &pTempMesh);
		if (FAILED(hr))
		{
			SAFE_RELEASE(pTempMesh);
			return E_FAIL;
		}
	}
	
	
	// Check if the old declaration contains a normal.
	bool bHadNormal = false;
	bool bHadTangent = false;
	D3DVERTEXELEMENT9 aOldDecl[MAX_FVF_DECL_SIZE];
	if (m_modelData.pMesh && SUCCEEDED(m_modelData.pMesh->GetDeclaration(aOldDecl)))
	{
		for (UINT index = 0; index < D3DXGetDeclLength(aOldDecl); index++)
		{
			if (aOldDecl[index].Usage == D3DDECLUSAGE_NORMAL)
			{
				bHadNormal = true;
			}
			if (aOldDecl[index].Usage == D3DDECLUSAGE_TANGENT)
			{
				bHadTangent = true;
			}
		}
	}
	
	// Check if the new declaration contains a normal.
	bool bHaveNormalNow = false;
	bool bHaveTangentNow = false;
	D3DVERTEXELEMENT9 aNewDecl[MAX_FVF_DECL_SIZE];
	if (pTempMesh && SUCCEEDED(pTempMesh->GetDeclaration(aNewDecl)))
	{
		for (UINT index = 0; index < D3DXGetDeclLength(aNewDecl); index++)
		{
			if (aNewDecl[index].Usage == D3DDECLUSAGE_NORMAL)
			{
				bHaveNormalNow = true;
			}
			if (aNewDecl[index].Usage == D3DDECLUSAGE_TANGENT)
			{
				bHaveTangentNow = true;
			}
		}
	}
	
	SAFE_RELEASE(m_modelData.pMesh);
	
	if (pTempMesh)
	{
		m_modelData.pMesh = pTempMesh;

		if (!bHadNormal && bHaveNormalNow && bAutoComputeNormals)
		{
			// Compute normals in case the meshes have them
			D3DXComputeNormals(m_modelData.pMesh, nullptr);
		}

		if (bHaveNormalNow && !bHadTangent && bHaveTangentNow && bAutoComputeTangents)
		{
			ID3DXMesh* pNewMesh;

			DWORD *rgdwAdjacency = nullptr;
			rgdwAdjacency = new DWORD[m_modelData.pMesh->GetNumFaces() * 3];
			if (rgdwAdjacency == nullptr)
			{
				return E_OUTOFMEMORY;
			}

			//V(m_modelData.pMesh->GenerateAdjacency(1e-6f,rgdwAdjacency));
			if (FAILED(m_modelData.pMesh->GenerateAdjacency(1e-6f, rgdwAdjacency)))
			{
				return E_FAIL;
			}

			float fPartialEdgeThreshold;
			float fSingularPointThreshold;
			float fNormalEdgeThreshold;
			if (bSplitVertexForOptimalTangents)
			{
				fPartialEdgeThreshold = 0.01f;
				fSingularPointThreshold = 0.25f;
				fNormalEdgeThreshold = 0.01f;
			}
			else
			{
				fPartialEdgeThreshold = -1.01f;
				fSingularPointThreshold = 0.01f;
				fNormalEdgeThreshold = -1.01f;
			}

			// Compute tangents, which are required for normal mapping
			hr = D3DXComputeTangentFrameEx
			( // ����
				m_modelData.pMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				rgdwAdjacency,
				fPartialEdgeThreshold,
				fSingularPointThreshold,
				fNormalEdgeThreshold,
				&pNewMesh,
				nullptr
			);

			SAFE_DEL_ARRAY(rgdwAdjacency);
			if (FAILED(hr))
			{
				return hr;
			}

			SAFE_RELEASE(m_modelData.pMesh);
			m_modelData.pMesh = pNewMesh;
		}
	}
#endif

	// ��������ǉ�
	LPD3DXMESH pTempSysMemMesh = NULL;
	//LPD3DXMESH m_modelData.pMesh  = NULL;

	//���b�V�������R�s�[����
	if (m_modelData.pMesh)
	{
			if (FAILED(m_modelData.pMesh->CloneMesh(m_modelData.pMesh->GetOptions(),
				pDecl,
				pDevice, &pTempSysMemMesh)))
				return E_FAIL;
	}

	//���_�����Q�Ƃ��A�@���x�N�g���A�ڐ��x�N�g���A�]�@���x�N�g�������邩���ׂ�
	DWORD Normal = D3DX_DEFAULT;
	DWORD Tangent = D3DX_DEFAULT;
	DWORD Binormal = D3DX_DEFAULT;
	if (pTempSysMemMesh)
	{
			for (UINT index = 0; index < D3DXGetDeclLength(pDecl); ++index)
			{
				if (pDecl[index].Usage == D3DDECLUSAGE_NORMAL)
					Normal = D3DDECLUSAGE_NORMAL;
				else if (pDecl[index].Usage == D3DDECLUSAGE_TANGENT)
					Tangent = D3DDECLUSAGE_TANGENT;
				else if (pDecl[index].Usage == D3DDECLUSAGE_BINORMAL)
					Binormal = D3DDECLUSAGE_BINORMAL;
		   }
	}

	//SAFE_RELEASE( pTempSysMemMesh );

	if (pTempSysMemMesh)
	{
		m_modelData.pMesh = pTempSysMemMesh;


		if (m_modelData.pMesh)
		{
			//���_���Ɋ�Â����_�f�[�^���Đ�������   
			if (Normal != D3DX_DEFAULT || Tangent != D3DX_DEFAULT || Binormal != D3DX_DEFAULT)
			{
				//D3DXComputeTangent( m_modelData.pMesh, 0, 0, 0, 0, NULL );
				//D3DXComputeTangentFrame( m_modelData.pMesh, 0 );
				//m_modelData.pMesh->CloneMesh( m_modelData.pMesh->GetOptions(), pDecl,
				//                                 pDevice, &m_modelData.pMesh );

				D3DXComputeTangentFrameEx(m_modelData.pMesh,
					D3DDECLUSAGE_TEXCOORD,
					0,
					D3DDECLUSAGE_TANGENT,
					0,
					D3DDECLUSAGE_BINORMAL,
					0,
					D3DDECLUSAGE_NORMAL,
					0,
					0,
					NULL,
					0.01f,    //�{�P�.�l��������������Ƃڂ��Ȃ��Ȃ�
					0.25f,
					0.01f,
					&m_modelData.pMesh,
					NULL
				);
			}
			//���_�f�[�^���Đ������Ȃ�
			else
			{
				m_modelData.pMesh->CloneMesh(m_modelData.pMesh->GetOptions(),
					pDecl,
					pDevice,
					&m_modelData.pMesh);
			}
			//SAFE_RELEASE(m_modelData.pMesh);
		}
	}

	return S_OK;
}
#endif

//============================================================
//	���}�e���A���̐ݒ菈��
//============================================================
HRESULT CObjectModel::SetOriginMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	//--------------------------------------------------------
	//	�������J���E�m��
	//--------------------------------------------------------
	if (m_pMat != nullptr)
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = nullptr;
	}

	if (m_pMat == nullptr)
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �}�e���A�������̃������m��
		m_pMat = new D3DXMATERIAL[nNumMat];

		if (m_pMat != nullptr)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pMat, 0, sizeof(D3DXMATERIAL) * nNumMat);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�}�e���A������ݒ�
	//--------------------------------------------------------
	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���f�[�^���R�s�[
		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}

	// ������Ԃ�
	return S_OK;
}
