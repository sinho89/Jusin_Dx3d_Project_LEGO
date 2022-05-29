/*!
 * \file Engine_struct.h
 * \date 2016/07/12 11:22
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Engine_struct_h__
#define Engine_struct_h__

namespace ENGINE
{
	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
	
	}VTXCOL;

	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR2		vTex;

	}VTXTEX;
	
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexCubeColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
	}VTXCUBECOL;

	const DWORD VTXFVF_CUBE_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertexCube
	{

		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;

	}VTXCUBE;

	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertexParticle
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
		FLOAT			fU, fV;

		tagVertexParticle() : vPos(0,0,0), fU(0), fV(0), dwColor(0xFFFFFFFF){}
		tagVertexParticle(FLOAT X, FLOAT Y, FLOAT Z
			, FLOAT U, FLOAT V
			, DWORD D=0xFFFFFFFF) : vPos(X,Y,Z), fU(U), fV(V), dwColor(D){}

		enum {	FVF= (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) };

	}VTXPTC;

	typedef struct tagVertexParticle2
	{
		D3DXVECTOR3	p;
		DWORD		d;

		tagVertexParticle2() : p(0,0,0), d(0xFFFFFFFF){}
		tagVertexParticle2(FLOAT X,FLOAT Y,FLOAT Z,DWORD D=0xFFFFFFFF): p(X,Y,Z), d(D){}
		enum { FVF =(D3DFVF_XYZ|D3DFVF_DIFFUSE) };
	}VTXPTC2;



	typedef struct tagSAVECUBE
	{
		ENGINE::SAVEOBJID	m_SaveType;
		
		float				m_fItv_X;
		float				m_fItv_Y;
		float				m_fItv_Z;

		float				m_fAngle[ANGLE_END];
		D3DXVECTOR3			m_vPos;
		D3DXVECTOR3			m_vDir;
		D3DXMATRIX			m_matWorld;

	}SAVECUBE;

	typedef struct InterverXYZ
	{
		float	m_wItvX;
		float	m_wItvY;
		float	m_wItvZ;
	}ITVXYZ;

	typedef struct tagIndex16
	{
		WORD		_1, _2, _3;

	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD		_1, _2, _3;

	}INDEX32;

	typedef struct tagTerrainInfo
	{
		VTXTEX*		m_pVertex;
		VTXTEX*		m_pConvertVertex;
		DWORD		m_dwVtxCnt;

		int			m_VtxCntX;
		int			m_VtxCntZ;
		int			m_Itv;

	}TERRAIN_INFO;


	typedef struct tagCamInfo
	{
		D3DXVECTOR3		m_vRight;
		D3DXVECTOR3		m_vUp;
		D3DXVECTOR3		m_vLook;
		D3DXVECTOR3		m_vPos;
		float			m_fSpeed;

	}CAM_INFO;


	typedef struct tagDivision
	{
		LPDIRECT3DTEXTURE9	pTx;
		LPDIRECT3DSURFACE9	pSf;

		tagDivision()
		{
			pTx	= NULL;
			pSf	= NULL;
		}

		void Invalidate()
		{
			ENGINE::Safe_Delete(	pTx	);
			ENGINE::Safe_Delete(	pSf	);
		}
	}DIVISION;

	typedef struct tagParticle
	{
		// ������ � ���
		D3DXVECTOR3		IntP;		// �ʱ� ��ġ
		D3DXVECTOR3		IntV;		// �ʱ� �ӵ�
		D3DXVECTOR3		IntA;		// �ʱ� �Ⱑ�ӵ�

		D3DXVECTOR3		CrnP;		// ���� ��ġ
		D3DXVECTOR3		CrnV;		// ���� �ӵ�
		D3DXVECTOR3		CrnA;		// ���� ���ӵ�

		D3DXVECTOR3		CrnR ;		// ���� ȸ��
		D3DXVECTOR3		CrnRv;		// ���� ȸ�� �ӵ�

		D3DXVECTOR3		CrnS ;		// ������

		FLOAT			fElst;		// ź��(Elastic) ���
		FLOAT			fDamp;		// ���� ����(Air Registance) ���

		// ������ ���� ���
		BOOL			bLive;		// Active (Yes/No)
		FLOAT			fLife;		// Particle fLife
		FLOAT			fFade;		// Fade Speed
		DWORD			dColor;		// Color


		// ������ ǥ�� ���
		FLOAT			PrsW;		// Present Billboard W
		FLOAT			PrsH;		// Present Billboard H
		FLOAT			PrsZ;		// ī�޶��� z��� ��ġ�� ���� ��

		INT				m_PrsImg;	// Image Index

	}PARTICLE;
}

#endif // Engine_struct_h__