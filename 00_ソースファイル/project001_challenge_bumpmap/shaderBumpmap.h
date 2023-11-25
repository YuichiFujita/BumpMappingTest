//============================================================
//
//	�o���v�}�b�s���O�w�b�_�[ [shaderBumpmap.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BUMPMAP_H_
#define _BUMPMAP_H_

//************************************************************
//	�N���X��`
//************************************************************
// �o���v�}�b�s���O�N���X
class CBumpMap
{
public:
	// �R���X�g���N�^
	CBumpMap();

	// �f�X�g���N�^
	~CBumpMap();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��

	//void Invalidate();
	//void Restore();
	//HRESULT Load();

	void Begin(void);					// �J�n
	void BeginPass(const BYTE pass);	// �p�X�J�n
	void EndPass(void);					// �p�X�I��
	void End(void);						// �I��

	void SetAmbient(const float fAmbient);		// �����ݒ�
	void SetSpecular(const float fSpecular);	// ���ˌ��ݒ�
	void SetSpecularPower(const float fPower);	// ���˃n�C���C�g�ݒ�

	void SetMatrix	// �}�g���b�N�X�ݒ�
	( // ����
		D3DXMATRIX* pMtxWorld,		// ���[���h�}�g���b�N�X
		D3DXVECTOR4* pPosCamera,	// �J�����ʒu
		D3DXVECTOR4* pDirLight		// ���C�g����
	);

	void CommitChanges(void);			// ��ԕύX�̓`�B
	bool IsOK(void) const;				// �G�t�F�N�g�g�p�\�󋵂̎擾
	LPD3DXEFFECT GetEffect(void) const;	// �G�t�F�N�g�|�C���^�擾

	// �ÓI�����o�֐�
	static CBumpMap *Create(void);	// ����
	static HRESULT Release(CBumpMap *&prBumpMap);	// �j��

private:
	// �����o�ϐ�
	LPD3DXEFFECT m_pEffect;	// �G�t�F�N�g�|�C���^

	D3DXHANDLE m_pTechnique, m_pWVP, m_pLightDir, m_pEyePos, m_pAmbient, m_pSpecular, m_pSpecularPower;
	D3DXMATRIX m_matView, m_matProj;
};

#endif	// _BUMPMAP_H_
