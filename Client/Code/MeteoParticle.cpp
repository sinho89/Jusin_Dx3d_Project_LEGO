#include "stdafx.h"
#include "MeteoParticle.h"

CMeteoParticle::CMeteoParticle(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos)
:CParticleObject(pDevice)
{
	m_vTargetPos = _Pos;
}

CMeteoParticle::~CMeteoParticle()
{

}

CMeteoParticle* CMeteoParticle::Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos)
{
	CMeteoParticle*		pMeteoParticle = new CMeteoParticle(pDevice, _Pos);

	if(FAILED(pMeteoParticle->Initialize()))
		ENGINE::Safe_Delete(pMeteoParticle);

	return pMeteoParticle;
}

void CMeteoParticle::Update(void)
{
	if(!m_bAni)
		return;

	int		i;

	FLOAT	ftime = m_fTimeAvg * 0.1f;

	m_fTime += 1.f * m_pTimeMgr->GetTime();

	if(m_fTime > 1)
	{
		m_bAlive = false;
		m_fTime  = 0;
	}
	for(i=0; i<m_PrtN; ++i)
	{
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];


		// 공기저항을 구한다.
		D3DXVECTOR3	vcAirR = pPrt->CrnV;					// 공기저항의 방향 벡터
		FLOAT		fLenV  = D3DXVec3LengthSq(&vcAirR);		// 속도의 제곱(Vx*Vx + Vy*Vy + Vz*Vz)크기 구함

		// 공기저항의 방향 벡터를 구한다.
		D3DXVec3Normalize(&vcAirR, &vcAirR);

		// 이동 속도와 반대로 설정
		vcAirR	*= -1.F;

		// 속력제곱 * 공기 저항 계수를 곱함.
		vcAirR	*= fLenV * pPrt->fDamp;


		// 1. 가속도에 공기저항을 더한다.
		pPrt->CrnA = pPrt->IntA + vcAirR;

		// 2. 현재 속도 갱신
		pPrt->CrnV += pPrt->CrnA * ftime;

		// 3. 현재 위치 갱신
		pPrt->CrnP += pPrt->CrnV * ftime;

		// 4. 경계값 설정
		if(pPrt->CrnP.y<0.f)
		{
			SetPart(i);
		}
	}



	// 카메라의 정보
	D3DXMATRIX mtView = *(m_pCameraMgr->GetView());

	D3DXVECTOR3 vcCamX(mtView._11, mtView._21, mtView._31);
	D3DXVECTOR3 vcCamY(mtView._12, mtView._22, mtView._32);
	D3DXVECTOR3 vcCamZ(mtView._13, mtView._23, mtView._33);

	for(i=0; i<m_PrtN; ++i)
	{	
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->CrnP;

		// 카메라의 Z축과 파티클의 위치와 내적
		pPrt->PrsZ	= D3DXVec3Dot(&vcP, &vcCamZ);
	}

	// Sorting
	/*qsort (m_PrtD
		, m_PrtN
		, sizeof(ENGINE::PARTICLE)
		, (int(*) (const void *, const void *)) CMcParticle::SortFnc);*/


	for(i=0; i<m_PrtN; ++i)
	{	
		ENGINE::PARTICLE* pPrt = &m_PrtD[i];

		D3DXVECTOR3	vcP	= pPrt->CrnP;
		D3DXCOLOR	xcC	= pPrt->dColor;

		FLOAT		fW = pPrt->PrsW;
		FLOAT		fH = pPrt->PrsH;
		FLOAT		fD = min(fW, fH);

		ENGINE::VTXPTC* pVtx = &m_pVtx[i*6 + 0];

		(pVtx+0)->vPos.x	= vcP.x - (vcCamX.x - vcCamY.x) * fW;
		(pVtx+0)->vPos.y	= vcP.y - (vcCamX.y - vcCamY.y) * fH;
		(pVtx+0)->vPos.z	= vcP.z - (vcCamX.z - vcCamY.z) * fD;
		(pVtx+0)->fU		= 0;
		(pVtx+0)->fV		= 0;
		(pVtx+0)->dwColor		= xcC;

		(pVtx+1)->vPos.x	= vcP.x + (vcCamX.x + vcCamY.x) * fW;
		(pVtx+1)->vPos.y	= vcP.y + (vcCamX.y + vcCamY.y) * fH;
		(pVtx+1)->vPos.z	= vcP.z + (vcCamX.z + vcCamY.z) * fD;
		(pVtx+1)->fU		= 1;
		(pVtx+1)->fV		= 0;
		(pVtx+1)->dwColor	= xcC;

		(pVtx+2)->vPos.x	= vcP.x - (vcCamX.x + vcCamY.x) * fW;
		(pVtx+2)->vPos.y	= vcP.y - (vcCamX.y + vcCamY.y) * fH;
		(pVtx+2)->vPos.z	= vcP.z - (vcCamX.z + vcCamY.z) * fD;
		(pVtx+2)->fU		= 0;
		(pVtx+2)->fV		= 1;
		(pVtx+2)->dwColor	= xcC;

		(pVtx+3)->vPos.x	= vcP.x + (vcCamX.x - vcCamY.x) * fW;
		(pVtx+3)->vPos.y	= vcP.y + (vcCamX.y - vcCamY.y) * fH;
		(pVtx+3)->vPos.z	= vcP.z + (vcCamX.z - vcCamY.z) * fD;
		(pVtx+3)->fU		= 1;
		(pVtx+3)->fV		= 1;
		(pVtx+3)->dwColor	= xcC;

		m_pVtx[i*6 + 4] = m_pVtx[i*6 + 2];
		m_pVtx[i*6 + 5] = m_pVtx[i*6 + 1];
	}

	SetAvgTime(0.005f / m_pTimeMgr->GetTime());
}

