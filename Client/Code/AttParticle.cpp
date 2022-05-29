#include "stdafx.h"
#include "AttParticle.h"
#include "SoundMgr.h"
CAttParticle::CAttParticle(LPDIRECT3DDEVICE9 pDevice)
:CParticleObject(pDevice)
{

}

CAttParticle::~CAttParticle()
{

}

CAttParticle* CAttParticle::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAttParticle*		pAttParticle = new CAttParticle(pDevice);

	if(FAILED(pAttParticle->Initialize()))
		ENGINE::Safe_Delete(pAttParticle);


	return pAttParticle;
}

HRESULT CAttParticle::Initialize(void)
{
	m_PrtN	= 800;
	m_PrtD	= new ENGINE::PARTICLE[m_PrtN];

	//m_pSoundMgr->EarthBoom(L"Explosion.wav");


	m_pDevice->CreateVertexBuffer(m_PrtN* sizeof(ENGINE::VTXPTC2)
		, D3DUSAGE_POINTS
		, ENGINE::VTXPTC2::FVF
		, D3DPOOL_MANAGED
		, &m_pVB, 0);

	// ������ � ��¿� ����
	D3DXCreateTextureFromFileEx(m_pDevice
		, L"../../Texture/Particle/alpha.png"
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, D3DX_DEFAULT
		, 0
		, D3DFMT_UNKNOWN
		, D3DPOOL_MANAGED
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, (D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR)
		, 0x00FFFFFF
		, NULL
		, NULL
		, &m_pTx
		);

	m_fColorRed = 0.f;
	m_fColorGreen = 0.f;
	m_fColorBlue = 0.f;
	m_fColorAlpha = 0.f;

	return S_OK;
}
void CAttParticle::Update(void)
{
	if(!m_bAni)
		return;

	int		i;

	FLOAT	ftime = m_fTimeAvg * 0.1f;

	D3DXMATRIX matS;
	D3DXMATRIX matW;

	for(i=0; i<m_PrtN; ++i)
	{
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];


		// ���������� ���Ѵ�.
		D3DXVECTOR3	vcAirR = pPrt->CrnV;					// ���������� ���� ����
		FLOAT		fLenV  = D3DXVec3LengthSq(&vcAirR);		// �ӵ��� ����(Vx*Vx + Vy*Vy + Vz*Vz)ũ�� ����

		// ���������� ���� ���͸� ���Ѵ�.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// �̵� �ӵ��� �ݴ�� ����
		vcAirR	*= -1.F;

		// �ӷ����� * ���� ���� ����� ����.
		vcAirR	*= fLenV * pPrt->fDamp;


		// 1. ���ӵ��� ���������� ���Ѵ�.
		pPrt->CrnA = pPrt->IntA + vcAirR;

		// 2. ���� �ӵ� ����
		pPrt->CrnV += pPrt->CrnA * ftime;

		// 3. ���� ��ġ ����
		pPrt->CrnP += pPrt->CrnV * ftime;

		// 4. ��谪 ����
		if(pPrt->CrnP.y<0.f)
		{
			SetPart(i);
		}
	}


	// ������ ��� Vertex Buffer�� ����
	ENGINE::VTXPTC2*	pVtx;
	m_pVB->Lock(0,0,(void**)&pVtx, 0);

	for(i=0; i<m_PrtN; ++i)
	{	
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		D3DXMatrixScaling( &matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

		//	// World matrix setup
		matW = matS;
		matW._41 =pPrt->CrnP.x;
		matW._42 =pPrt->CrnP.y;
		matW._43 =pPrt->CrnP.z;

		//	// vertex setup
		/*m_pVtx[i*6 + 0] = ENGINE::VTXPTC( -fW, +fH, 0, 0, 0, xcC);
		m_pVtx[i*6 + 1] = ENGINE::VTXPTC( +fW, +fH, 0, 1, 0, xcC);
		m_pVtx[i*6 + 2] = ENGINE::VTXPTC( -fW, -fH, 0, 0, 1, xcC);
		m_pVtx[i*6 + 3] = ENGINE::VTXPTC( +fW, -fH, 0, 1, 1, xcC);*/

		D3DXVec3TransformCoord( &(pVtx+0)->p, &(pVtx+0)->p, &matW);
		D3DXVec3TransformCoord( &(pVtx+1)->p, &(pVtx+1)->p, &matW);
		D3DXVec3TransformCoord( &(pVtx+2)->p, &(pVtx+2)->p, &matW);
		D3DXVec3TransformCoord( &(pVtx+3)->p, &(pVtx+3)->p, &matW);

		pVtx[i].p =	pPrt->CrnP;
		pVtx[i].d = pPrt->dColor;
	}

	m_pVB->Unlock();

	SetAvgTime(0.004f / m_pTimeMgr->GetTime());
}