void CMeteoParticle::Render(void)
{
	if(!m_bAni)
		return;

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->SetTexture(0, m_pTx);
	m_pDevice->SetFVF(ENGINE::VTXPTC::FVF);

	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_PrtN * 2, m_pVtx, sizeof(ENGINE::VTXPTC));

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

D3DXMATRIX* CMeteoParticle::GetWorldMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CMeteoParticle::GetViewMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CMeteoParticle::GetProjMatrix(void)
{
	return NULL;
}

LPDIRECT3DDEVICE9* CMeteoParticle::GetDevice(void)
{
	return NULL;
}

void CMeteoParticle::SetAni(BOOL bAni/*=TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return;

	for(int i=0; i<m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CMeteoParticle::SetAvgTime(FLOAT fTime)
{
	m_fTimeAvg = fTime;
}

void CMeteoParticle::SetPos(D3DXVECTOR3 _Pos)
{
	m_vTargetPos = _Pos;
}

HRESULT CMeteoParticle::Initialize(void)
{
	m_PrtN	= 150;
	m_PrtD	= new ENGINE::PARTICLE[m_PrtN];


	m_iVtx = m_PrtN * 2 * 3;
	m_pVtx = new ENGINE::VTXPTC[m_iVtx];

	// 입자의 운동 출력용 설정
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

	return S_OK;
}

void CMeteoParticle::Release(void)
{

}

void CMeteoParticle::SetPart(int nIdx)
{
	ENGINE::PARTICLE* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// 각도 θ
	FLOAT	fPhi;		// 각도 φ
	FLOAT	fSpdR;		// 속도 크기

	// 초기 가속도
	pPrt->IntA = D3DXVECTOR3(0, -0.2F,0);

	//초기 속도와 위치를 설정하기 위한 변수
	fTheta	= float(rand()%61);
	fTheta	-=30.f;

	fPhi	= float(rand()%360);

	fSpdR = 100.f + rand()%101;
	fSpdR *=0.1f;

	// 라디안으로 변경
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);

	// 초기 속도
	pPrt->IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->IntV.y = fSpdR * cosf(fTheta);
	pPrt->IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// 초기 위치		
	pPrt->IntP.x = m_vTargetPos.x;
	pPrt->IntP.y = m_vTargetPos.y;
	pPrt->IntP.z = m_vTargetPos.z;


	// 탄성 계수 설정
	pPrt->fElst= (50 + rand()%51)*0.01f;

	// 공기저항 계수
	pPrt->fDamp= (100 + rand()%101)*0.005F;

	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;



	// 입자의 생명 요소
	float	fColor	= 30.f + rand()%71;
	fColor	*=0.01f;
	pPrt->dColor	=	D3DXCOLOR(0.7, 0.3, 0, fColor);


	// 입자의 표현 요소
	pPrt->PrsW = 1.5f + rand()%10;
	pPrt->PrsW *= 0.02f;

	pPrt->PrsH = 1.5f + rand()%10;
	pPrt->PrsH *= 0.02f;
}