void CAttParticle::Render(void)
{
	if(!m_bAni)
		return;


	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE); 
	m_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(20.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(5.0f));


	m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(1.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(2.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(3.0f));


	m_pDevice->SetTexture(0, m_pTx);
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(ENGINE::VTXPTC2));
	m_pDevice->SetFVF(ENGINE::VTXPTC2::FVF);

	m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_PrtN);

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

D3DXMATRIX* CAttParticle::GetWorldMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CAttParticle::GetViewMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CAttParticle::GetProjMatrix(void)
{
	return NULL;
}

LPDIRECT3DDEVICE9* CAttParticle::GetDevice(void)
{
	return NULL;
}	

void CAttParticle::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CAttParticle::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}


void CAttParticle::Release(void)
{

}

void CAttParticle::SetPart(int nIdx)
{
	ENGINE::PARTICLE* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// ���� ��
	FLOAT	fPhi;		// ���� ��
	FLOAT	fSpdR;		// �ӵ� ũ��

	// �ʱ� ���ӵ�
	pPrt->IntA = D3DXVECTOR3(0, -0.2F,0);

	//�ʱ� �ӵ��� ��ġ�� �����ϱ� ���� ����
	fTheta	= float(rand()%61);
	fTheta	-=30.f;

	fPhi	= float(rand()%360);

	fSpdR = 100.f + rand()%101;
	fSpdR *=0.1f;

	// �������� ����
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);

	// �ʱ� �ӵ�
	pPrt->IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->IntV.y = fSpdR * cosf(fTheta);
	pPrt->IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// �ʱ� ��ġ		
	pPrt->IntP.x = 350.f;
	pPrt->IntP.y = 80.f;
	pPrt->IntP.z = -15.f;

	// Scale��
	pPrt->CrnS.x	= ( 5 + rand()%101) /30000000.f;
	pPrt->CrnS.y	= (10 + rand()%101) /30000000.f;
	pPrt->CrnS.z	= ( 5 + rand()%101) /30000000.f;

	// ź�� ��� ����
	pPrt->fElst= (50 + rand()%51)*0.01f;

	// �������� ���
	pPrt->fDamp= (100 + rand()%101)*0.0001F;

	// �ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;


	// ������ ���� ���
	float	fColor = 30.f + rand() % 71;
	fColor	*= 0.01f;
	pPrt->dColor	=	D3DXCOLOR(1.f, 0.0f, 0.0f, 0.7f);

}

void CAttParticle::SetPos(D3DXVECTOR3 _vTargetPos)
{
	m_vTargetPos = _vTargetPos;
}

void CAttParticle::SetTarget(ENGINE::CComponent* _Target)
{
	m_pTargetInfo = _Target;
}

void CAttParticle::SetColor(float fColorRed, float fColorGreen, float fColorBlue, float fColorAlpha)
{
	m_fColorRed		= fColorRed;
	m_fColorGreen	= fColorGreen;
	m_fColorBlue	= fColorBlue;
	m_fColorAlpha	= fColorAlpha;
}
